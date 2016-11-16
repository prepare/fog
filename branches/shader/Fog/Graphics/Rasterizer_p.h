// [Fog-Graphics Library - Private API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_GRAPHICS_RASTERIZER_P_H
#define _FOG_GRAPHICS_RASTERIZER_P_H

// [Dependencies]
#include <Fog/Core/Build.h>
#include <Fog/Graphics/Constants.h>
#include <Fog/Graphics/Geometry.h>
#include <Fog/Graphics/Path.h>
#include <Fog/Graphics/Span_p.h>
#include <Fog/Graphics/Scanline_p.h>

namespace Fog {

//! @addtogroup Fog_Graphics_Private
//! @{

// ============================================================================
// [Fog::Rasterizer]
// ============================================================================

//! @internal
//!
//! Scanline polygon rasterizer.
//!
//! Polygon rasterizer that is used to render filled polygons with 
//! high-quality Anti-Aliasing. Internally, by default, the class uses 
//! integer coordinates in format 24.8, i.e. 24 bits for integer part 
//! and 8 bits for fractional - see poly_subpixel_shift. This class can be 
//! used in the following  way:
//!
//! 1. getFillRule() / setFillRule(int fillRule) - set fill rule.
//!
//! 2. reset() - optional, needed only if you are reusing rasterizer.
//!
//! 3. addPath(path) - make the polygon. One can create more than one contour,
//!    but each contour must consist of at least 3 vertices, is the absolute
//!    minimum of vertices that define a triangle. The algorithm does not check
//!    either the number of vertices nor coincidence of their coordinates, but
//!    in the worst case it just won't draw anything.
//!
//!    The orger of the vertices (clockwise or counterclockwise) 
//!    is important when using the non-zero filling rule (@c FILL_NON_ZERO).
//!    In this case the vertex order of all the contours must be the same
//!    if you want your intersecting polygons to be without "holes".
//!    You actually can use different vertices order. If the contours do not 
//!    intersect each other the order is not important anyway. If they do, 
//!    contours with the same vertex order will be rendered without "holes" 
//!    while the intersecting contours with different orders will have "holes".
//!
//! setFillRule() can be called anytime before "sweeping".
//!
//! Rasterizers are pooled to maximize performance and decrease memory 
//! fragmentation. If you want to create or free rasterizer use methods
//! @c Rasterizer::getRasterizer() and @c Rasterizer::releaseRasterizer(),
//! respectively.
struct FOG_HIDDEN Rasterizer
{
  // --------------------------------------------------------------------------
  // [Cell]
  // --------------------------------------------------------------------------

#include <Fog/Core/Compiler/PackDWord.h>
  //! @internal
  struct FOG_HIDDEN CellXY
  {
    int x;
    int y;
    int cover;
    int area;

    FOG_INLINE void setCell(int _x, int _y, int _cover, int _area) { x = _x; y = _y; cover = _cover; area = _area; }
    FOG_INLINE void setCell(const CellXY& other) { x = other.x; y = other.y; cover = other.cover; area = other.area; }

    FOG_INLINE void setPosition(int _x, int _y) { x = _x, y = _y; }
    FOG_INLINE bool hasPosition(int _x, int _y) const { return ((_x - x) | (_y - y)) == 0; }

    FOG_INLINE void setCovers(int _cover, int _area) { cover = _cover; area = _area; }
    FOG_INLINE void addCovers(int _cover, int _area) { cover += _cover; area += _area; }
    FOG_INLINE bool hasCovers() const { return (cover | area) != 0; }
  };
#include <Fog/Core/Compiler/PackRestore.h>

#include <Fog/Core/Compiler/PackDWord.h>
  //! @internal
  struct FOG_HIDDEN CellX
  {
    int x;
    int cover;
    int area;

    FOG_INLINE void set(int _x, int _cover, int _area)
    {
      x = _x;
      cover = _cover;
      area = _area;
    }

    FOG_INLINE void set(const CellX& other)
    {
      x = other.x;
      cover = other.cover;
      area = other.area;
    }

    FOG_INLINE void set(const CellXY& other)
    {
      x = other.x;
      cover = other.cover;
      area = other.area;
    }
  };
#include <Fog/Core/Compiler/PackRestore.h>

  // --------------------------------------------------------------------------
  // [CellXYBuffer]
  // --------------------------------------------------------------------------

  //! @internal
  //!
  //! @brief Cell buffer.
  struct FOG_HIDDEN CellXYBuffer
  {
    CellXYBuffer* prev;
    CellXYBuffer* next;
    uint32_t capacity;
    uint32_t count;
    CellXY cells[1];
  };

  // --------------------------------------------------------------------------
  // [RowInfo]
  // --------------------------------------------------------------------------

  //! @internal
  //!
  //! @brief Lookup table that contains index and count of cells in sorted cells
  //! buffer. Each index to this table represents one row.
  struct FOG_HIDDEN RowInfo
  {
    uint32_t index;
    uint32_t count;
  };

  // --------------------------------------------------------------------------
  // [Typedefs]
  // --------------------------------------------------------------------------

