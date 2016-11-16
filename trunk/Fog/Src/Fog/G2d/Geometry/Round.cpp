// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Global/Init_p.h>
#include <Fog/Core/Global/Private.h>
#include <Fog/Core/Math/Constants.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/G2d/Geometry/CBezier.h>
#include <Fog/G2d/Geometry/Round.h>
#include <Fog/G2d/Geometry/Transform.h>

namespace Fog {

// ============================================================================
// [Fog::Round - GetBoundingBox]
// ============================================================================

template<typename NumT>
static err_t FOG_CDECL RoundT_getBoundingBox(const NumT_(Round)* self,
  NumT_(Box)* dst,
  const NumT_(Transform)* tr)
{
  // TODO: RoundT<>::getBoundingBox() missing implementation.
  return ERR_OK;
}

// ============================================================================
// [Fog::Round - HitTest]
// ============================================================================

template<typename NumT>
static bool FOG_CDECL RoundT_hitTest(const NumT_(Round)* self,
  const NumT_(Point)* pt)
{
  NumT x = pt->x - self->rect.x;
  NumT y = pt->y - self->rect.y;

  NumT w = self->rect.w;
  NumT h = self->rect.h;

  NumT rx = Math::abs(self->radius.x);
  NumT ry = Math::abs(self->radius.y);

  // Hit-test the bounding box.
  if (x < NumT(0.0) || y < NumT(0.0) || x >= w || y >= h) return false;

  // Normalize rx/ry.
  rx = Math::min(rx, w * NumT(2.0));
  ry = Math::min(ry, h * NumT(2.0));

  // Hit-test the inner two boxes.
  if (x >= rx && x <= w - rx) return true;
  if (y >= ry && y <= h - ry) return true;

  // Hit-test the four symmetric rounded corners. There are used the same trick
  // as in Ellipse hit-test (the elliptic corners are scaled to be circualr).
  x -= rx;
  y -= ry;

  if (x >= rx) x -= self->rect.w - rx - rx;
  if (y >= ry) y -= self->rect.h - ry - ry;

  // No radius or very close to zero (positive hit-test).
  if (Math::isFuzzyZero(rx) || Math::isFuzzyZero(ry)) return true;

  if (rx > ry)
  {
    y = y * rx / ry;
  }
  else if (rx < ry)
  {
    x = x * ry / rx;
    rx = ry;
  }

  NumT ptDist = Math::pow2(x) + Math::pow2(y);
  NumT maxDist = Math::pow2(rx);

  return ptDist <= maxDist;
}

// ============================================================================
// [Init / Fini]
// ============================================================================

FOG_NO_EXPORT void Round_init(void)
{
  fog_api.roundf_getBoundingBox = RoundT_getBoundingBox<float>;
  fog_api.roundd_getBoundingBox = RoundT_getBoundingBox<double>;

  fog_api.roundf_hitTest = RoundT_hitTest<float>;
  fog_api.roundd_hitTest = RoundT_hitTest<double>;
}

} // Fog namespace
