// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Acc/AccC.h>
#include <Fog/Core/Global/Init_p.h>
#include <Fog/Core/Global/Private.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/Core/Memory/MemMgr.h>
#include <Fog/Core/Threading/Lock.h>
#include <Fog/Core/Tools/Swap.h>
#include <Fog/G2d/Painting/RasterApi_p.h>
#include <Fog/G2d/Painting/RasterScanline_p.h>
#include <Fog/G2d/Painting/RasterSpan_p.h>
#include <Fog/G2d/Painting/RasterStructs_p.h>
#include <Fog/G2d/Painting/Rasterizer_p.h>
#include <Fog/G2d/Tools/Region.h>
#include <Fog/G2d/Tools/RegionUtil_p.h>

namespace Fog {

// ============================================================================
// [Debug]
// ============================================================================

// #define FOG_DEBUG_RASTERIZER

#if defined(FOG_DEBUG_RASTERIZER)
struct FOG_NO_EXPORT RasterizerLogger
{
  static RasterizerLogger instance;

  FILE* f;
  RasterizerLogger()
  {
    f = fopen("Cell.txt", "w+");
  }

  static void out(const char* fmt, ...)
  {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(instance.f, fmt, ap);
    va_end(ap);
  }

  static void logChunk(int y, PathRasterizer8::Chunk* chunk)
  {
    if (chunk == NULL)
    {
      fprintf(instance.f, "%d - Null\n", y);
    }
    else
    {
      PathRasterizer8::Chunk* first = chunk;
      fprintf(instance.f, "%d - ", y);
      int sum = 0;

      do {
        fprintf(instance.f, "%d->%d [", chunk->x0, chunk->x1);

        for (size_t i = 0; i < chunk->getLength(); i++)
        {
          fprintf(instance.f, "%d|%d ", chunk->cells[i].cover, chunk->cells[i].area);
          sum += chunk->cells[i].cover;
        }
        fprintf(instance.f, "] ");

        chunk = chunk->next;
      } while (chunk != first);

      fprintf(instance.f, "COVER_SUM=%d", sum);
      fprintf(instance.f, "\n");
      fflush(instance.f);
      FOG_ASSERT(sum == 0);
    }
  }

  static void logCell(const char* _where, int x, int y, int cover, int area)
  {
    fprintf(instance.f, "%s [%3d %3d] ... [Cover=%d, Area=%d (%d*%d)]\n",
      _where, x, y,
      cover,
      area, cover ? area / cover : 0, cover);
    fflush(instance.f);
  }
};
RasterizerLogger RasterizerLogger::instance;

static void Rasterizer_dumpSpans(int y, const RasterSpan8* span)
{
  StringA b;
  b.appendFormat("Y=%d - ", y);

  while (span)
  {
    if (span->isConst())
    {
      b.appendFormat("C[%d %d]%0.2X", span->_x0, span->_x1, span->getConstMask());
    }
    else
    {
      b.appendFormat("M[%d %d]", span->_x0, span->_x1);
      for (int x = 0; x < span->getLength(); x++)
        b.appendFormat("%0.2X", reinterpret_cast<uint8_t*>(span->_mask)[x]);
    }
    b.append(' ');
    span = span->getNext();
  }

  printf("%s\n", b.getData());
}

static void PathRasterizer8_verifyChunks(PathRasterizer8::Chunk* first)
{
  PathRasterizer8::Chunk* chunk = first;

  int sum = 0;

  FOG_ASSERT_X(chunk->x0 < first->prev->x1,
    "Fog::PathRasterizer8::verifyChunks() - Invalid first chunk.");

  do {
    PathRasterizer8::Chunk* next = chunk->next;

    FOG_ASSERT(chunk->x0 <  chunk->x1);
    FOG_ASSERT(chunk->x1 <= next->x0 || next == first);

    int length = chunk->x1 - chunk->x0;
    for (int i = 0; i < length; i++)
    {
      sum += chunk->cells[i].cover;
    }

    chunk = next;
  } while (chunk != first);

  FOG_ASSERT_X(sum == 0,
    "Fog::PathRasterizer8::verifyChunks() - Invalid sum of cells, must be zero.");
}

# define LOG_CELL(_Where_, _X_, _Y_, _Cover_, _Area_) RasterizerLogger::logCell(_Where_, _X_, _Y_, _Cover_, _Area_)
# define VERIFY_CHUNKS_8(_First_) PathRasterizer8_verifyChunks(_First_)
#else
# define LOG_CELL(_Where_, _X_, _Y_, _Cover_, _Area_) FOG_NOP
# define VERIFY_CHUNKS_8(_First_) FOG_NOP
#endif // FOG_DEBUG_RASTERIZER

// ============================================================================
// [Helpers]
// ============================================================================

enum A8_ENUM
{
  A8_SHIFT   = 8,             // 8
  A8_SHIFT_2 = A8_SHIFT + 1,  // 9

  A8_SCALE   = 1 << A8_SHIFT, // 256
  A8_SCALE_2 = A8_SCALE * 2,  // 512

  A8_MASK    = A8_SCALE - 1,  // 255
  A8_MASK_2  = A8_SCALE_2 - 1,// 511

  A8_I32_COORD_LIMIT = 16384 << A8_SHIFT,

  A8_ALLOCATOR_SIZE = 16384 - 84,
  A8_MAX_CHUNK_LENGTH = A8_ALLOCATOR_SIZE / (sizeof(PathRasterizer8::Cell) * 8)
};

enum CELL_OP
{
  CELL_OP_COPY  = 0x0,
  CELL_OP_MERGE = 0x2,

  CELL_OP_POSITIVE = 0x0,
  CELL_OP_NEGATIVE = 0x1
};

static FOG_INLINE int upscale24x8(float  v) { return Math::ifloor(v * (float )256 + 0.5f); }
static FOG_INLINE int upscale24x8(double v) { return Math::ifloor(v * (double)256 + 0.5 ); }

#define NEW_SPAN(_Span_, _OnFail_) \
  FOG_MACRO_BEGIN \
    RasterSpan8* _newSpan = scanline->allocSpan(); \
    if (FOG_IS_NULL(_newSpan)) \
    { \
      _OnFail_; \
    } \
    _Span_->_next = _newSpan; \
    _Span_ = _newSpan; \
  FOG_MACRO_END

#define NEW_SPANS(_Span_, _Count_, _OnFail_) \
  FOG_MACRO_BEGIN \
    RasterSpan8* _newSpan = scanline->allocSpans(_Count_); \
    if (FOG_IS_NULL(_newSpan)) \
    { \
      _OnFail_; \
    } \
    _Span_->_next = _newSpan; \
    _Span_ = _newSpan; \
  FOG_MACRO_END

// ============================================================================
// [Fog::RasterizerApi]
// ============================================================================

FOG_NO_EXPORT RasterizerApi Rasterizer_api;

// ============================================================================
// [Fog::BoxRasterizer8 - Init - 32x0]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_init32x0(BoxRasterizer8* self, const BoxI* box)
{
  // The box should be already clipped to the scene-box.
  FOG_ASSERT(self->_sceneBox.subsumes(*box));
  FOG_ASSERT(self->_clipType < RASTER_CLIP_COUNT);

  self->_initialized = true;
  self->_boxBounds = *box;
  self->_render = Rasterizer_api.box8.render_32x0[self->_clipType];
}

// ============================================================================
// [Fog::BoxRasterizer8 - Init - 24x8]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_init24x8(BoxRasterizer8* self, const BoxI* box24x8)
{
  // The box should be already clipped to the scene-box.
  FOG_ASSERT(self->_sceneBox24x8.subsumes(*box24x8));
  FOG_ASSERT(self->_clipType < RASTER_CLIP_COUNT);

  // Extract box coordinates.
  int x0 = box24x8->x0;
  int y0 = box24x8->y0;
  int x1 = box24x8->x1;
  int y1 = box24x8->y1;

  self->_initialized = (x0 < x1) & (y0 < y1);
  if (!self->_initialized) return;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  self->_boxBounds.x0 = (x0 >> 8);
  self->_boxBounds.y0 = (y0 >> 8);
  self->_boxBounds.x1 = (x1 >> 8);
  self->_boxBounds.y1 = (y1 >> 8);

  self->_box24x8.x0 = x0;
  self->_box24x8.y0 = y0;
  self->_box24x8.x1 = x1;
  self->_box24x8.y1 = y1;

  uint32_t fx0 = x0 & 0xFF;
  uint32_t fy0 = y0 & 0xFF;
  uint32_t fx1 = x1 & 0xFF;
  uint32_t fy1 = y1 & 0xFF;

  self->_boxBounds.x1 += fx1 != 0;
  self->_boxBounds.y1 += fy1 != 0;

  if (fx1 == 0) fx1 = 256;
  if (fy1 == 0) fy1 = 256;

  uint32_t hl = 256 - fx0;
  uint32_t hr = fx1;
  uint32_t vt = 256 - fy0;
  uint32_t vb = fy1;

  uint32_t opacity = self->_opacity;

  if ((x0 & ~0xFF) == (x1 & ~0xFF))
  {
    hr -= hl;
    hl  = hr;
  }

  if ((y0 & ~0xFF) == (y1 & ~0xFF))
  {
    vb -= vt;
    vt  = vb;
  }

  vt *= opacity;
  vb *= opacity;

  // --------------------------------------------------------------------------
  // [Corners]
  // --------------------------------------------------------------------------

  self->_ct[0] = (uint16_t)( (hl * vt) >> 16 );
  self->_ct[1] = (uint16_t)( (vt     ) >>  8 );
  self->_ct[2] = (uint16_t)( (hr * vt) >> 16 );

  self->_ci[0] = (uint16_t)( (hl * opacity) >> 8 );
  self->_ci[1] = (uint16_t)( opacity             );
  self->_ci[2] = (uint16_t)( (hr * opacity) >> 8 );

  self->_cb[0] = (uint16_t)( (hl * vb) >> 16 );
  self->_cb[1] = (uint16_t)( (vb     ) >>  8 );
  self->_cb[2] = (uint16_t)( (hr * vb) >> 16 );

  self->_render = Rasterizer_api.box8.render_24x8[self->_clipType];
}

// ============================================================================
// [Fog::BoxRasterizer8 - Render - 32x0 - Clip-Box]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_render_32x0_st_clip_box(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* _scanline)
{
  FOG_UNUSED(_scanline);

  BoxRasterizer8* self = static_cast<BoxRasterizer8*>(_self);
  const BoxI& box = self->_boxBounds;

  int y0 = box.y0;
  int y1 = box.y1;
  int i;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  filler->prepare(y0);
  RasterFiller::ProcessFunc process = filler->_process;

  RasterSpan8 span[1];
  span[0].setPositionAndType(box.x0, box.x1, RASTER_SPAN_C);
  span[0].setConstMask(self->_opacity);
  span[0].setNext(NULL);

  // --------------------------------------------------------------------------
  // [Process]
  // --------------------------------------------------------------------------

  for (i = y1 - y0; i; i--)
    process(filler, span);
}

// ============================================================================
// [Fog::BoxRasterizer8 - Render - 32x0 - Clip-Region]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_render_32x0_st_clip_region(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  BoxRasterizer8* self = static_cast<BoxRasterizer8*>(_self);
  const BoxI& box = self->_boxBounds;

  int x0 = box.x0;
  int x1 = box.x1;

  int y0 = box.y0;
  int y1 = box.y1;

  const BoxI* cPtr = self->_clip.region.data;
  const BoxI* cEnd = cPtr + self->_clip.region.length;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  // Skip boxes which do not intersect in vertical direction.
  while (cPtr->y1 <= y0)
  {
    if (++cPtr == cEnd)
      return;
  }

