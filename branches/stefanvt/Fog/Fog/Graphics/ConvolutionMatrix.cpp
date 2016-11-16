// [Fog-Graphics Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Math.h>
#include <Fog/Core/Memory.h>
#include <Fog/Graphics/ConvolutionMatrix.h>
#include <Fog/Graphics/Geometry.h>

namespace Fog {

// ============================================================================
// [Fog::ConvolutionMatrixI]
// ============================================================================

Static<ConvolutionMatrixI::Data> ConvolutionMatrixI::sharedNull;

ConvolutionMatrixI::ConvolutionMatrixI() :
  _d(sharedNull->ref())
{
}

ConvolutionMatrixI::ConvolutionMatrixI(const ConvolutionMatrixI& other) :
  _d(other._d->ref())
{
}

ConvolutionMatrixI::~ConvolutionMatrixI()
{
  _d->deref();
}

ConvolutionMatrixI ConvolutionMatrixI::fromData(int w, int h, const ValueType* data)
{
  Data* d = NULL;

  if (w > 0 && h > 0 && (d = Data::alloc(w, h)))
    Memory::copy(d->m, data, (sysuint_t)w * (sysuint_t)h * (sysuint_t)sizeof(ValueType));
  else
    d = sharedNull->ref();

  return ConvolutionMatrixI(d);
}

err_t ConvolutionMatrixI::_detach()
{
  if (_d->refCount.get() == 1) return ERR_OK;

  Data* newd = Data::alloc(_d->width, _d->height);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  Data::copy(newd, 0, 0, _d, 0, 0, _d->width, _d->height);
  return ERR_OK;
}

err_t ConvolutionMatrixI::setData(int width, int height, const ValueType* data)
{
  if (_d->refCount.get() == 1 && (
      (_d->width == width && _d->height == height) ||
      (_d->width == height && _d->height == width)))
  {
    _d->width = width;
    _d->height = height;
    Memory::copy(_d->m, data, (sysuint_t)width * (sysuint_t)height * (sysuint_t)sizeof(ValueType));
    return ERR_OK;
  }

  Data* newd = Data::alloc(width, height);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  Memory::copy(newd->m, data, (sysuint_t)width * (sysuint_t)height * (sysuint_t)sizeof(ValueType));
  atomicPtrXchg(&_d, newd)->deref();
  return ERR_OK;
}

err_t ConvolutionMatrixI::create(int width, int height)
{
  if (_d->width == width && _d->height == height) return ERR_OK;

  Data* newd = Data::alloc(width, height);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  atomicPtrXchg(&_d, newd)->deref();
  return ERR_OK;
}

err_t ConvolutionMatrixI::extend(int width, int height, ValueType value)
{
  if (_d->width == width && _d->height == height) return ERR_OK;

  Data* newd = Data::alloc(width, height);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  int copyw = Math::min(_d->width, width);
  int copyh = Math::min(_d->height, height);
  int zerow = width - copyw;

  int x, y;

  ValueType *dstCur = newd->m;
  ValueType* srcM = _d->m;

  for (y = copyh; y; y--, srcM += _d->width)
  {
    ValueType *srcCur = srcM;

    for (x = copyw; x; x--, dstCur++, srcCur++) *dstCur++ = *srcCur++;
    for (x = zerow; x; x--) *dstCur++ = value;
  }

  for (y = copyh; y; y--)
  {
    for (x = width; x; x--) *dstCur++ = value;
  }

  atomicPtrXchg(&_d, newd)->deref();
  return ERR_OK;
}

int ConvolutionMatrixI::getCell(int x, int y) const
{
  Data* d = _d;
  if ((uint)x >= (uint)d->width || (uint)y >= (uint)_d->height) return 0;

  return d->m[y * d->width + x];
}

err_t ConvolutionMatrixI::setCell(int x, int y, int val)
{
  if ((uint)x >= (uint)getWidth() || (uint)y >= (uint)getHeight()) return ERR_OK;

  FOG_RETURN_ON_ERROR(detach());

  Data* d = _d;
  d->m[y * d->width + x] = val;

  return ERR_OK;
}

err_t ConvolutionMatrixI::fill(const IntRect& rect, ValueType value)
{
  int x1 = Math::max<int>(rect.getX1(), 0);
  int y1 = Math::max<int>(rect.getY1(), 0);

  int x2 = Math::min<int>(rect.getX2(), getWidth());
  int y2 = Math::min<int>(rect.getY2(), getHeight());

  if (x1 >= x2 || y1 >= y2) return ERR_OK;

  FOG_RETURN_ON_ERROR(detach());

  int width = getWidth();
  ValueType* cur = &_d->m[y1 * width + x1];
  
  int w = x2 - x1;
  int h = y2 - y1;

  for (int y = h; y; y--, cur += width)
  {
    ValueType* p = cur;
    for (int x = w; x; x--, p++) *p = value;
  }

  return ERR_OK;
}

ConvolutionMatrixI& ConvolutionMatrixI::operator=(const ConvolutionMatrixI& other)
{
  atomicPtrXchg(&_d, other._d->ref())->deref();
  return *this;
}

ConvolutionMatrixI::ValueType* ConvolutionMatrixI::operator[](int y)
{
  Data* d = _d;
  if ((uint)y >= (uint)_d->height) return NULL;

  if (FOG_UNLIKELY(!isDetached()))
  {
    if (_detach() != ERR_OK) return NULL;
    d = _d;
  }

  return d->m + y * d->width;
}

// ============================================================================
// [Fog::ConvolutionMatrixI::Data]
// ============================================================================

ConvolutionMatrixI::Data* ConvolutionMatrixI::Data::alloc(int w, int h)
{
  if (w == 0 || h == 0) return sharedNull->ref();

  Data* d = (Data*)Memory::alloc(sizeof(Data) + (w * h) * sizeof(ValueType));
  if (!d) return NULL;

  d->refCount.init(1);
  d->width = w;
  d->height = h;
  return d;
}

void ConvolutionMatrixI::Data::copy(Data* dst, int dstX, int dstY, Data* src, int srcX, int srcY, int w, int h)
{
  ValueType* dstM = dst->m + dstY * dst->width + dstX;
  ValueType* srcM = src->m + srcY * src->width + srcX;

  for (int y = h; y; y--, dstM += dst->width, srcM += src->width)
  {
    ValueType* dstCur = dstM;
    ValueType* srcCur = srcM;

    for (int x = w; x; x--, dstCur++, srcCur++) *dstCur++ = *srcCur++;
  }
}

} // Fog namespace

// ============================================================================
// [Library Initializers]
// ============================================================================

FOG_INIT_DECLARE err_t fog_convolutionmatrix_init(void)
{
  using namespace Fog;

  ConvolutionMatrixI::sharedNull.init();
  return ERR_OK;
}

FOG_INIT_DECLARE void fog_convolutionmatrix_shutdown(void)
{
  using namespace Fog;

  ConvolutionMatrixI::sharedNull.destroy();
}