  typedef Span8* (*SweepScanlineSimpleFn)(Rasterizer* rasterizer, Scanline8& scanline, int y);
  typedef Span8* (*SweepScanlineRegionFn)(Rasterizer* rasterizer, Scanline8& scanline, int y, const IntBox* clipBoxes, sysuint_t count);
  typedef Span8* (*SweepScanlineSpansFn)(Rasterizer* rasterizer, Scanline8& scanline, int y, const Span8* clipSpans);

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  Rasterizer();
  virtual ~Rasterizer();

  //! @brief Called before rasterizer is pooled. Default implementation is NOP.
  virtual void pooled() = 0;

  //! @brief Reset rasterizer.
  virtual void reset() = 0;

  // --------------------------------------------------------------------------
  // [Clip Box]
  // --------------------------------------------------------------------------

  FOG_INLINE const IntBox& getClipBox() const { return _clipBox; }

  virtual void setClipBox(const IntBox& clipBox) = 0;
  virtual void resetClipBox() = 0;

  // --------------------------------------------------------------------------
  // [Bounding Box]
  // --------------------------------------------------------------------------

  //! @brief Get rasterized object bounding box.
  //!
  //! @note This method is only valid after @c finalize() call.
  FOG_INLINE const IntBox& getBoundingBox() const { return _boundingBox; }

  // --------------------------------------------------------------------------
  // [Error]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t getError() const { return _error; }

  virtual void setError(err_t error) = 0;
  virtual void resetError() = 0;

  // --------------------------------------------------------------------------
  // [Fill Rule]
  // --------------------------------------------------------------------------

  FOG_INLINE uint32_t getFillRule() const { return _fillRule; }
  virtual void setFillRule(uint32_t fillRule) = 0;

  // --------------------------------------------------------------------------
  // [Alpha]
  // --------------------------------------------------------------------------

  FOG_INLINE uint32_t getAlpha() const { return _alpha; }
  virtual void setAlpha(uint32_t alpha) = 0;

  // --------------------------------------------------------------------------
  // [Finalized / Empty]
  // --------------------------------------------------------------------------

  FOG_INLINE bool finalized() const { return _finalized; }
  FOG_INLINE bool isValid() const { return _isValid; }

  // --------------------------------------------------------------------------
  // [Commands]
  // --------------------------------------------------------------------------

  virtual void addPath(const DoublePath& path) = 0;
  virtual void finalize() = 0;

  // --------------------------------------------------------------------------
  // [Sweep]
  // --------------------------------------------------------------------------

  //! @brief Sweep scanline @a y.
  FOG_INLINE Span8* sweepScanline(Scanline8& scanline, int y)
  { return _sweepScanlineSimpleFn(this, scanline, y); }

  //! @brief Enhanced version of @c sweepScanline() that accepts clip boundary.
  //!
  //! This method is called by raster paint engine if clipping region is complex.
  FOG_INLINE Span8* sweepScanline(Scanline8& scanline, int y, const IntBox* clipBoxes, sysuint_t count)
  { return _sweepScanlineRegionFn(this, scanline, y, clipBoxes, count); }

  //! @brief Enhanced version of @c sweepScanline() that accepts clip spans.
  //!
  //! This method is called by raster paint engine if clipping region is mask.
  FOG_INLINE Span8* sweepScanline(Scanline8& scanline, int y, const Span8* clipSpans)
  { return _sweepScanlineSpansFn(this, scanline, y, clipSpans); }

  // --------------------------------------------------------------------------
  // [Pooling]
  // --------------------------------------------------------------------------

  //! @brief Get rasterizer instance.
  static Rasterizer* getRasterizer();
  //! @brief Release rasterizer instance.
  static void releaseRasterizer(Rasterizer* rasterizer);

  //! @brief Get cell buffer instance.
  static CellXYBuffer* getCellXYBuffer();
  //! @brief Release cell buffer instance.
  static void releaseCellXYBuffer(CellXYBuffer* cellBuffer);

  //! @brief Free all pooled rasterizer and cell buffer instances.
  static void cleanup();

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  SweepScanlineSimpleFn _sweepScanlineSimpleFn;
  SweepScanlineRegionFn _sweepScanlineRegionFn;
  SweepScanlineSpansFn _sweepScanlineSpansFn;

  //! @brief Clip bounding box (always must be valid, initialy set to zero).
  IntBox _clipBox;

  //! @brief bounding box of rasterized object (after clipping).
  IntBox _boundingBox;

  //! @brief Fill rule;
  uint32_t _fillRule;

  //! @brief Alpha.
  uint32_t _alpha;

  //! @brief Rasterizer error code.
  err_t _error;

  //! @brief Whether rasterizer is finalized.
  bool _finalized;

  //! @brief Whether the rasterized object is empty (not paint).
  bool _isValid;

private:
  //! @brief Link to next pooled @c Rasterizer instance. Always NULL when you 
  //! get rasterizer by @c Rasterizer::getRasterizer() method.
  Rasterizer* _poolNext;

  FOG_DISABLE_COPY(Rasterizer)
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_GRAPHICS_RASTERIZER_P_H