  if (cPtr->y0 >= y1)
    return;

  if (y0 < cPtr->y0)
    y0 = cPtr->y0;

  int yEnd = y1;
  int yPos = y0;

  uint32_t opacity = self->_opacity;

  filler->prepare(yPos);
  RasterFiller::ProcessFunc process = filler->_process;

  // --------------------------------------------------------------------------
  // [Process]
  // --------------------------------------------------------------------------

  do {
    // Skip clip boxes which do not intersect with the shape-box.
    while (cPtr->x1 <= x0)
    {
_Start:
      if (++cPtr == cEnd)
        return;
    }

    while (cPtr->x0 >= x1)
    {
      if (++cPtr == cEnd)
        return;
    }

    y0 = cPtr->y0;
    if (y0 >= yEnd)
      return;

    if (cPtr->x1 <= x0)
      goto _Start;

    if (y0 < box.y0)
      y0 = box.y0;
    y1 = cPtr->y1;

    // Build a scanline.
    RasterSpan8* span = scanline->begin();

    // We need to care about x0 in the first span. Another spans have to be
    // inside of the painting box, so we need only to handle x1, which can be
    // outside (also applies to x0 in case that one clip box completely subsumes
    // the painting one).
    NEW_SPAN(span, return);
    span->setPositionAndType(Math::max<int>(x0, cPtr->x0), Math::min<int>(x1, cPtr->x1), RASTER_SPAN_C);
    span->setConstMask(opacity);

    for (;;)
    {
      if (++cPtr == cEnd)
        break;

      if (cPtr->y1 != y1)
        break;

      if (cPtr->x0 < x1)
      {
        NEW_SPAN(span, return);
        span->setPositionAndType(cPtr->x0, Math::min<int>(x1, cPtr->x1), RASTER_SPAN_C);
        span->setConstMask(opacity);
      }
    }

    if (y1 > yEnd)
      y1 = yEnd;

    span = scanline->end(span);
    if (span != NULL)
    {
      if (yPos != y0)
        filler->_skip(filler, y0 - yPos);

      do {
        process(filler, span);
      } while (++y0 != y1);
      yPos = y1;
    }
  } while (cPtr != cEnd);
}

// ============================================================================
// [Fog::BoxRasterizer8 - Render - 32x0 - Clip-Mask]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_render_32x0_st_clip_mask(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  // TODO: BoxRasterizer8 - render 32x0 - clip-mask.
  BoxRasterizer8* self = static_cast<BoxRasterizer8*>(_self);
}

// ============================================================================
// [Fog::BoxRasterizer8 - Render - 24x8 - Clip-Box]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_render_24x8_st_clip_box(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* _scanline)
{
  BoxRasterizer8* self = static_cast<BoxRasterizer8*>(_self);
  const BoxI& box = self->_boxBounds;

  FOG_UNUSED(_scanline);

  int y0 = box.y0;
  int y1 = box.y1 - 1;

  uint w = box.getWidth();
  uint i;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  filler->prepare(y0);
  RasterFiller::ProcessFunc process = filler->_process;

  RasterSpan8 span[3];
  uint16_t mask[10];

  // --------------------------------------------------------------------------
  // [Process]
  // --------------------------------------------------------------------------

  if (w < 10)
  {
#define SETUP_MASK(_Coverage_) \
    FOG_MACRO_BEGIN \
      mask[0] = _Coverage_[0]; \
      for (i = 1; i < w-1; i++) mask[i] = _Coverage_[1]; \
      mask[i] = _Coverage_[2]; \
    FOG_MACRO_END

    span[0].setPositionAndType(box.x0, box.x1, RASTER_SPAN_AX_EXTRA);
    span[0].setA8Extra(reinterpret_cast<uint8_t*>(mask));
    span[0].setNext(NULL);

    // Render top part (or top+bottom part in case that the box is 1 pixel height).
    SETUP_MASK(self->_ct);
    process(filler, span);
    if (y0 >= y1)
      return;

    // Render inner part.
    SETUP_MASK(self->_ci);
    while (++y0 < y1)
      process(filler, span);

    // Render bottom part.
    SETUP_MASK(self->_cb);
    process(filler, span);
#undef SETUP_MASK
  }
  else
  {
#define SETUP_MASK(_Coverage_) \
    FOG_MACRO_BEGIN \
      span[0].setConstMask(_Coverage_[0]); \
      span[1].setConstMask(_Coverage_[1]); \
      span[2].setConstMask(_Coverage_[2]); \
    FOG_MACRO_END

    span[0].setPositionAndType(box.x0    , box.x0 + 1, RASTER_SPAN_C);
    span[0].setNext(&span[1]);

    span[1].setPositionAndType(box.x0 + 1, box.x1 - 1, RASTER_SPAN_C);
    span[1].setNext(&span[2]);

    span[2].setPositionAndType(box.x1 - 1, box.x1    , RASTER_SPAN_C);
    span[2].setNext(NULL);

    // Render top part (or top+bottom part in case that the box is 1 pixel height).
    SETUP_MASK(self->_ct);
    process(filler, span);
    if (y0 >= y1)
      return;

    // Render inner part.
    SETUP_MASK(self->_ci);
    while (++y0 < y1)
      process(filler, span);

    // Render bottom part.
    SETUP_MASK(self->_cb);
    process(filler, span);
#undef SETUP_MASK
  }
}

// ============================================================================
// [Fog::BoxRasterizer8 - Render - 24x8 - Clip-Region]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_render_24x8_st_clip_region(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  BoxRasterizer8* self = static_cast<BoxRasterizer8*>(_self);
  const BoxI& box = self->_boxBounds;

  int x0 = box.x0;
  int x1 = box.x1;

  int y0 = box.y0;
  int y1 = box.y1;

  const BoxI* cPtr = self->_clip.region.data;
  const BoxI* cEnd = cPtr + self->_clip.region.length;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  // Skip boxes which do not intersect in vertical direction.
  while (cPtr->y1 <= y0)
  {
    if (++cPtr == cEnd)
      return;
  }

  if (cPtr->y0 >= y1)
    return;

  if (y0 < cPtr->y0)
    y0 = cPtr->y0;

  int yEnd = y1;
  int yPos = y0;

  int yTop = box.y0;
  int yBottom = y1 - 1;

  filler->prepare(yPos);
  RasterFiller::ProcessFunc process = filler->_process;

  // --------------------------------------------------------------------------
  // [Process]
  // --------------------------------------------------------------------------

  do {
    const BoxI* cMark;
    const uint16_t* covers;

    RasterSpan8* span;
    RasterSpan8* rightSpan;

    // Skip clip boxes which do not intersect with the shape-box.
    while (cPtr->x1 <= x0)
    {
_Start:
      if (++cPtr == cEnd)
        return;
    }

    while (cPtr->x0 >= x1)
    {
      if (++cPtr == cEnd)
        return;
    }

    y0 = cPtr->y0;
    if (y0 >= yEnd)
      return;

    if (cPtr->x1 <= x0)
      goto _Start;

    if (y0 < yTop)
      y0 = yTop;
    y1 = cPtr->y1;

    // Build a scanline.
    cMark = cPtr;

_Repeat:
    span = scanline->begin();
    covers = self->_covers + 4 * ((y0 != yTop) + (y0 == yBottom));

    // We need to care about x0 in the first span. Another spans have to be
    // inside of the painting box, so we need only to handle x1, which can be
    // outside (also applies to x0 in case that one clip box completely subsumes
    // the painting one).
    NEW_SPANS(span, 3, return);
    span->setPositionAndType(Math::max<int>(x0, cPtr->x0), Math::min<int>(x1, cPtr->x1), RASTER_SPAN_C);
    span->setConstMask(covers[1]);

    // Used only if last pixel is inside the region.
    rightSpan = &span[2];

    // Handle the left coverage if not clipped out.
    if (span->getX0() == x0)
    {
      span->setConstMask(covers[0]);
      if (span->getLength() > 1)
      {
        span[1].setPositionAndType(x0 + 1, span[0].getX1(), RASTER_SPAN_C);
        span[1].setConstMask(covers[1]);
        span[0].setX1(x0 + 1);
        span[0].setNext(&span[1]);
        span++;
      }
    }

    for (;;)
    {
      if (++cPtr == cEnd)
        break;

      if (cPtr->y1 != y1)
        break;

      if (cPtr->x0 < x1)
      {
        NEW_SPAN(span, return);
        span->setPositionAndType(cPtr->x0, Math::min<int>(x1, cPtr->x1), RASTER_SPAN_C);
        span->setConstMask(covers[1]);
      }
    }

    if (y1 > yEnd)
      y1 = yEnd;
    
    // Handle the right coverage if not clipped out.
    if (span->getX1() == x1)
    {
      if (span->getLength() > 1)
      {
        span->_x1--;
        span->setNext(rightSpan);
        span = rightSpan;
        span->setPositionAndType(x1 - 1, x1, RASTER_SPAN_C);
        span->setConstMask(covers[2]);
      }
      else if (span->getX0() != x0)
      {
        span->setConstMask(covers[2]);
      }
    }

    span = scanline->end(span);
    if (span != NULL)
    {
      if (yPos != y0)
        filler->_skip(filler, y0 - yPos);

      // Because there are 3 cases, we need to stop filling if we advance from top-inner
      // or inner-bottom covers. Instead of patching scanlines we simply run the process
      // again, the correct coverage will be set at the start of loop.
      yPos = y1;

      if (y0 == yTop)
        yPos = y0 + 1;
      else if (y1 == box.y1 && y0 != yBottom)
        yPos = yBottom;

      do {
        process(filler, span);
      } while (++y0 != yPos);

      if (y1 != yPos)
      {
        cPtr = cMark;
        goto _Repeat;
      }
    }
  } while (cPtr != cEnd);
}

// ============================================================================
// [Fog::BoxRasterizer8 - Render - 24x8 - Clip-Mask]
// ============================================================================

static void FOG_CDECL BoxRasterizer8_render_24x8_st_clip_mask(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  // TODO: BoxRasterizer8 - render 24x8 - clip-mask.
  BoxRasterizer8* self = static_cast<BoxRasterizer8*>(_self);
}

// ============================================================================
// [Fog::PathRasterizer8 - Construction / Destruction]
// ============================================================================

PathRasterizer8::PathRasterizer8() :
  _allocator(A8_ALLOCATOR_SIZE)
{
  // Default is no multithreading.
  _scope.reset();

  // Clear rows.
  _rowsCapacity = 0;
  _rowsStorage = NULL;
  _rowsAdjusted = NULL;

  reset();
}

PathRasterizer8::~PathRasterizer8()
{
  if (_rowsStorage != NULL)
    MemMgr::free(_rowsStorage);
}

// ============================================================================
// [Fog::PathRasterizer8 - Helpers]
// ============================================================================

template<typename FixedT>
static bool PathRasterizer8_renderLine(PathRasterizer8* self, FixedT x0, FixedT y0, FixedT x1, FixedT y1);

// ============================================================================
// [Fog::PathRasterizer8 - Reset]
// ============================================================================

void PathRasterizer8::reset()
{
  // Reset scene-box.
  _sceneBox.reset();
  _boundingBox.setBox(-1, -1, -1, -1);

  // Reset error.
  _error = ERR_OK;
  // Reset opacity.
  _opacity = 0x100;

  // Reset fill-rule.
  _fillRule = FILL_RULE_DEFAULT;
  // Not valid neither finalized.
  _isValid = false;
  _isFinalized = false;
}

// ============================================================================
// [Fog::PathRasterizer8 - Init]
// ============================================================================

