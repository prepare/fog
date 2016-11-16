// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Acc/AccSse.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/G2d/Geometry/Box.h>
#include <Fog/G2d/Geometry/Internals_p.h>
#include <Fog/G2d/Geometry/Path.h>
#include <Fog/G2d/Geometry/PathClipper.h>
#include <Fog/G2d/Geometry/Rect.h>
#include <Fog/G2d/Geometry/Transform.h>

namespace Fog {

// ============================================================================
// [Fog::PathClipper - MeasurePath]
// ============================================================================

static uint32_t FOG_CDECL PathClipperF_measurePath_SSE(PathClipperF* self, const PathF* src)
{
  self->_lastIndex = INVALID_INDEX;

  __m128f xmmClipBox;
  __m128f xmmClipMin;
  __m128f xmmClipMax;

  Acc::m128fLoad16u(xmmClipBox, &self->_clipBox);
  Acc::m128fShuffle<1, 0, 1, 0>(xmmClipMin, xmmClipBox);
  Acc::m128fShuffle<3, 2, 3, 2>(xmmClipMax, xmmClipBox);

  // If path bounding box is not dirty then we can simply use it instead of
  // performing the calculation.
  if (src->_hasBoundingBox())
  {
    __m128f xmm0;
    __m128f xmm1;
    int msk;

    Acc::m128fLoad16u(xmm0,  &src->_d->boundingBox);
    Acc::m128fCopy(xmm1, xmm0);

    Acc::m128fCmpGePS(xmm0, xmm0, xmmClipMin);
    Acc::m128fCmpLePS(xmm1, xmm1, xmmClipMax);

    Acc::m128fAnd(xmm0, xmm0, xmm1);
    Acc::m128fMoveMask(msk, xmm0);

    if (msk == 0xF)
    {
      _FOG_PATH_VERIFY_BOUNDING_BOX(*src);
      return PATH_CLIPPER_MEASURE_BOUNDED;
    }
  }

  Acc::m128fXor(xmmClipBox, xmmClipBox, FOG_XMM_GET_CONST_PS(m128f_p0_p0_sn_sn));

  size_t i = src->getLength();
  const uint8_t* cmd = src->getCommands();
  const PointF* pts = src->getVertices();

  // Current command with _PATH_CMD_HAS_INITIAL flag included. Instead of using
  // variable for 'hasInitial' flag and command separately, we combined it into
  // single variable, thus one branch has been eliminated within the loop.
  uint32_t c = 0;

  while (i)
  {
    __m128f xmm0;
    int msk;

    Acc::m128fZero(xmm0);
    Acc::m128fLoad8Lo(xmm0, pts);

    c |= static_cast<uint32_t>(cmd[0]);
    switch (c)
    {
      // ----------------------------------------------------------------------
      // [Move-To]
      // ----------------------------------------------------------------------

      case PATH_CMD_MOVE_TO:
      case PATH_CMD_MOVE_TO | _PATH_CMD_HAS_INITIAL:
        c = _PATH_CMD_HAS_INITIAL;

        Acc::m128fStore8Lo(&self->_lastMoveTo, xmm0);
        // ... Fall through ...

      // ----------------------------------------------------------------------
      // [Line-To]
      // ----------------------------------------------------------------------

      case PATH_CMD_LINE_TO | _PATH_CMD_HAS_INITIAL:
      {
        c = _PATH_CMD_HAS_INITIAL;

        Acc::m128fShuffle<1, 0, 1, 0>(xmm0, xmm0, xmm0);
        Acc::m128fXor(xmm0, xmm0, FOG_XMM_GET_CONST_PS(m128f_p0_p0_sn_sn));
        Acc::m128fCmpLePS(xmm0, xmm0, xmmClipBox);

        Acc::m128fMoveMask(msk, xmm0);
        if (msk != 0xF)
          goto _Unbounded;

        i--;
        cmd++;
        pts++;
        break;
      }

      // ----------------------------------------------------------------------
      // [Quad-To]
      // ----------------------------------------------------------------------

      case PATH_CMD_QUAD_TO | _PATH_CMD_HAS_INITIAL:
      {
        __m128f xmm1;

        c = _PATH_CMD_HAS_INITIAL;
        FOG_ASSERT(i >= 2);

        Acc::m128fLoad8Hi(xmm0, pts + 1);

        Acc::m128fCopy(xmm1, xmm0);
        Acc::m128fCmpGePS(xmm0, xmm0, xmmClipMin);
        Acc::m128fCmpLePS(xmm1, xmm1, xmmClipMax);

        Acc::m128fAnd(xmm0, xmm0, xmm1);

        Acc::m128fMoveMask(msk, xmm0);
        if (msk != 0xF)
          goto _Unbounded;

        i   -= 2;
        cmd += 2;
        pts += 2;
        break;
      }

      // ----------------------------------------------------------------------
      // [Cubic-To]
      // ----------------------------------------------------------------------

      case PATH_CMD_CUBIC_TO | _PATH_CMD_HAS_INITIAL:
      {
        __m128f xmm1, xmm2;

        c = _PATH_CMD_HAS_INITIAL;
        FOG_ASSERT(i >= 3);

        Acc::m128fZero(xmm2);
        Acc::m128fLoad8Hi(xmm0, pts + 1);
        Acc::m128fLoad8Lo(xmm2, pts + 2);

        Acc::m128fShuffle<1, 0, 1, 0>(xmm2, xmm2, xmm2);
        Acc::m128fXor(xmm2, xmm2, FOG_XMM_GET_CONST_PS(m128f_p0_p0_sn_sn));

        Acc::m128fCopy(xmm1, xmm0);
        Acc::m128fCmpGePS(xmm0, xmm0, xmmClipMin);
        Acc::m128fCmpLePS(xmm1, xmm1, xmmClipMax);
        Acc::m128fCmpLePS(xmm2, xmm2, xmmClipBox);

        Acc::m128fAnd(xmm0, xmm0, xmm1);
        Acc::m128fAnd(xmm0, xmm0, xmm2);

        Acc::m128fMoveMask(msk, xmm0);
        if (msk != 0xF)
          goto _Unbounded;

        i   -= 3;
        cmd += 3;
        pts += 3;
        break;
      }

      // ----------------------------------------------------------------------
      // [Close]
      // ----------------------------------------------------------------------

      case PATH_CMD_CLOSE:
      case PATH_CMD_CLOSE | _PATH_CMD_HAS_INITIAL:
        c = 0;

        i--;
        cmd++;
        pts++;
        break;

      // ----------------------------------------------------------------------
      // [Error]
      // ----------------------------------------------------------------------

      case PATH_CMD_LINE_TO:
      case PATH_CMD_QUAD_TO:
      case PATH_CMD_CUBIC_TO:
        goto _Invalid;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }
  return PATH_CLIPPER_MEASURE_BOUNDED;

_Unbounded:
  self->_lastIndex = (size_t)(cmd - src->getCommands());
  return PATH_CLIPPER_MEASURE_UNBOUNDED;

_Invalid:
  return PATH_CLIPPER_MEASURE_INVALID;
}

// ============================================================================
// [Init / Fini]
// ============================================================================

FOG_NO_EXPORT void PathClipper_init_SSE(void)
{
  fog_api.pathclipperf_measurePath = PathClipperF_measurePath_SSE;
}

} // Fog namespace
