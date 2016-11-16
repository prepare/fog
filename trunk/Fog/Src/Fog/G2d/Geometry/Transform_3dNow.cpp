// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

#include <Fog/Core/Face/Face_MMX.h>
#include <Fog/Core/Face/Face_3dNow.h>
#include <Fog/Core/Global/Init_p.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/G2d/Geometry/Point.h>
#include <Fog/G2d/Geometry/Transform.h>

namespace Fog {

// ============================================================================
// [Fog::Transform - MapPoints]
// ============================================================================

static void FOG_CDECL TransformF_mapPointsF_Translation_3dNow(const TransformF* self, PointF* dst, const PointF* src, size_t length)
{
  size_t i;

  __m64 m_20_21;

  Face::m64fLoad8(m_20_21, &self->_20);

  for (i = length >> 2; i; i--, dst += 4, src += 4)
  {
    Face::m64f src0, src1, src2, src3;

    Face::m64fLoad8(src0, src + 0);
    Face::m64fLoad8(src1, src + 1);
    Face::m64fLoad8(src2, src + 2);
    Face::m64fLoad8(src3, src + 3);

    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fAdd(src1, src1, m_20_21);
    Face::m64fAdd(src2, src2, m_20_21);
    Face::m64fAdd(src3, src3, m_20_21);

    Face::m64fStore8(dst + 0, src0);
    Face::m64fStore8(dst + 1, src1);
    Face::m64fStore8(dst + 2, src2);
    Face::m64fStore8(dst + 4, src3);
  }

  for (i = length & 3; i; i--, dst++, src++)
  {
    Face::m64f src0;

    Face::m64fLoad8(src0, src + 0);
    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fStore8(dst + 0, src0);
  }

  Face::m64fEMMS();
}

static void FOG_CDECL TransformF_mapPointsF_Scaling_3dNow(const TransformF* self, PointF* dst, const PointF* src, size_t length)
{
  size_t i;

  __m64 m_00_11;
  __m64 m_20_21;

  Face::m64fLoad8(m_00_11, &self->_00, &self->_11);
  Face::m64fLoad8(m_20_21, &self->_20);

  for (i = length >> 2; i; i--, dst += 4, src += 4)
  {
    Face::m64f src0, src1, src2, src3;

    Face::m64fLoad8(src0, src + 0);
    Face::m64fLoad8(src1, src + 1);
    Face::m64fLoad8(src2, src + 2);
    Face::m64fLoad8(src3, src + 3);

    Face::m64fMul(src0, src0, m_00_11);
    Face::m64fMul(src1, src1, m_00_11);
    Face::m64fMul(src2, src2, m_00_11);
    Face::m64fMul(src3, src3, m_00_11);

    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fAdd(src1, src1, m_20_21);
    Face::m64fAdd(src2, src2, m_20_21);
    Face::m64fAdd(src3, src3, m_20_21);

    Face::m64fStore8(dst + 0, src0);
    Face::m64fStore8(dst + 1, src1);
    Face::m64fStore8(dst + 2, src2);
    Face::m64fStore8(dst + 4, src3);
  }

  for (i = length & 3; i; i--, dst++, src++)
  {
    Face::m64f src0;

    Face::m64fLoad8(src0, src + 0);
    Face::m64fMul(src0, src0, m_00_11);
    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fStore8(dst + 0, src0);
  }

  Face::m64fEMMS();
}

static void FOG_CDECL TransformF_mapPointsF_Affine_3dNow(const TransformF* self, PointF* dst, const PointF* src, size_t length)
{
  size_t i;

  __m64 m_00_11;
  __m64 m_10_01;
  __m64 m_20_21;

  Face::m64fLoad8(m_00_11, &self->_00, &self->_11);
  Face::m64fLoad8(m_10_01, &self->_10, &self->_01);
  Face::m64fLoad8(m_20_21, &self->_20);

  for (i = length >> 2; i; i--, dst += 4, src += 4)
  {
    Face::m64f src0, src1;
    Face::m64f tmp0, tmp1;

    Face::m64fLoad8(src0, src + 0);
    Face::m64fLoad8(src1, src + 1);

    Face::m64fSwapXY(tmp0, src0);
    Face::m64fSwapXY(tmp1, src1);

    Face::m64fMul(src0, src0, m_00_11);
    Face::m64fMul(src1, src1, m_00_11);

    Face::m64fMul(tmp0, tmp0, m_10_01);
    Face::m64fMul(tmp1, tmp1, m_10_01);

    Face::m64fAdd(src0, src0, tmp0);
    Face::m64fAdd(src1, src1, tmp1);

    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fAdd(src1, src1, m_20_21);

    Face::m64fStore8(dst + 0, src0);
    Face::m64fStore8(dst + 1, src1);

    Face::m64fLoad8(src0, src + 2);
    Face::m64fLoad8(src1, src + 3);

    Face::m64fSwapXY(tmp0, src0);
    Face::m64fSwapXY(tmp1, src1);

    Face::m64fMul(src0, src0, m_00_11);
    Face::m64fMul(src1, src1, m_00_11);

    Face::m64fMul(tmp0, tmp0, m_10_01);
    Face::m64fMul(tmp1, tmp1, m_10_01);

    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fAdd(src1, src1, m_20_21);

    Face::m64fAdd(src0, src0, tmp0);
    Face::m64fAdd(src1, src1, tmp1);

    Face::m64fStore8(dst + 2, src0);
    Face::m64fStore8(dst + 3, src1);
  }

  for (i = length & 3; i; i--, dst++, src++)
  {
    Face::m64f src0;
    Face::m64f tmp0;

    Face::m64fLoad8(src0, src + 0);
    Face::m64fSwapXY(tmp0, src0);
    Face::m64fMul(src0, src0, m_00_11);
    Face::m64fMul(tmp0, tmp0, m_10_01);
    Face::m64fAdd(src0, src0, m_20_21);
    Face::m64fAdd(src0, src0, tmp0);
    Face::m64fStore8(dst + 0, src0);
  }

  Face::m64fEMMS();
}

// ============================================================================
// [Fog::G2d - Library Initializers]
// ============================================================================

FOG_NO_EXPORT void Transform_init_3dNow(void)
{
  _api.transformf_mapPointsF[TRANSFORM_TYPE_TRANSLATION] = _TransformF_mapPointsF_Translation_3dNow;
  _api.transformf_mapPointsF[TRANSFORM_TYPE_SCALING    ] = _TransformF_mapPointsF_Scaling_3dNow;
  _api.transformf_mapPointsF[TRANSFORM_TYPE_ROTATION   ] = _TransformF_mapPointsF_Affine_3dNow;
  _api.transformf_mapPointsF[TRANSFORM_TYPE_AFFINE     ] = _TransformF_mapPointsF_Affine_3dNow;
}

} // Fog namespace