err_t PathRasterizer8::init()
{
  _allocator.clear();
  _boundingBox.setBox(-1, -1, -1, -1);

  _error = ERR_OK;
  _isValid = false;
  _isFinalized = false;

  _sceneBox24x8.setBox(_sceneBox.x0 << 8, _sceneBox.y0 << 8, _sceneBox.x1 << 8, _sceneBox.y1 << 8);

  _offsetF.reset();
  _offsetD.reset();
  uint i = (uint)_sceneBox.getHeight() + 1;

  if (_rowsCapacity < i)
  {
    if (_rowsStorage != NULL)
      MemMgr::free(_rowsStorage);

    // Align...
    _rowsCapacity = (i + 255U) & ~255U;
    _rowsStorage = reinterpret_cast<Row*>(MemMgr::alloc(_rowsCapacity * sizeof(Row)));
    _rowsAdjusted = NULL;

    if (_rowsStorage == NULL)
    {
      _rowsCapacity = 0;
      setError(ERR_RT_OUT_OF_MEMORY);
      return _error;
    }
  }

  _rowsAdjusted = _rowsStorage - _sceneBox.y0;
  return _error;
}

// ============================================================================
// [Fog::PathRasterizer8 - AddPath]
// ============================================================================

template<typename SrcT>
static void PathRasterizer8_addPathData(PathRasterizer8* self,
  const SrcT_(Point)* srcPts, const uint8_t* srcCmd, size_t count, const SrcT_(Point)& offset)
{
  if (count == 0)
    return;

  const uint8_t* srcEnd = srcCmd + count;

  // Current/Start moveTo x position.
  Fixed24x8 x0, startX0;
  // Current/Start moveTo y position.
  Fixed24x8 y0, startY0;

  // Current command.
  uint8_t c;

  // Stack.
  PointI curveStack[32 * 3 + 1];
  int levelStack[32];

_Start:
  // Find: 'move-to' command and then go to the second loop.
  for (;;)
  {
    c = srcCmd[0];
    srcPts++;
    srcCmd++;

    // This can happen only when 'move-to' is the last command. In this case
    // there is nothing to rasterize.
    if (srcCmd == srcEnd)
      return;

    // ------------------------------------------------------------------------
    // [MoveTo]
    // ------------------------------------------------------------------------

    if (PathCmd::isMoveTo(c))
    {
_MoveTo:
      x0 = startX0 = Math::bound<Fixed24x8>(upscale24x8(srcPts[-1].x + offset.x), self->_sceneBox24x8.x0, self->_sceneBox24x8.x1);
      y0 = startY0 = Math::bound<Fixed24x8>(upscale24x8(srcPts[-1].y + offset.y), self->_sceneBox24x8.y0, self->_sceneBox24x8.y1);
      break;
    }
  }

  // Process: 'line-to', 'quad-to' and 'cubic-to'.
  // Stop at: 'move-to', 'close'.
  for (;;)
  {
    c = srcCmd[0];

    // ------------------------------------------------------------------------
    // [LineTo]
    // ------------------------------------------------------------------------

    if (PathCmd::isLineTo(c))
    {
      Fixed24x8 x1 = Math::bound<Fixed24x8>(upscale24x8(srcPts[0].x + offset.x), self->_sceneBox24x8.x0, self->_sceneBox24x8.x1);
      Fixed24x8 y1 = Math::bound<Fixed24x8>(upscale24x8(srcPts[0].y + offset.y), self->_sceneBox24x8.y0, self->_sceneBox24x8.y1);

      if ((x0 != x1) | (y0 != y1) && !PathRasterizer8_renderLine<int>(self, x0, y0, x1, y1))
        return;

      x0 = x1;
      y0 = y1;

      srcPts++;
      srcCmd++;

      if (srcCmd == srcEnd)
        goto _ClosePath;
    }

    // ------------------------------------------------------------------------
    // [QuadTo]
    // ------------------------------------------------------------------------

    else if (PathCmd::isQuadTo(c))
    {
      PointI* curve = curveStack;

      curve[0].x = upscale24x8(srcPts[ 1].x + offset.x);
      curve[0].y = upscale24x8(srcPts[ 1].y + offset.y);
      curve[1].x = upscale24x8(srcPts[ 0].x + offset.x);
      curve[1].y = upscale24x8(srcPts[ 0].y + offset.y);
      curve[2].x = x0;
      curve[2].y = y0;

      int d = Math::max(Math::abs( curve[2].x + curve[0].x - 2 * curve[1].x),
                        Math::abs( curve[2].y + curve[0].y - 2 * curve[1].y));

      int level = 0;
      int top = 0;

      while (d > (A8_SCALE / 6))
      {
        d >>= 2;
        level++;
      }

      levelStack[0] = level;
      top = 0;

      do {
        level = levelStack[top];
        if (level > 1)
        {
          int a, b;

          curve[4].x = curve[2].x;
          b = curve[1].x;
          a = curve[3].x = (curve[2].x + b) / 2;
          b = curve[1].x = (curve[0].x + b) / 2;
          curve[2].x = (a + b) / 2;

          curve[4].y = curve[2].y;
          b = curve[1].y;
          a = curve[3].y = (curve[2].y + b) / 2;
          b = curve[1].y = (curve[0].y + b) / 2;
          curve[2].y = (a + b) / 2;

          curve += 2;
          top++;
          levelStack[top] = levelStack[top - 1] = level - 1;
          continue;
        }

        // LineTo.
        Fixed24x8 x1 = Math::bound<Fixed24x8>(curve[0].x, self->_sceneBox24x8.x0, self->_sceneBox24x8.x1);
        Fixed24x8 y1 = Math::bound<Fixed24x8>(curve[0].y, self->_sceneBox24x8.y0, self->_sceneBox24x8.y1);

        if (!PathRasterizer8_renderLine<int>(self, x0, y0, x1, y1))
          return;

        x0 = x1;
        y0 = y1;

        top--;
        curve -= 2;
      } while (top >= 0);

      srcPts += 2;
      srcCmd += 2;

      if (srcCmd == srcEnd)
        goto _ClosePath;
    }

    // ------------------------------------------------------------------------
    // [CubicTo]
    // ------------------------------------------------------------------------

    else if (PathCmd::isCubicTo(c))
    {
      PointI* curve = curveStack;
      PointI* curveEnd = curve + 31 * 3;

      curve[0].x = upscale24x8(srcPts[ 2].x + offset.x);
      curve[0].y = upscale24x8(srcPts[ 2].y + offset.y);
      curve[1].x = upscale24x8(srcPts[ 1].x + offset.x);
      curve[1].y = upscale24x8(srcPts[ 1].y + offset.y);
      curve[2].x = upscale24x8(srcPts[ 0].x + offset.x);
      curve[2].y = upscale24x8(srcPts[ 0].y + offset.y);
      curve[3].x = x0;
      curve[3].y = y0;

      for (;;)
      {
        // Decide whether to split or draw. See `Rapid Termination. Evaluation
        // for Recursive Subdivision of Bezier Curves' by Thomas F. Hain, at:
        // http://www.cis.southalabama.edu/~hain/general/Publications/Bezier/Camera-ready%20CISST02%202.pdf
        if (curve != curveEnd)
        {
          int dx, dy;
          int dx_, dy_;
          int dx1, dy1;
          int dx2, dy2;
          int L, s, limit;

          // dx and dy are x and y components of the P0-P3 chord vector.
          dx = curve[3].x - curve[0].x;
          dy = curve[3].y - curve[0].y;

          // L is an (under)estimate of the Euclidean distance P0-P3.
          //
          // If dx >= dy, then r = sqrt(dx^2 + dy^2) can be overestimated
          // with least maximum error by
          //
          //   r_upperbound = dx + (sqrt(2) - 1) * dy,
          //
          // where sqrt(2) - 1 can be (over)estimated by 107/256, giving an
          // error of no more than 8.4%.
          //
          // Similarly, some elementary calculus shows that r can be
          // underestimated with least maximum error by
          //
          //   r_lowerbound = sqrt(2 + sqrt(2)) / 2 * dx
          //                + sqrt(2 - sqrt(2)) / 2 * dy .
          //
          // 236/256 and 97/256 are (under)estimates of the two algebraic
          // numbers, giving an error of no more than 8.1%.
          dx_ = Math::abs(dx);
          dy_ = Math::abs(dy);

          // This is the same as:
          //
          //   L = (236 * max( dx_, dy_ ) + 97 * min( dx_, dy_ )) >> 8;
          L = ((dx_ > dy_) ? (236 * dx_ +  97 * dy_)
                           : ( 97 * dx_ + 236 * dy_)) >> 8;

          // Avoid possible arithmetic overflow below by splitting.
          if (L > 32767)
            goto _CBezierSplit;

          // Max deviation may be as much as (s/L) * 3/4 (if Hain's v = 1).
          limit = L * (int)(A8_SCALE / 6);

          // s is L * the perpendicular distance from P1 to the line P0-P3.
          dx1 = curve[1].x - curve[0].x;
          dy1 = curve[1].y - curve[0].y;
          s = Math::abs(dy * dx1 - dx * dy1);

          if (s > limit)
            goto _CBezierSplit;

          // s is L * the perpendicular distance from P2 to the line P0-P3.
          dx2 = curve[2].x - curve[0].x;
          dy2 = curve[2].y - curve[0].y;
          s = Math::abs(dy * dx2 - dx * dy2);

          if (s > limit)
            goto _CBezierSplit;

          // If P1 or P2 is outside P0-P3, split the curve.
          if ((dy * dy1 + dx * dx1 < 0) ||
              (dy * dy2 + dx * dx2 < 0) ||
              (dy * (curve[3].y - curve[1].y) + dx * (curve[3].x - curve[1].x) < 0) ||
              (dy * (curve[3].y - curve[2].y) + dx * (curve[3].x - curve[2].x) < 0))
            goto _CBezierSplit;
        }

        {
          Fixed24x8 x1 = Math::bound<Fixed24x8>(curve[0].x, self->_sceneBox24x8.x0, self->_sceneBox24x8.x1);
          Fixed24x8 y1 = Math::bound<Fixed24x8>(curve[0].y, self->_sceneBox24x8.y0, self->_sceneBox24x8.y1);

          if (!PathRasterizer8_renderLine<int>(self, x0, y0, x1, y1))
            return;

          x0 = x1;
          y0 = y1;

          if (curve == curveStack)
            break;

          curve -= 3;
          continue;
        }

_CBezierSplit:
        {
          int a, b, c, d;

          curve[6].x = curve[3].x;
          c = curve[1].x;
          d = curve[2].x;
          curve[1].x = a = ( curve[0].x + c ) / 2;
          curve[5].x = b = ( curve[3].x + d ) / 2;
          c = ( c + d ) / 2;
          curve[2].x = a = ( a + c ) / 2;
          curve[4].x = b = ( b + c ) / 2;
          curve[3].x = ( a + b ) / 2;

          curve[6].y = curve[3].y;
          c = curve[1].y;
          d = curve[2].y;
          curve[1].y = a = ( curve[0].y + c ) / 2;
          curve[5].y = b = ( curve[3].y + d ) / 2;
          c = ( c + d ) / 2;
          curve[2].y = a = ( a + c ) / 2;
          curve[4].y = b = ( b + c ) / 2;
          curve[3].y = ( a + b ) / 2;
        }

        curve += 3;
        continue;
      }

      srcPts += 3;
      srcCmd += 3;

      if (srcCmd == srcEnd)
        goto _ClosePath;
    }

    // ------------------------------------------------------------------------
    // [MoveTo / Close]
    // ------------------------------------------------------------------------

    else
    {
      FOG_ASSERT(PathCmd::isMoveTo(c) || PathCmd::isClose(c));

_ClosePath:
      // Close the current polygon.
      if ((x0 != startX0) | (y0 != startY0) && !PathRasterizer8_renderLine<int>(self, x0, y0, startX0, startY0))
        return;

      if (srcCmd == srcEnd)
        return;
      
      srcPts++;
      srcCmd++;

      if (srcCmd == srcEnd)
        return;

      if (PathCmd::isMoveTo(c))
        goto _MoveTo;
      else
        goto _Start;
    }
  }
}

void PathRasterizer8::addPath(const PathF& path)
{
  FOG_ASSERT(_isFinalized == false);
  if (_error != ERR_OK) return;

  PathRasterizer8_addPathData<float>(this, path.getVertices(), path.getCommands(), path.getLength(), _offsetF);
}

void PathRasterizer8::addPath(const PathF& path, const PointF& _offset)
{
  FOG_ASSERT(_isFinalized == false);
  if (_error != ERR_OK) return;

  PointF offset = _offset + _offsetF;
  PathRasterizer8_addPathData<float>(this, path.getVertices(), path.getCommands(), path.getLength(), offset);
}

void PathRasterizer8::addPath(const PathD& path)
{
  FOG_ASSERT(_isFinalized == false);
  if (_error != ERR_OK) return;

  PathRasterizer8_addPathData<double>(this, path.getVertices(), path.getCommands(), path.getLength(), _offsetD);
}

void PathRasterizer8::addPath(const PathD& path, const PointD& _offset)
{
  FOG_ASSERT(_isFinalized == false);
  if (_error != ERR_OK) return;

  PointD offset = _offset + _offsetD;
  PathRasterizer8_addPathData<double>(this, path.getVertices(), path.getCommands(), path.getLength(), offset);
}

// ============================================================================
// [Fog::PathRasterizer8 - AddBox]
// ============================================================================

static const uint8_t Rasterizer_boxCommands[8] =
{
  PATH_CMD_MOVE_TO,
  PATH_CMD_LINE_TO,
  PATH_CMD_LINE_TO,
  PATH_CMD_LINE_TO,
  PATH_CMD_CLOSE,
  0xFF,
  0xFF,
  0xFF
};

void PathRasterizer8::addBox(const BoxF& box)
{
  if (_error != ERR_OK) return;
  FOG_ASSERT(_isFinalized == false);

  PointF pts[5];
  pts[0].set(box.x0, box.y0);
  pts[1].set(box.x1, box.y0);
  pts[2].set(box.x1, box.y1);
  pts[3].set(box.x0, box.y1);
  pts[4].set(0.0f, 0.0f);

  PathRasterizer8_addPathData<float>(this, pts, Rasterizer_boxCommands, 5, _offsetF);
}

void PathRasterizer8::addBox(const BoxD& box)
{
  if (_error != ERR_OK) return;
  FOG_ASSERT(_isFinalized == false);

  PointD pts[5];
  pts[0].set(box.x0, box.y0);
  pts[1].set(box.x1, box.y0);
  pts[2].set(box.x1, box.y1);
  pts[3].set(box.x0, box.y1);
  pts[4].set(0.0, 0.0);

  PathRasterizer8_addPathData<double>(this, pts, Rasterizer_boxCommands, 5, _offsetD);
}

// ============================================================================
// [Fog::PathRasterizer8 - ROW_NEW_CHUNK]
// ============================================================================

#define ROW_NEW_CHUNK(_Chunk_, _X_, _Length_) \
  FOG_MACRO_BEGIN \
    _Chunk_ = static_cast<PathRasterizer8::Chunk*>(self->_allocator.alloc(PathRasterizer8::Chunk::getSizeOf(_Length_))); \
    if (FOG_IS_NULL(_Chunk_)) goto _Bail; \
    \
    _Chunk_->x0 = (_X_); \
    _Chunk_->x1 = (_X_) + _Length_; \
  FOG_MACRO_END

// ============================================================================
// [Fog::PathRasterizer8 - ROW_ADD_ONE]
// ============================================================================

#define ROW_ADD_ONE(_Name_, _Row_, _X_, _Cover_, _Area_) \
  FOG_MACRO_BEGIN \
    PathRasterizer8::Chunk* _chunk = _Row_->first; \
    PathRasterizer8::Chunk* _cNew; \
    PathRasterizer8::Chunk* _cLast; \
    \
    /* ------------------------------------------------------------------- */ \
    /* [First]                                                             */ \
    /* ------------------------------------------------------------------- */ \
    \
    if (_chunk == NULL) \
    { \
      ROW_NEW_CHUNK(_cNew, _X_, 1); \
      _cNew->prev = _cNew; \
      _cNew->next = _cNew; \
      \
      _Row_->first = _cNew; \
      goto _Name_##_Add_One_Set; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Prepend]                                                           */ \
    /* ------------------------------------------------------------------- */ \
    \
    _cLast = _chunk->prev; \
    \
    if (_X_ < _chunk->x0) \
    { \
      ROW_NEW_CHUNK(_cNew, _X_, 1); \
      _cNew->prev = _cLast; \
      _cNew->next = _chunk; \
      \
      _cLast->next = _cNew; \
      _chunk->prev = _cNew; \
      \
      _Row_->first = _cNew; \
      goto _Name_##_Add_One_Set; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Insert / Append]                                                   */ \
    /* ------------------------------------------------------------------- */ \
    \
    if (_X_ < _cLast->x1) \
    { \
      if ((_X_ - _chunk->x0) <= (_cLast->x1 - _X_)) \
      { \
        for (;;) \
        { \
          if (_X_ < _chunk->x0) \
            break; \
          \
          if (_X_ < _chunk->x1) \
            goto _Name_##_Add_One_Merge; \
          \
          FOG_ASSERT(_chunk != _cLast); \
          _chunk = _chunk->next; \
        } \
      } \
      else \
      { \
        _chunk = _cLast; \
        for (;;) \
        { \
          if (_X_ >= _chunk->x0) \
          { \
_Name_##_Add_One_Merge: \
            LOG_CELL("Add_One", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
            _chunk->cells[_X_ - _chunk->x0].add(_Cover_, _Area_); \
            goto _Name_##_Add_One_End; \
          } \
          \
          _chunk = _chunk->prev; \
          FOG_ASSERT(_chunk != _cLast); \
          \
          if (_X_ >= _chunk->x1) \
            break; \
        } \
        _chunk = _chunk->next; \
      } \
      FOG_ASSERT(_X_ <= _chunk->x0); \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Create]                                                            */ \
    /* ------------------------------------------------------------------- */ \
    \
    _cLast = _chunk->prev; \
    \
    ROW_NEW_CHUNK(_cNew, _X_, 1); \
    _cNew->prev = _cLast; \
    _cNew->next = _chunk; \
    \
    _cLast->next = _cNew; \
    _chunk->prev = _cNew; \
    \
_Name_##_Add_One_Set: \
    LOG_CELL("Add_One", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
    _cNew->cells[0].set(_Cover_, _Area_); \
    \
_Name_##_Add_One_End: \
    ; \
  FOG_MACRO_END \

// ============================================================================
// [Fog::PathRasterizer8 - ROW_ADD_TWO]
// ============================================================================

#define ROW_ADD_TWO(_Name_, _Row_, _X_, _Cover_, _Area_, _Advance_) \
  FOG_MACRO_BEGIN \
    PathRasterizer8::Chunk* _chunk = _Row_->first; \
    PathRasterizer8::Chunk* _cNew; \
    PathRasterizer8::Chunk* _cLast; \
    \
    int _count; \
    \
    /* ------------------------------------------------------------------- */ \
    /* [First]                                                             */ \
    /* ------------------------------------------------------------------- */ \
    \
    if (_chunk == NULL) \
    { \
      ROW_NEW_CHUNK(_chunk, _X_, 2); \
      _chunk->prev = _chunk; \
      _chunk->next = _chunk; \
      \
      _Row_->first = _chunk; \
      \
      LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
      _chunk->cells[0].set(_Cover_, _Area_); \
      \
      _Advance_ \
      \
      LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
      _chunk->cells[1].set(_Cover_, _Area_); \
      \
      goto _Name_##_Add_Two_End; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Prepend]                                                           */ \
    /* ------------------------------------------------------------------- */ \
    \
    _cLast = _chunk->prev; \
    \
    if (_X_ < _chunk->x0) \
    { \
      _count = Math::min(_chunk->x0 - _X_, 2); \
      FOG_ASSERT(_count > 0); \
      \
      ROW_NEW_CHUNK(_cNew, _X_, _count); \
      _cNew->prev = _cLast; \
      _cNew->next = _chunk; \
      \
      _cLast->next = _cNew; \
      _chunk->prev = _cNew; \
      \
      _Row_->first = _cNew; \
      FOG_ASSERT(_cNew->x1 <= _chunk->x0); \
      \
      goto _Name_##_Add_Two_Set; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Insert / Append]                                                   */ \
    /* ------------------------------------------------------------------- */ \
    \
    if (_X_ < _cLast->x1) \
    { \
      if ((_X_ - _chunk->x0) <= (_cLast->x1 - _X_)) \
      { \
        for (;;) \
        { \
          if (_X_ < _chunk->x0) \
            break; \
          \
          if (_X_ < _chunk->x1) \
            goto _Name_##_Add_Two_Merge; \
          \
          FOG_ASSERT(_chunk != _cLast); \
          _chunk = _chunk->next; \
        } \
      } \
      else \
      { \
        _chunk = _cLast; \
        for (;;) \
        { \
          if (_X_ >= _chunk->x0) \
          { \
_Name_##_Add_Two_Merge: \
            LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
            _chunk->cells[_X_ - _chunk->x0].add(_Cover_, _Area_); \
            \
            _Advance_ \
            \
            if (_X_ < _chunk->x1) \
            { \
_Name_##_Add_Two_Merge_Second: \
              LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
              _chunk->cells[_X_ - _chunk->x0].add(_Cover_, _Area_); \
              goto _Name_##_Add_Two_End; \
            } \
            \
            _cLast = _chunk; \
            _chunk = _chunk->next; \
            \
            if (_X_ >= _chunk->x0 && _X_ < _chunk->x1) \
              goto _Name_##_Add_Two_Merge_Second; \
            \
            ROW_NEW_CHUNK(_cNew, _X_, 1); \
            _cNew->prev = _cLast; \
            _cNew->next = _chunk; \
            \
            _cLast->next = _cNew; \
            _chunk->prev = _cNew; \
            \
            LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
            _cNew->cells[0].set(_Cover_, _Area_); \
            goto _Name_##_Add_Two_End; \
          } \
          \
          _chunk = _chunk->prev; \
          FOG_ASSERT(_chunk != _cLast); \
          \
          if (_X_ >= _chunk->x1) \
            break; \
        } \
        _chunk = _chunk->next; \
      } \
      FOG_ASSERT(_X_ <= _chunk->x0); \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Create]                                                            */ \
    /* ------------------------------------------------------------------- */ \
    \
    _cLast = _chunk->prev; \
    _count = _chunk->x0 - _X_; \
    \
    if ((uint)_count >= 2) _count = 2; \
    \
    ROW_NEW_CHUNK(_cNew, _X_, _count); \
    _cNew->prev = _cLast; \
    _cNew->next = _chunk; \
    \
    _cLast->next = _cNew; \
    _chunk->prev = _cNew; \
    \
_Name_##_Add_Two_Set: \
    LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
    _cNew->cells[0].set(_Cover_, _Area_); \
    \
    _Advance_ \
    \
    if (_count > 1) \
    { \
      FOG_ASSERT(_cNew->x1 - _cNew->x0 == 2); \
      \
      LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
      _cNew->cells[1].set(_Cover_, _Area_); \
    } \
    else \
    { \
      FOG_ASSERT(_cNew->next == _chunk); \
      FOG_ASSERT(_cNew->x1 <= _chunk->x0); \
      \
      LOG_CELL("Add_Two", _X_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
      _chunk->cells[0].add(_Cover_, _Area_); \
    } \
    \
_Name_##_Add_Two_End: \
    ; \
  FOG_MACRO_END

// ============================================================================
// [Fog::PathRasterizer8 - ROW_ADD_N]
// ============================================================================

#define ROW_ADD_N(_Name_, _Row_, _X0_, _X1_, _Cover_, _Area_, _Advance_, _Last_) \
  FOG_MACRO_BEGIN \
    PathRasterizer8::Chunk* _cFirst = _Row_->first; \
    PathRasterizer8::Chunk* _cLast = NULL; \
    \
    PathRasterizer8::Chunk* _chunk = _cFirst; \
    PathRasterizer8::Cell* _cell; \
    \
    int _stop; \
    \
    /* ------------------------------------------------------------------- */ \
    /* [First]                                                             */ \
    /* ------------------------------------------------------------------- */ \
    \
    if (_chunk == NULL) \
    { \
      int _length = _X1_ - _X0_; \
      \
      if (FOG_UNLIKELY(_length > A8_MAX_CHUNK_LENGTH)) \
        _length = A8_MAX_CHUNK_LENGTH; \
      \
      ROW_NEW_CHUNK(_chunk, _X0_, _length); \
      _chunk->prev = _chunk; \
      _chunk->next = _chunk; \
      \
      _cFirst = _chunk; \
      _cLast = _chunk; \
      \
      _Row_->first = _chunk; \
      \
      _cell = _chunk->cells; \
      _stop = _X0_ + _length; \
      \
      goto _Name_##_Add_N_Fill; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Prepend]                                                           */ \
    /* ------------------------------------------------------------------- */ \
    \
    _cLast = _chunk->prev; \
    \
    if (_X0_ < _chunk->x0) \
    { \
      int _length = Math::min(_chunk->x0, _X1_) - _X0_; \
      \
      if (FOG_UNLIKELY(_length > A8_MAX_CHUNK_LENGTH)) \
        _length = A8_MAX_CHUNK_LENGTH; \
      \
      PathRasterizer8::Chunk* _cNext = _chunk; \
      ROW_NEW_CHUNK(_chunk, _X0_, _length); \
      \
      _chunk->prev = _cLast; \
      _chunk->next = _cNext; \
      \
      _cLast->next = _chunk; \
      _cNext->prev = _chunk; \
      \
      _cFirst = _chunk; \
      _Row_->first = _chunk; \
      \
      _cell = _chunk->cells; \
      _stop = _X0_ + _length; \
      \
      goto _Name_##_Add_N_Fill; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Append]                                                            */ \
    /* ------------------------------------------------------------------- */ \
    \
    if (_X0_ >= _cLast->x1) \
    { \
      _chunk = _cLast; \
      \
      goto _Name_##_Add_N_Append; \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Insert]                                                            */ \
    /* ------------------------------------------------------------------- */ \
    \
    if ((_X0_ - _chunk->x0) <= (_cLast->x1 - _X0_)) \
    { \
      for (;;) \
      { \
        if (_X0_ < _chunk->x1) \
          goto _Name_##_Add_N_Merge; \
        \
        _chunk = _chunk->next; \
        \
        /* The list is circular, but we know that the [x0, x1] intersects  */ \
        /* with the bounding-box of the chunk-list. This means that we     */ \
        /* can't skip into the first chunk in this loop.                   */ \
        FOG_ASSERT(_chunk != _cFirst); \
      } \
    } \
    else \
    { \
      _chunk = _cLast; \
      \
      for (;;) \
      { \
        if (_X0_ >= _chunk->x0) \
        { \
          if (_X0_ >= _chunk->x1) \
            _chunk = _chunk->next; \
          \
          goto _Name_##_Add_N_Merge; \
        } \
        \
        /* Again, we can't iterate forever. */ \
        FOG_ASSERT(_chunk != _cFirst); \
        _chunk = _chunk->prev; \
      } \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Advance]                                                           */ \
    /* ------------------------------------------------------------------- */ \
    \
_Name_##_Add_N_Advance: \
    if (_chunk == _cLast) \
    { \
_Name_##_Add_N_Append: \
      int _length = _X1_ - _X0_; \
      \
      if (FOG_UNLIKELY(_length > A8_MAX_CHUNK_LENGTH)) \
        _length = A8_MAX_CHUNK_LENGTH; \
      \
      PathRasterizer8::Chunk* _cPrev = _chunk; \
      \
      ROW_NEW_CHUNK(_chunk, _X0_, _length); \
      _chunk->prev = _cPrev; \
      _chunk->next = _cFirst; \
      \
      _cFirst->prev = _chunk; \
      _cPrev->next = _chunk; \
      \
      _cLast = _chunk; \
      _cell = _chunk->cells; \
      \
      _stop = _X0_ + _length; \
    } \
    else \
    { \
      _chunk = _chunk->next; \
      \
_Name_##_Add_N_Merge: \
      if (_X0_ < _chunk->x0) \
      { \
        int _length = Math::min(_X1_, _chunk->x0) - _X0_; \
        \
        if (FOG_UNLIKELY(_length > A8_MAX_CHUNK_LENGTH)) \
          _length = A8_MAX_CHUNK_LENGTH; \
        \
        PathRasterizer8::Chunk* _cPrev = _chunk->prev; \
        PathRasterizer8::Chunk* _cNext = _chunk; \
        \
        ROW_NEW_CHUNK(_chunk, _X0_, _length); \
        _chunk->prev = _cPrev; \
        _chunk->next = _cNext; \
        \
        _cPrev->next = _chunk; \
        _cNext->prev = _chunk; \
        \
        _cell = _chunk->cells; \
        _stop = _X0_ + _length; \
      } \
      else \
      { \
        FOG_ASSERT(_X0_ < _chunk->x1); \
        \
        _cell = &_chunk->cells[_X0_ - _chunk->x0]; \
        _stop = Math::min(_chunk->x1, _X1_); \
        \
        _cellOp |= CELL_OP_MERGE; \
      } \
    } \
    \
    /* ------------------------------------------------------------------- */ \
    /* [Fill]                                                              */ \
    /* ------------------------------------------------------------------- */ \
    \
_Name_##_Add_N_Fill: \
    { \
      FOG_ASSERT(_X0_ <  _stop); \
      FOG_ASSERT(_X1_ >= _stop); \
      \
      bool _isLast = (_stop == _X1_); \
      _stop -= _isLast; \
      \
      switch (_cellOp) \
      { \
        case CELL_OP_COPY | CELL_OP_POSITIVE: \
          if (_X0_ != _stop) \
          { \
            for (;;) \
            { \
              LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
              _cell->set(_Cover_, _Area_); \
              _cell++; \
              \
              _Advance_ \
              \
              if (++_X0_ == _stop) \
                break; \
            } \
            \
            if (!_isLast) \
              goto _Name_##_Add_N_Advance; \
          } \
          \
          _Last_ \
          \
          LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
          _cell->set(_Cover_, _Area_); \
          break; \
        \
        case CELL_OP_COPY | CELL_OP_NEGATIVE: \
          if (_X0_ != _stop) \
          { \
            for (;;) \
            { \
              LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), -(_Cover_), -(_Area_)); \
              _cell->set(-(_Cover_), -(_Area_)); \
              _cell++; \
              \
              _Advance_ \
              \
              if (++_X0_ == _stop) \
                break; \
            } \
            \
            if (!_isLast) \
              goto _Name_##_Add_N_Advance; \
          } \
          \
          _Last_ \
          \
          LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), -(_Cover_), -(_Area_)); \
          _cell->set(-(_Cover_), -(_Area_)); \
          break; \
        \
        case CELL_OP_MERGE | CELL_OP_POSITIVE: \
          _cellOp &= ~CELL_OP_MERGE; \
          \
          if (_X0_ != _stop) \
          { \
            for (;;) \
            { \
              LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
              _cell->add(_Cover_, _Area_); \
              _cell++; \
              \
              _Advance_ \
              \
              if (++_X0_ == _stop) \
                break; \
            } \
            \
            if (!_isLast) \
              goto _Name_##_Add_N_Advance; \
          } \
          \
          _Last_ \
          \
          LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
          _cell->add(_Cover_, _Area_); \
          break; \
        \
        case CELL_OP_MERGE | CELL_OP_NEGATIVE: \
          _cellOp &= ~CELL_OP_MERGE; \
          \
          if (_X0_ != _stop) \
          { \
            for (;;) \
            { \
              LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
              _cell->sub(_Cover_, _Area_); \
              _cell++; \
              \
              _Advance_ \
              \
              if (++_X0_ == _stop) \
                break; \
            } \
            \
            if (!_isLast) \
              goto _Name_##_Add_N_Advance; \
          } \
          \
          _Last_ \
          \
          LOG_CELL("Add_N", _X0_, (int)(_Row_ - self->_rowsAdjusted), _Cover_, _Area_); \
          _cell->sub(_Cover_, _Area_); \
          break; \
        \
        default: \
          FOG_ASSERT_NOT_REACHED(); \
      } \
    } \
  FOG_MACRO_END

template<typename FixedT>
static bool PathRasterizer8_renderLine(PathRasterizer8* self, FixedT x0, FixedT y0, FixedT x1, FixedT y1)
{
  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  FixedT dx = x1 - x0;
  FixedT dy = y1 - y0;
  
  // The rasterizer does nothing in such case.
  if (dy == FixedT(0))
    return true;

  int cover = int(dy);
  int area;

  if (dx < 0)
    dx = -dx;

  if (dy < 0)
    dy = -dy;

  // Instead of subdividing a line to fit into A8_I32_COORD_LIMIT, we use 64-bit
  // integer version of renderLine() to do the job. This situation can happen
  // only when drawing to a screen where one or both dimensions is larger than
  // 16384 pixels.
  if (sizeof(FixedT) < sizeof(int64_t) && (dx >= FixedT(A8_I32_COORD_LIMIT) || dy >= FixedT(A8_I32_COORD_LIMIT)))
    return PathRasterizer8_renderLine<int64_t>(self, int64_t(x0), int64_t(y0), int64_t(x1), int64_t(y1));

  int rInc = 1;
  int coverSign = 1;

  // The new rasterizer algorithm is based on traversing the line from the left
  // to the right. The start and end coordinates can be swapped, but it's needed
  // to change the coverSign in such case.
  if (x0 > x1)
  {
    swap(x0, x1);
    swap(y0, y1);

    coverSign = -1;
  }

  // If we are going from bottom to top then we need to invert the fractional
  // parts of y0/y1. In case that the inversion overflow it's needed to subtract
  // A8_SCALE*2 to normalize the position into the opposite pixel.
  if (y0 > y1)
  {
    static const int norm[2] = { 1, 1 - A8_SCALE * 2};

    y0 ^= A8_MASK;
    y0 += norm[(int(y0) & A8_MASK) == A8_MASK];
    y1  = y0 + dy;

    rInc = -1;
    coverSign = -coverSign;
  }

  // Extract the raster and fractional coordinates.
  int ex0 = int(x0 >> A8_SHIFT);
  int fx0 = int(x0) & A8_MASK;

  int ey0 = int(y0 >> A8_SHIFT);
  int fy0 = int(y0) & A8_MASK;

  int ex1 = int(x1 >> A8_SHIFT);
  int fy1 = int(y1) & A8_MASK;

  // How many Y iterations to do.
  int i = 1;
  // How many Y iterations to do next.
  int j = int(y1 >> A8_SHIFT) - ey0;

  // NOTE: Variable 'i' is just loop counter. Because we need to make sure to
  // handle the start and end points of the line (start point is always based
  // on the y0 coordinate and end point is always based on the y1 coordinate),
  // we need two variables:
  //
  //  - 'i' - Used for current loop.
  //  - 'j' - Used for next loop.

  // Prepare the current and end row pointers.
  PathRasterizer8::Row* rPtr = self->_rowsAdjusted;
  PathRasterizer8::Row* rEnd = self->_rowsAdjusted;

  // --------------------------------------------------------------------------
  // [Bounding-Box]
  // --------------------------------------------------------------------------

  {
    int by0 = ey0;
    int by1 = ey0 + (j - (fy1 == 0)) * rInc;

    if (by0 > by1)
      swap(by0, by1);
    
    if (FOG_UNLIKELY(self->_boundingBox.y0 == -1))
    {
      // Initialize the bounding-box.
      self->_boundingBox.x0 = ex0;
      self->_boundingBox.y0 = by0;
      self->_boundingBox.x1 = ex1;
      self->_boundingBox.y1 = by1;

      // Initialize the rows.
      for (;;)
      {
        rPtr[by0].first = NULL;
        if (by0 == by1)
          break;
        by0++;
      }
    }
    else
    {
      int bEnd;

      // Merge the bounding-box with the line.
      if (ex0 < self->_boundingBox.x0) self->_boundingBox.x0 = ex0;
      if (ex1 > self->_boundingBox.x1) self->_boundingBox.x1 = ex1;

      // Clear the rows (clear).
      bEnd = self->_boundingBox.y0;
      if (by0 < bEnd)
      {
        self->_boundingBox.y0 = by0;
        do {
          rPtr[by0++].first = NULL;
        } while (by0 != bEnd);
      }

      bEnd = self->_boundingBox.y1;
      if (by1 > bEnd)
      {
        self->_boundingBox.y1 = by1;
        do {
          rPtr[++bEnd].first = NULL;
        } while (by1 != bEnd);
      }
    }
  }

  rPtr += ey0;
  rEnd += ey0 + (j + (fy1 != 0)) * rInc;

  // --------------------------------------------------------------------------
  // [Point]
  // --------------------------------------------------------------------------

  // If the line is so small that it covers only one cell in one scanline, we
  // can use this fast-path to return as early as possible from renderLine().
  //
  // This can happen when approximating very small circular arcs (for example
  // small radius when painting rounded rectangle) or when painting an object
  // which is heavily down-scaled.
  if ((j | ((fx0 + int(dx)) > 256)) == 0)
  {
    fx0 = fx0 * 2 + int(dx);
    goto _One;
  }

  // --------------------------------------------------------------------------
  // [Vertical Only]
  // --------------------------------------------------------------------------

  if (dx == 0)
  {
    // We have to calculate start and end cells, and then - the common values
    // of the area and coverage for all cells of the line. We know exactly 
    // there's only one cell per scanline, thus, we don't have to use
    // complicated scanline rasterization.
    if (j > 0)
      cover = (A8_SCALE - fy0) * coverSign;

    fx0 *= 2;
    fy0 = A8_SCALE;

    // Instead of making two versions of this function (one for positive covers
    // and one for negative covers) we just negate the important variables if
    // needed.
    if (coverSign < 0)
    {
      fy0 = -fy0;
      fy1 = -fy1;
    }

    for (;;)
    {
_One:
      area = fx0 * cover;
      do {
        ROW_ADD_ONE(_Vert_Only, rPtr, ex0, cover, area);
        rPtr += rInc;
      } while (--i);

      if (rPtr == rEnd)
        break;

      i = j;
      j = 1;
      cover = fy1;

      if (i > 1)
      {
        cover = fy0;
        i--;
      }
    }
    return true;
  }

  // --------------------------------------------------------------------------
  // [Vertical / Horizontal Orientation]
  // --------------------------------------------------------------------------

  else
  {
    // Only one or two cells are generated per scanline.
    FixedT xErr = -dy / 2, xBase, xLift, xRem, xDlt = dx;
    FixedT yErr = -dx / 2, yBase, yLift, yRem, yDlt = dy;

    xBase = dx * A8_SCALE;
    xLift = xBase / dy;
    xRem  = xBase % dy;

    yBase = dy * A8_SCALE;
    yLift = yBase / dx;
    yRem  = yBase % dx;

    if (j != 0)
    {
      FixedT p = FixedT(A8_SCALE - fy0) * dx;

      xDlt  = p / dy;
      xErr += p % dy;

      fy1 = A8_SCALE;
    }

    if (ex0 != ex1)
    {
      FixedT p = FixedT(A8_SCALE - fx0) * dy;

      yDlt = p / dx;
      yErr += p % dx;
    }

    // ------------------------------------------------------------------------
    // [Vertical Orientation]
    // ------------------------------------------------------------------------

    if (dy >= dx)
    {
      int yAcc = int(y0) + int(yDlt);

      if (coverSign > 0)
      {
        goto _Vert_P_Skip;

        for (;;)
        {
          do {
            xDlt = xLift;
            xErr += xRem;
            if (xErr >= 0) { xErr -= dy; xDlt++; }

_Vert_P_Skip:
            area = fx0;
            fx0 += int(xDlt);

            if (fx0 <= 256)
            {
              cover = fy1 - fy0; // Positive.
              area += fx0;

_Vert_P_Single:
              area *= cover;
              ROW_ADD_ONE(_Vert_P, rPtr, ex0, cover, area);

              if (fx0 == 256)
              {
                ex0++;
                fx0 = 0;
                goto _Vert_P_Advance;
              }
            }
            else
            {
              yAcc &= 0xFF;
              FOG_ASSERT(yAcc >= fy0 && yAcc <= fy1);
              
              cover = yAcc - fy0; // Positive.
              fx0  &= A8_MASK;

              // Improve the count of generated cells in case that the resulting
              // cover is zero using the 'ROW_ADD_ONE'. The 'goto' ensures that
              // the ROW_ADD_ONE() macro will be expanded only once.
              if (cover == 0)
              {
                cover = fy1 - yAcc; // Positive.
                area  = fx0;
                ex0++;
                goto _Vert_P_Single;
              }
              else
              {
                area = (area + A8_SCALE) * cover;
                ROW_ADD_TWO(_Vert_P, rPtr, ex0, cover, area,
                {
                  cover = fy1 - yAcc; // Positive.
                  area  = fx0 * cover;
                  ex0++;
                });
              }

_Vert_P_Advance:
              yAcc += int(yLift);
              yErr += yRem;
              if (yErr >= 0) { yErr -= dx; yAcc++; }
            }

            rPtr += rInc;
          } while (--i);

          if (rPtr == rEnd)
            break;

          i = j;
          j = 1;

          if (i > 1)
          {
            fy0 = 0;
            fy1 = A8_SCALE;
            i--;
          }
          else
          {
            fy0 = 0;
            fy1 = int(y1) & A8_MASK;

            xDlt = x1 - (ex0 << 8) - fx0;
            goto _Vert_P_Skip;
          }
        }
      }
      else
      {
        goto _Vert_N_Skip;

        for (;;)
        {
          do {
            xDlt = xLift;
            xErr += xRem;
            if (xErr >= 0) { xErr -= dy; xDlt++; }

_Vert_N_Skip:
            area = fx0;
            fx0 += int(xDlt);

            if (fx0 <= 256)
            {
              cover = fy0 - fy1; // Negative.
              area += fx0;

_Vert_N_Single:
              area *= cover;
              ROW_ADD_ONE(_Vert_N, rPtr, ex0, cover, area);

              if (fx0 == 256)
              {
                ex0++;
                fx0 = 0;
                goto _Vert_N_Advance;
              }
            }
            else
            {
              yAcc &= 0xFF;
              FOG_ASSERT(yAcc >= fy0 && yAcc <= fy1);

              cover = fy0 - yAcc; // Negative.
              fx0  &= A8_MASK;

              // Improve the count of generated cells in case that the resulting
              // cover is zero using the 'ROW_ADD_ONE'. The 'goto' ensures that
              // the ROW_ADD_ONE() macro will be expanded only once.
              if (cover == 0)
              {
                cover = yAcc - fy1; // Negative.
                area  = fx0;
                ex0++;
                goto _Vert_N_Single;
              }
              else
              {
                area = (area + A8_SCALE) * cover;
                ROW_ADD_TWO(_Vert_N, rPtr, ex0, cover, area,
                {
                  cover = yAcc - fy1; // Negative.
                  area  = fx0 * cover;
                  ex0++;
                });
              }

_Vert_N_Advance:
              yAcc += int(yLift);
              yErr += yRem;
              if (yErr >= 0) { yErr -= dx; yAcc++; }
            }

            rPtr += rInc;
          } while (--i);

          if (rPtr == rEnd)
            break;

          i = j;
          j = 1;

          if (i > 1)
          {
            fy0 = 0;
            fy1 = A8_SCALE;
            i--;
          }
          else
          {
            fy0 = 0;
            fy1 = int(y1) & A8_MASK;
            xDlt = x1 - (ex0 << 8) - fx0;
            goto _Vert_N_Skip;
          }
        }
      }

      return true;
    }

    // ------------------------------------------------------------------------
    // [Horizontal Orientation]
    // ------------------------------------------------------------------------

    else
    {
      // Cell operator, directly used/modified by ROW_ADD_N() macro.
      int _cellOp = coverSign > 0 ? CELL_OP_POSITIVE : CELL_OP_NEGATIVE;

      int fx1;
      int coverAcc = fy0;

      cover = int(yDlt);
      coverAcc += cover;

      if (j != 0)
        fy1 = A8_SCALE;

      if (fx0 + int(xDlt) > 256)
        goto _Horz_Inside;

      x0 += xDlt;

      cover = (fy1 - fy0) * coverSign;
      area = (fx0 * 2 + int(xDlt)) * cover;

_Horz_Single:
      ROW_ADD_ONE(_Horz, rPtr, ex0, cover, area);
      
      rPtr += rInc;
      if (rPtr == rEnd)
        return true;
      
      if (fx0 + int(xDlt) == 256)
      {
        coverAcc += int(yLift);
        yErr += yRem;
        if (yErr >= 0) { yErr -= dx; coverAcc++; }
      }

      if (--i == 0)
        goto _Horz_After;

      for (;;)
      {
        do {
          xDlt = xLift;
          xErr += xRem;
          if (xErr >= 0) { xErr -= dy; xDlt++; }

          ex0 = int(x0 >> A8_SHIFT);
          fx0 = int(x0) & A8_MASK;

_Horz_Skip:
          coverAcc -= 256;
          cover = coverAcc;
          FOG_ASSERT(cover >= 0 && cover <= 256);

_Horz_Inside:
          x0 += xDlt;

          ex1 = int(x0 >> A8_SHIFT);
          fx1 = int(x0) & A8_MASK;

          FOG_ASSERT(ex0 != ex1);

          if (fx1 == 0)
            fx1 = A8_SCALE;
          else
            ex1++;

          // Calculate the first cover/area pair. All cells inside the line
          // will be calculated inside ROW_ADD_N() loop.
          area = (fx0 + A8_SCALE) * cover;

          ROW_ADD_N(_Horz, rPtr, ex0, ex1, cover, area,
          { /* Advance: */
            cover = int(yLift);
            yErr += yRem;
            if (yErr >= 0) { yErr -= dx; cover++; }

            coverAcc += cover;
            area = A8_SCALE * cover;
          },
          { /* Last: */
            cover += fy1 - coverAcc;
            area = fx1 * cover;

            if (fx1 == A8_SCALE)
            {
              coverAcc += int(yLift);
              yErr += yRem;
              if (yErr >= 0) { yErr -= dx; coverAcc++; }
            }
          });

          rPtr += rInc;
        } while (--i);

        if (rPtr == rEnd)
          break;

_Horz_After:
        i = j;
        j = 1;

        if (i > 1)
        {
          fy1 = A8_SCALE;
          i--;
        }
        else
        {
          fy1 = int(y1) & A8_MASK;
          xDlt = x1 - x0;

          ex0 = int(x0 >> A8_SHIFT);
          fx0 = int(x0) & A8_MASK;

          if (fx0 + int(xDlt) <= 256)
          {
            cover = fy1 * coverSign;
            area = (fx0 * 2 + int(xDlt)) * cover;
            goto _Horz_Single;
          }
          else
          {
            goto _Horz_Skip;
          }
        }
      }

      return true;
    }
  }

_Bail:
  return false;
}

// ============================================================================
// [Fog::PathRasterizer8 - Render - Helpers]
// ============================================================================

template<int _RULE, int _USE_ALPHA>
static FOG_INLINE uint32_t PathRasterizer8_calculateAlpha(const PathRasterizer8* self, int cover)
{
  if (cover < 0) cover = -cover;

  if (_RULE == FILL_RULE_NON_ZERO)
  {
    if (cover > A8_SCALE)
      cover = A8_SCALE;
  }
  else
  {
    cover &= A8_MASK_2;
    if (cover > A8_SCALE)
      cover = A8_SCALE_2 - cover;
  }

  if (_USE_ALPHA)
    cover = (cover * self->_opacity) >> 8;

  return cover;
}

// ============================================================================
// [Fog::PathRasterizer8 - Render - Clip-Box]
// ============================================================================

template<int _RULE, int _USE_ALPHA>
static void FOG_CDECL PathRasterizer8_render_st_clip_box(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  PathRasterizer8* self = static_cast<PathRasterizer8*>(_self);
  FOG_ASSERT(self->_isFinalized);

  int y0 = self->_boundingBox.y0;
  int y1 = self->_boundingBox.y1;
  int xEnd = self->_sceneBox.x1;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  if (FOG_IS_ERROR(scanline->prepare(self->_boundingBox.getWidth() * 2)))
    return;

  PathRasterizer8::Row* rows = self->_rowsAdjusted + y0;
  filler->prepare(y0);
  
  // --------------------------------------------------------------------------
  // [Process]
  // --------------------------------------------------------------------------

  for (;;)
  {
    // ------------------------------------------------------------------------
    // [Merge / Skip]
    // ------------------------------------------------------------------------

    PathRasterizer8::Chunk* first = rows->first;
    rows++;

    if (FOG_IS_NULL(first))
    {
      int mark = y0;
      do {
        if (++y0 >= y1) return;

        first = rows->first;
        rows++;
      } while (first == NULL);

      filler->skip(y0 - mark);
    }

    VERIFY_CHUNKS_8(first);

    // ------------------------------------------------------------------------
    // [Fetch]
    // ------------------------------------------------------------------------

    RasterSpan8* span = scanline->begin();
    uint8_t* mask = scanline->getMask();

    PathRasterizer8::Chunk* chunk = first;
    PathRasterizer8::Cell* cell = chunk->cells;

    uint i = static_cast<uint>(chunk->getLength());

    int x;
    int xNext = chunk->x0;
    int cover = 0;
    uint32_t alpha;

    for (;;)
    {
      if (xNext == xEnd)
        break;

      x = xNext;

      // Link to the existing span if possible. In such case we don't need to
      // worry about span allocation, because we have preallocated two spans,
      // one for v-mask and one for possible c-mask.
      if (span->getX1() != x || span->getType() != RASTER_SPAN_AX_EXTRA)
      {
        NEW_SPANS(span, 2, return);
        span->setX0AndType(x, RASTER_SPAN_AX_EXTRA);
        span->setVariantMask(mask);
      }

_Continue:
      x += i;
      if (x > xEnd && --i == 0)
      {
        FOG_ASSERT(x - 1 == xEnd);
        break;
      }

      // Terminate span for now.
      span->setX1(x);

      do {
        cover += cell->cover;
        alpha  = PathRasterizer8_calculateAlpha<_RULE, _USE_ALPHA>(self, cover - (cell->area >> A8_SHIFT_2));

        Acc::p32Store2a(mask, alpha);
        mask += 2;

        cell++;
      } while (--i);

      chunk = chunk->next;
      if (chunk == first)
        break;

      cell = chunk->cells;
      xNext = chunk->x0;
      i = (uint)chunk->getLength();

      if (x == xNext)
        goto _Continue;

      alpha = PathRasterizer8_calculateAlpha<_RULE, _USE_ALPHA>(self, cover);
      if (alpha != 0)
      {
        uint len = static_cast<uint>(xNext - x);

        if (len <= RASTER_SPAN_C_THRESHOLD)
        {
          // If CPU supports unaligned read/write then we can optimize filling
          // of small spans by extending the alpha to 32-bit or 64-bit integer
          // and storing it directly to the current array. Span allocator always
          // reserves some bytes for these optimizations.
          //
          // This optimization increased performance of FogBench for about 1-2%,
          // depending on rasterized shape and size. It's very cheap, and gives
          // good results.
          FOG_ASSERT(RASTER_SPAN_C_THRESHOLD <= 4);

#if FOG_ARCH_BITS >= 64 && defined(FOG_ARCH_UNALIGNED_ACCESS_64)
          Acc::p64Store8u(mask,
            (uint64_t)alpha * FOG_UINT64_C(0x0001000100010001));
          mask += len * 2;
#elif FOG_ARCH_BITS >= 32 && defined(FOG_ARCH_UNALIGNED_ACCESS_32)
          alpha *= 0x00010001;
          Acc::p32Store4u(mask + 0, alpha);
          Acc::p32Store4u(mask + 4, alpha);
          mask += len * 2;
#else
          // Unoptimized case.
          do {
            Acc::p32Store2a(mask, alpha);
            mask += 2;
          } while (--len);
#endif
          span->setX1(xNext);
        }
        else
        {
          span->setNext(span + 1);
          span++;
          
          span->setPositionAndType(x, xNext, RASTER_SPAN_C);
          span->setConstMask(alpha);
        }
      }
    }

    span = scanline->end(span);

    // ------------------------------------------------------------------------
    // [Fill / Skip]
    // ------------------------------------------------------------------------

    if (FOG_IS_NULL(span))
    {
      filler->skip(1);
    }
    else
    {
#if defined(FOG_DEBUG_RASTERIZER)
      Rasterizer_dumpSpans(y0, scanline->getSpans());
#endif // FOG_DEBUG_RASTERIZER
      filler->process(span);
    }

    if (++y0 >= y1)
      return;
  }
}

// ============================================================================
// [Fog::PathRasterizer8 - Render - Clip-Region]
// ============================================================================

template<int _RULE, int _USE_ALPHA>
static void FOG_CDECL PathRasterizer8_render_st_clip_region(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  PathRasterizer8* self = static_cast<PathRasterizer8*>(_self);
  FOG_ASSERT(self->_isFinalized);

  int xMin = self->_boundingBox.x0;
  int xEnd = self->_boundingBox.x1;

  int yMin = self->_boundingBox.y0;
  int yEnd = self->_boundingBox.y1;

  int y0 = yMin;
  int y1;
  int yPos = y0;

  const BoxI* cPtr = self->_clip.region.data;
  const BoxI* cEnd = cPtr + self->_clip.region.length;

  const BoxI* cBandStart;
  const BoxI* cBandEnd;

  // --------------------------------------------------------------------------
  // [Prepare]
  // --------------------------------------------------------------------------

  if (FOG_IS_ERROR(scanline->prepare(self->_boundingBox.getWidth() * 2)))
    return;

  PathRasterizer8::Row* rows = self->_rowsAdjusted;
  filler->prepare(yPos);

  // --------------------------------------------------------------------------
  // [Process]
  // --------------------------------------------------------------------------
  
_NextBand:
  if (cPtr == cEnd)
    return;

  for (;;)
  {
    PathRasterizer8::Chunk* first;

    // ------------------------------------------------------------------------
    // [Advance Clip-Box]
    // ------------------------------------------------------------------------

    // Skip boxes which do not intersect in vertical direction.
    while (cPtr->y1 <= y0)
    {
      if (++cPtr == cEnd)
        return;
    }

    // Skip boxes which do not intersect in horizontal direction.
    while (cPtr->x1 <= xMin)
    {
      if (++cPtr == cEnd)
        return;
    }

    if (cPtr->x0 >= xEnd)
    {
      do {
        if (++cPtr == cEnd)
          return;
      } while (cPtr->x0 >= xEnd);
      continue;
    }

    y0 = cPtr->y0;
    if (y0 >= yEnd)
      return;

    // Save start of the current band and find the end. We limit the end to the
    // last band which can be processed. If some clip rectangle is completely
    // outside of the shape bounding box, then the band won't be included in
    // cBandStart...cBandEnd list.
    //
    // NOTE: Like all other pointers, cBandEnd contains the first band which is
    // invalid.
    cBandStart = cPtr;
    cBandEnd = cPtr;

    while (++cBandEnd != cEnd && cBandEnd->y0 == y0 && cBandEnd->x0 < xEnd)
      continue;

    if (y0 < yMin)
      y0 = yMin;

    y1 = cPtr->y1;
    if (y1 > yEnd)
      y1 = yEnd;

    FOG_ASSERT(y0 != y1);
    goto _LoopRow;

    // ------------------------------------------------------------------------
    // [Merge / Skip]
    // ------------------------------------------------------------------------

_NextRow:
    if (++y0 == y1)
    {
      cPtr = cBandEnd;
      goto _NextBand;
    }
    cPtr = cBandStart;

_LoopRow:
    for (;;)
    {
      first = rows[y0].first;
      if (first != NULL)
        break;

      if (++y0 == y1)
      {
        cPtr = cBandEnd;
        goto _NextBand;
      }
    }

    VERIFY_CHUNKS_8(first);

    // ------------------------------------------------------------------------
    // [Fetch]
    // ------------------------------------------------------------------------

    RasterSpan8* span = scanline->begin();
    uint8_t* mask = scanline->getMask();

    PathRasterizer8::Chunk* chunk = first;
    PathRasterizer8::Cell* cell = chunk->cells;

    int x0 = chunk->x0;
    int x1 = chunk->x1;
  
    int cover = 0;
    uint32_t alpha = 0;

    int xc;
    int xLocalEnd;

    // Skip clip rectangles which do not intersect with the current chunk.
    while (x0 >= cPtr->x1)
    {
_AdvanceClip:
      if (++cPtr == cBandEnd)
        goto _Fill;
    }

    // Skip cells which do not intersect with the current clip rectangle.
    xc = cPtr->x0;
    if (x0 < xc)
    {
_SkipCells:
      xLocalEnd = Math::min<int>(xc, x1);
      FOG_ASSERT(xLocalEnd != 0);

      do {
        cover += cell->cover;
        cell++;
      } while (++x0 != xLocalEnd);

      if (x0 == x1)
      {
        chunk = chunk->next;
        if (chunk == first)
          goto _Fill;

        cell = chunk->cells;
        x0 = chunk->x0;
        x1 = chunk->x1;

        if (x0 < xc)
          goto _SkipCells;
      }
      alpha = PathRasterizer8_calculateAlpha<_RULE, _USE_ALPHA>(self, cover);
    }
    FOG_ASSERT(x0 >= xc);

    if (alpha == 0)
    {
      // Skip clip rectangles which do not intersect with the current chunk.
      if (x0 >= cPtr->x1)
        goto _AdvanceClip;

      if (xc < x0)
        xc = x0;
    }
    else
    {
      // Skip part of rectangle which do not intersect with the current chunk,
      // filling it with const mask.
      if (xc < x0)
      {
        for (;;)
        {
_ConstSpan:
          NEW_SPAN(span, return);
          span->setX0AndType(xc, RASTER_SPAN_C);

          xc = Math::min<int>(x0, cPtr->x1);
          FOG_ASSERT(span->getX0() < xc);

          span->setX1(xc);
          span->setConstMask(alpha);

          if (xc == cPtr->x1)
          {
            if (++cPtr == cBandEnd)
              goto _Fill;
            xc = cPtr->x0;
            if (xc < x0)
              continue;
          }
          break;
        }
      }
    }

    if (x0 < xc)
      goto _SkipCells;

    FOG_ASSERT(x0 == xc);
_NewSpan:
    x0 = Math::min<int>(x1, cPtr->x1);

_NewSpan1:
    NEW_SPAN(span, return);
    span->setX0AndType(xc, RASTER_SPAN_AX_EXTRA); 
    span->setVariantMask(mask);

_ContinueSpan:
    do {
      cover += cell->cover;
      alpha = PathRasterizer8_calculateAlpha<_RULE, _USE_ALPHA>(self, cover - (cell->area >> A8_SHIFT_2));
      
      Acc::p16Store2a(mask, static_cast<uint16_t>(alpha));
      mask += 2;

      cell++;
    } while (++xc != x0);
    span->setX1(xc);

    if (xc == x1)
    {
      chunk = chunk->next;
      if (chunk == first)
        goto _Fill;

      cell = chunk->cells;
      x0 = chunk->x0;
      x1 = chunk->x1;

      if (xc == cPtr->x1)
      {
        // If alpha is zero, we can jump into _AdvanceClip loop, which efficiently
        // discards all clipped rectangles. Otherwise we need to create const-mask
        // spans, because of non-zero alpha.
        alpha = PathRasterizer8_calculateAlpha<_RULE, _USE_ALPHA>(self, cover);
        if (alpha == 0)
          goto _AdvanceClip;

_AdvanceClipAlphaNonZero:
        if (++cPtr == cBandEnd)
          goto _Fill;

        xc = cPtr->x0;
        if (xc < x0)
          goto _ConstSpan;

        if (xc > x0)
          goto _SkipCells;
      }

      if (xc == x0)
      {
        x0 = Math::min<int>(x1, cPtr->x1);
        if (span->getX1() != x0 || span->getType() != RASTER_SPAN_AX_EXTRA)
          goto _NewSpan1;
        else
          goto _ContinueSpan;
      }
      else
      {
        uint len = Math::min<int>(x0, cPtr->x1) - xc;
        alpha = PathRasterizer8_calculateAlpha<_RULE, _USE_ALPHA>(self, cover);

        if (alpha == 0)
        {
          xc += len;
          if (xc == cPtr->x1)
            goto _AdvanceClip;
          else
            goto _NewSpan;
        }
  
        if (len <= RASTER_SPAN_C_THRESHOLD)
        {
          // The same optimization as in render_st_clip_box(), see comment there.
          FOG_ASSERT(RASTER_SPAN_C_THRESHOLD <= 4);

          xc += len;
          span->setX1(xc);

#if FOG_ARCH_BITS >= 64 && defined(FOG_ARCH_UNALIGNED_ACCESS_64)
          Acc::p64Store8u(mask,
            (uint64_t)alpha * FOG_UINT64_C(0x0001000100010001));
          mask += len * 2;
#elif FOG_ARCH_BITS >= 32 && defined(FOG_ARCH_UNALIGNED_ACCESS_32)
          alpha *= 0x00010001;
          Acc::p32Store4u(mask + 0, alpha);
          Acc::p32Store4u(mask + 4, alpha);
          mask += len * 2;
#else
          // Unoptimized case.
          do {
            Acc::p32Store2a(mask, alpha);
            mask += 2;
          } while (--len);
#endif

          if (xc == cPtr->x1)
            goto _AdvanceClipAlphaNonZero;

          FOG_ASSERT(xc < cPtr->x1);
          FOG_ASSERT(xc == x0);

          x0 = Math::min<int>(x1, cPtr->x1);
          goto _ContinueSpan;
        }
        else
        {
          NEW_SPAN(span, return);
          span->setX0AndType(xc, RASTER_SPAN_C);
          xc += len;
          span->setX1(xc);
          span->setConstMask(alpha);

          if (xc != cPtr->x1)
          {
            FOG_ASSERT(xc < cPtr->x1);
            FOG_ASSERT(xc == x0);
            goto _NewSpan;
          }

          goto _AdvanceClipAlphaNonZero;
        }
      }
    }
    else
    {
      if (alpha == 0)
        goto _AdvanceClip;
      else
        goto _AdvanceClipAlphaNonZero;
    }

    // ------------------------------------------------------------------------
    // [Fill / Skip]
    // ------------------------------------------------------------------------

_Fill:
    span = scanline->end(span);
    if (span != NULL)
    {
#if defined(FOG_DEBUG_RASTERIZER)
      Rasterizer_dumpSpans(y0, scanline->getSpans());
#endif // FOG_DEBUG_RASTERIZER

      if (yPos != y0)
        filler->skip(y0 - yPos);
      yPos = y0 + 1;

      filler->process(span);
    }
    goto _NextRow;
  }
}

// ============================================================================
// [Fog::PathRasterizer8 - Render - Clip-Mask]
// ============================================================================

template<int _RULE, int _USE_ALPHA>
static void FOG_CDECL PathRasterizer8_render_st_clip_mask(
  Rasterizer8* _self, RasterFiller* filler, RasterScanline8* scanline)
{
  // TODO: Rasterizer - clip-mask
  PathRasterizer8* self = static_cast<PathRasterizer8*>(_self);
}

// ============================================================================
// [Fog::PathRasterizer8 - Finalize]
// ============================================================================

err_t PathRasterizer8::finalize()
{
  PathRasterizer8* self = this;

  // If already finalized this is the NOP.
  if (_error != ERR_OK || _isFinalized)
    return _error;

  // If no shape has been added, then the output is not valid.
  if (_boundingBox.y0 == -1)
    goto _NotValid;

  // Normalize bounding box to our standard, x1/y1 coordinates are outside.
  _boundingBox.x1++;
  _boundingBox.y1++;

  // Small tolerance here due to the floating point arithmetic. Sweep scanline
  // never return anything beyond the bounding box.
  FOG_ASSERT(_boundingBox.x1 <= _sceneBox.x1 + 1);
  FOG_ASSERT(_boundingBox.y1 <= _sceneBox.y1 + 1);

  if (_boundingBox.x1 > _sceneBox.x1) _boundingBox.x1 = _sceneBox.x1;
  if (_boundingBox.y1 > _sceneBox.y1) _boundingBox.y1 = _sceneBox.y1;

  // We added small tolerance to the rasterizer because of floating point based
  // clipping, but if there is no bounding box (due to correction above) then
  // there is no shape.
  _isValid = (_boundingBox.x0 != _boundingBox.x1 &&
              _boundingBox.y0 != _boundingBox.y1);
  _isFinalized = true;

  // Setup render method.
  if (self->_fillRule == FILL_RULE_NON_ZERO)
    self->_render = Rasterizer_api.path8.render_nonzero[self->_opacity != 0x100][self->_clipType];
  else
    self->_render = Rasterizer_api.path8.render_evenodd[self->_opacity != 0x100][self->_clipType];
  return ERR_OK;

_NotValid:
  _isValid = false;
  _isFinalized = true;
  return ERR_OK;

#undef SETUP_FUNCS
}

FOG_NO_EXPORT void Rasterizer_init(void)
{
  // --------------------------------------------------------------------------
  // [Fog::BoxRasterizer8]
  // --------------------------------------------------------------------------

  Rasterizer_api.box8.init32x0 = BoxRasterizer8_init32x0;
  Rasterizer_api.box8.init24x8 = BoxRasterizer8_init24x8;

  Rasterizer_api.box8.render_32x0[RASTER_CLIP_BOX   ] = BoxRasterizer8_render_32x0_st_clip_box;
  Rasterizer_api.box8.render_32x0[RASTER_CLIP_REGION] = BoxRasterizer8_render_32x0_st_clip_region;
  Rasterizer_api.box8.render_32x0[RASTER_CLIP_MASK  ] = BoxRasterizer8_render_32x0_st_clip_mask;

  Rasterizer_api.box8.render_24x8[RASTER_CLIP_BOX   ] = BoxRasterizer8_render_24x8_st_clip_box;
  Rasterizer_api.box8.render_24x8[RASTER_CLIP_REGION] = BoxRasterizer8_render_24x8_st_clip_region;
  Rasterizer_api.box8.render_24x8[RASTER_CLIP_MASK  ] = BoxRasterizer8_render_24x8_st_clip_mask;

  // --------------------------------------------------------------------------
  // [Fog::PathRasterizer8]
  // --------------------------------------------------------------------------

  Rasterizer_api.path8.render_nonzero[0][RASTER_CLIP_BOX   ] = PathRasterizer8_render_st_clip_box   <FILL_RULE_NON_ZERO, 0>;
  Rasterizer_api.path8.render_nonzero[0][RASTER_CLIP_REGION] = PathRasterizer8_render_st_clip_region<FILL_RULE_NON_ZERO, 0>;
  Rasterizer_api.path8.render_nonzero[0][RASTER_CLIP_MASK  ] = PathRasterizer8_render_st_clip_mask  <FILL_RULE_NON_ZERO, 0>;

  Rasterizer_api.path8.render_nonzero[1][RASTER_CLIP_BOX   ] = PathRasterizer8_render_st_clip_box   <FILL_RULE_NON_ZERO, 1>;
  Rasterizer_api.path8.render_nonzero[1][RASTER_CLIP_REGION] = PathRasterizer8_render_st_clip_region<FILL_RULE_NON_ZERO, 1>;
  Rasterizer_api.path8.render_nonzero[1][RASTER_CLIP_MASK  ] = PathRasterizer8_render_st_clip_mask  <FILL_RULE_NON_ZERO, 1>;

  Rasterizer_api.path8.render_evenodd[0][RASTER_CLIP_BOX   ] = PathRasterizer8_render_st_clip_box   <FILL_RULE_EVEN_ODD, 0>;
  Rasterizer_api.path8.render_evenodd[0][RASTER_CLIP_REGION] = PathRasterizer8_render_st_clip_region<FILL_RULE_EVEN_ODD, 0>;
  Rasterizer_api.path8.render_evenodd[0][RASTER_CLIP_MASK  ] = PathRasterizer8_render_st_clip_mask  <FILL_RULE_EVEN_ODD, 0>;

  Rasterizer_api.path8.render_evenodd[1][RASTER_CLIP_BOX   ] = PathRasterizer8_render_st_clip_box   <FILL_RULE_EVEN_ODD, 1>;
  Rasterizer_api.path8.render_evenodd[1][RASTER_CLIP_REGION] = PathRasterizer8_render_st_clip_region<FILL_RULE_EVEN_ODD, 1>;
  Rasterizer_api.path8.render_evenodd[1][RASTER_CLIP_MASK  ] = PathRasterizer8_render_st_clip_mask  <FILL_RULE_EVEN_ODD, 1>;
}

} // Fog namespace
