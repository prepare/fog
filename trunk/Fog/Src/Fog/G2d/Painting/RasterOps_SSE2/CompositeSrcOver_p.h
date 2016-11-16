// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_SSE2_COMPOSITESRCOVER_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_SSE2_COMPOSITESRCOVER_P_H

// [Dependencies]
#include <Fog/G2d/Painting/RasterOps_SSE2/CompositeSrc_p.h>

namespace Fog {
namespace RasterOps_SSE2 {

// ============================================================================
// [Fog::RasterOps_SSE2 - CompositeSrcOver]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT CompositeSrcOver
{
  enum { COMBINE_FLAGS = RASTER_COMBINE_OP_SRC_OVER };

  // ==========================================================================
  // [PRGB32 - CBlit - PRGB32 - Helpers]
  // ==========================================================================

  static FOG_INLINE void _prgb32_cblit_prgb32_line(
    uint8_t* dst, int w, const __m128i& src0xmm, const __m128i& sra0xmm)
  {
    Acc::prefetchT0(dst);

    FOG_BLIT_LOOP_32x8_SSE2_INIT()

    FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(C_Opaque)
      __m128i dst0xmm;

      Acc::m128iLoad4(dst0xmm, dst);
      Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
      Acc::m128iMulHiPU16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
      Acc::m128iAddPI16(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iStore4(dst, dst0xmm);

      dst += 4;
    FOG_BLIT_LOOP_32x8_SSE2_ONE_END(C_Opaque)

    FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(C_Opaque)
      __m128i dst0xmm;

      Acc::m128iLoad8(dst0xmm, dst);
      Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
      Acc::m128iMulHiPU16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
      Acc::m128iAddPI16(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iStore8(dst, dst0xmm);

      dst += 8;
    FOG_BLIT_LOOP_32x8_SSE2_TWO_END(C_Opaque)

    FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(C_Opaque)
      __m128i dst0xmm, dst1xmm;
      __m128i dst2xmm, dst3xmm;

      Acc::m128iLoad16a(dst0xmm, dst +  0);
      Acc::m128iLoad16a(dst2xmm, dst + 16);

      Acc::m128iUnpackPI16FromPI8Hi(dst1xmm, dst0xmm);
      Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
      Acc::m128iUnpackPI16FromPI8Hi(dst3xmm, dst2xmm);
      Acc::m128iUnpackPI16FromPI8Lo(dst2xmm, dst2xmm);

      Acc::m128iMulHiPU16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iMulHiPU16(dst1xmm, dst1xmm, sra0xmm);
      Acc::m128iMulHiPU16(dst2xmm, dst2xmm, sra0xmm);
      Acc::m128iMulHiPU16(dst3xmm, dst3xmm, sra0xmm);

      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, dst1xmm);
      Acc::m128iPackPU8FromPU16(dst2xmm, dst2xmm, dst3xmm);
      Acc::m128iAddPI16(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iAddPI16(dst2xmm, dst2xmm, src0xmm);

      Acc::m128iStore16a(dst +  0, dst0xmm);
      Acc::m128iStore16a(dst + 16, dst2xmm);

      dst += 32;
    FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(C_Opaque)
  }

  // ==========================================================================
  // [PRGB32 - CBlit - PRGB32 - Line]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_cblit_prgb32_line(
    uint8_t* dst, const RasterSolid* src, int w, const RasterClosure* closure)
  {
    __m128i src0xmm;
    __m128i sra0xmm;

    Acc::m128iLoad4(src0xmm, &src->prgb32);
    Acc::m128iRShiftPI32<24>(sra0xmm, src0xmm);
    Acc::m128iExtendPI32FromSI32(src0xmm, src0xmm);
    Acc::m128iExpandPI16FromSI16(sra0xmm, sra0xmm);
    Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
    Acc::m128iLShiftPU16<8>(sra0xmm, sra0xmm);

    _prgb32_cblit_prgb32_line(dst, w, src0xmm, sra0xmm);
  }

  // ==========================================================================
  // [PRGB32 - CBlit - PRGB32 - Span]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_cblit_prgb32_span(
    uint8_t* dst, const RasterSolid* src, const RasterSpan* span, const RasterClosure* closure)
  {
    __m128i sro0xmm;

    Acc::m128iLoad4(sro0xmm, &src->prgb32);
    Acc::m128iExtendPI32FromSI32(sro0xmm, sro0xmm);

    FOG_CBLIT_SPAN8_BEGIN(4)

    // ------------------------------------------------------------------------
    // [C-Any]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_ANY()
    {
      __m128i src0xmm;
      __m128i sra0xmm;

      Acc::m128iCvtSI128FromSI(sra0xmm, msk0);
      Acc::m128iUnpackPI16FromPI8Lo(src0xmm, sro0xmm, sro0xmm);
      Acc::m128iExpandPI16FromSI16(sra0xmm, sra0xmm);
      Acc::m128iLShiftPU16<8>(src0xmm, src0xmm);
      Acc::m128iMulHiPU16(src0xmm, src0xmm, sra0xmm);

      Acc::m128iShufflePI16<3, 3, 3, 3>(sra0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(src0xmm, src0xmm, src0xmm);
      Acc::m128iShufflePI32<0, 0, 0, 0>(sra0xmm, sra0xmm);
      Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
      Acc::m128iLShiftPU16<8>(sra0xmm, sra0xmm);

      _prgb32_cblit_prgb32_line(dst, w, src0xmm, sra0xmm);
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_GLYPH()
    {
      Acc::prefetchT0(dst);

      Acc::m128iUnpackPI16FromPI8Lo(sro0xmm, sro0xmm, sro0xmm);
      Acc::m128iLShiftPU16<8>(sro0xmm, sro0xmm);

      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(A8_Glyph)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad1(src0xmm, msk);
        Acc::m128iLoad4(dst0xmm, dst);

        Acc::m128iExtendPI16FromSI16Lo(src0xmm, src0xmm);
        Acc::m128iCvt256From255PI16(src0xmm, src0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, sro0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iShufflePI16Lo<3, 3, 3, 3>(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        msk += 1;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(A8_Glyph)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(A8_Glyph)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad2(src0xmm, msk);
        Acc::m128iLoad8(dst0xmm, dst);

        Acc::m128iUnpackMask2PI8(src0xmm, src0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
        Acc::m128iCvt256From255PI16(src0xmm, src0xmm);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, sro0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iShufflePI16<3, 3, 3, 3>(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        msk += 2;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(A8_Glyph)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(A8_Glyph)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm, src1xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm;

        Acc::m128iLoad8ZX(src0xmm, msk);
        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iCvt256From255PI16(src0xmm, src0xmm);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iUnpackPI32FromPI16Hi(src1xmm, src0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(src0xmm, src0xmm, src0xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(sra0xmm, src0xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(src0xmm, src0xmm);
        Acc::m128iShufflePI32<3, 3, 2, 2>(sra1xmm, src1xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(src1xmm, src1xmm);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, sro0xmm);
        Acc::m128iMulHiPU16(sra0xmm, sra0xmm, sro0xmm);
        Acc::m128iMulHiPU16(src1xmm, src1xmm, sro0xmm);
        Acc::m128iMulHiPU16(sra1xmm, sra1xmm, sro0xmm);

        Acc::m128iPackPU8FromPU16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iPackPU8FromPU16(src1xmm, src1xmm, sra1xmm);

        Acc::m128iFill(sra0xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src0xmm);
        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst0xmm, dst0xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iFill(sra0xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);
        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);
        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst1xmm, dst1xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, src1xmm);
        
        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        msk += 8;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(A8_Glyph)

      Acc::m128iRShiftPU16<8>(sro0xmm, sro0xmm);
      Acc::m128iPackPU8FromPU16(sro0xmm, sro0xmm);
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_EXTRA()
    {
      Acc::prefetchT0(dst);

      Acc::m128iUnpackPI16FromPI8Lo(sro0xmm, sro0xmm, sro0xmm);
      Acc::m128iLShiftPU16<8>(sro0xmm, sro0xmm);

      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(A8_Extra)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad2(src0xmm, msk);
        Acc::m128iLoad4(dst0xmm, dst);

        Acc::m128iExtendPI16FromSI16Lo(src0xmm, src0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, sro0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iShufflePI16Lo<3, 3, 3, 3>(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        msk += 2;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(A8_Extra)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(A8_Extra)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad4(src0xmm, msk);
        Acc::m128iLoad8(dst0xmm, dst);

        Acc::m128iUnpackMask2PI16(src0xmm, src0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, sro0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iShufflePI16<3, 3, 3, 3>(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        msk += 4;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(A8_Extra)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(A8_Extra)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm, src1xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm;

        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iLoad8(src0xmm, msk + 0);
        Acc::m128iLoad8(src1xmm, msk + 8);

        Acc::m128iUnpackMask4PI16(src0xmm, sra0xmm, src0xmm);
        Acc::m128iUnpackMask4PI16(src1xmm, sra1xmm, src1xmm);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, sro0xmm);
        Acc::m128iMulHiPU16(sra0xmm, sra0xmm, sro0xmm);
        Acc::m128iMulHiPU16(src1xmm, src1xmm, sro0xmm);
        Acc::m128iMulHiPU16(sra1xmm, sra1xmm, sro0xmm);

        Acc::m128iPackPU8FromPU16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iPackPU8FromPU16(src1xmm, src1xmm, sra1xmm);

        Acc::m128iFill(sra0xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src0xmm);
        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst0xmm, dst0xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iFill(sra0xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);
        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);
        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst1xmm, dst1xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, src1xmm);
        
        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        msk += 16;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(A8_Extra)

      Acc::m128iRShiftPU16<8>(sro0xmm, sro0xmm);
      Acc::m128iPackPU8FromPU16(sro0xmm, sro0xmm);
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_ARGB32_GLYPH()
    {
      // TODO:
    }

    FOG_CBLIT_SPAN8_END()
  }

  // ==========================================================================
  // [PRGB32 - VBlit - PRGB32 - Helpers]
  // ==========================================================================

  static FOG_INLINE void _prgb32_vblit_prgb32_line(
    uint8_t* dst, const uint8_t* src, int w)
  {
    FOG_BLIT_LOOP_32x8_SSE2_INIT()

    FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(C_Opaque)
      __m128i dst0xmm;
      __m128i src0xmm;
      __m128i sra0xmm;

      Acc::m128iLoad4ZX(src0xmm, src);
      Acc::m128iLoad4ZX(dst0xmm, dst);
      Acc::m128iShufflePI16Lo<3, 3, 3, 3>(sra0xmm, src0xmm);
      Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
      Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
      Acc::m128iStore4(dst, dst0xmm);

      dst += 4;
      src += 4;
    FOG_BLIT_LOOP_32x8_SSE2_ONE_END(C_Opaque)

    FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(C_Opaque)
      __m128i dst0xmm;
      __m128i src0xmm;
      __m128i sra0xmm;

      Acc::m128iLoad8ZX(src0xmm, src);
      Acc::m128iLoad8ZX(dst0xmm, dst);
      Acc::m128iShufflePI16<3, 3, 3, 3>(sra0xmm, src0xmm);
      Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
      Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
      Acc::m128iStore8(dst, dst0xmm);

      dst += 8;
      src += 8;
    FOG_BLIT_LOOP_32x8_SSE2_TWO_END(C_Opaque)

    FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(C_Opaque)
      __m128i dst0xmm, dst1xmm;
      __m128i src0xmm, src1xmm;
      __m128i sra0xmm, sra1xmm;
      __m128i tmp0xmm;

      Acc::m128iLoad16u(src0xmm, src +  0);
      Acc::m128iLoad16a(dst0xmm, dst +  0);

      Acc::m128iFill(sra0xmm);
      Acc::m128iLoad16u(src1xmm, src + 16);
      Acc::m128iLoad16a(dst1xmm, dst + 16);

      Acc::m128iXor(sra0xmm, sra0xmm, src0xmm);
      Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
      Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

      Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
      Acc::m128iMulDiv255PI16_2x(dst0xmm, dst0xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

      Acc::m128iFill(sra0xmm);
      Acc::m128iXor(sra0xmm, sra0xmm, src1xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

      Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
      Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);

      Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
      Acc::m128iMulDiv255PI16_2x(dst1xmm, dst1xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

      Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);
      Acc::m128iAddPI32(dst1xmm, dst1xmm, src1xmm);

      Acc::m128iStore16a(dst +  0, dst0xmm);
      Acc::m128iStore16a(dst + 16, dst1xmm);

      dst += 32;
      src += 32;
    FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(C_Opaque)
  }

  // ==========================================================================
  // [PRGB32 - VBlit - PRGB32 - Line]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_vblit_prgb32_line(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    _prgb32_vblit_prgb32_line(dst, src, w);
  }

  // ==========================================================================
  // [PRGB32 - VBlit - PRGB32 - Span]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_vblit_prgb32_span(
    uint8_t* dst, const RasterSpan* span, const RasterClosure* closure)
  {
    FOG_VBLIT_SPAN8_BEGIN(4)

    // ------------------------------------------------------------------------
    // [C-Opaque]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_C_OPAQUE()
    {
      _prgb32_vblit_prgb32_line(dst, src, w);
    }

    // ------------------------------------------------------------------------
    // [C-Mask]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_C_MASK()
    {
      __m128i msk0xmm;

      Acc::m128iCvtSI128FromSI(msk0xmm, msk0);
      Acc::m128iLShiftPU16<8>(msk0xmm, msk0xmm);
      Acc::m128iExpandPI16FromSI16(msk0xmm, msk0xmm);

      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(C_Mask)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad4ZX(src0xmm, src);
        Acc::m128iLoad4ZX(dst0xmm, dst);
        Acc::m128iMulHiPU16(src0xmm, src0xmm, msk0xmm);
        Acc::m128iShufflePI16Lo<3, 3, 3, 3>(sra0xmm, src0xmm);
        Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        src += 4;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(C_Mask)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(C_Mask)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad8ZX(src0xmm, src);
        Acc::m128iLoad8ZX(dst0xmm, dst);
        Acc::m128iMulHiPU16(src0xmm, src0xmm, msk0xmm);
        Acc::m128iShufflePI16<3, 3, 3, 3>(sra0xmm, src0xmm);
        Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        src += 8;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(C_Mask)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(C_Mask)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm, src1xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm;

        Acc::m128iLoad8ZX(src0xmm, src +  0);
        Acc::m128iLoad8ZX(sra0xmm, src +  8);

        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, msk0xmm);
        Acc::m128iLoad8ZX(src1xmm, src + 16);

        Acc::m128iMulHiPU16(sra0xmm, sra0xmm, msk0xmm);
        Acc::m128iLoad8ZX(sra1xmm, src + 24);

        Acc::m128iMulHiPU16(src1xmm, src1xmm, msk0xmm);
        Acc::m128iPackPU8FromPU16(src0xmm, src0xmm, sra0xmm);

        Acc::m128iMulHiPU16(sra1xmm, sra1xmm, msk0xmm);
        Acc::m128iFill(sra0xmm);

        Acc::m128iPackPU8FromPU16(src1xmm, src1xmm, sra1xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst0xmm, dst0xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iFill(sra0xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);

        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst1xmm, dst1xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, src1xmm);

        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        src += 32;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(C_Mask)
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_A8_GLYPH()
    {
      // TODO:
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_A8_EXTRA()
    {
      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(A8_Extra)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad4ZX(src0xmm, src);
        Acc::m128iLoad2ExpandLo(sra0xmm, msk);
        Acc::m128iLoad4ZX(dst0xmm, dst);

        Acc::m128iMulDiv256PI16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iShufflePI16Lo<3, 3, 3, 3>(sra0xmm, src0xmm);
        Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        src += 4;
        msk += 2;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(A8_Extra)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(A8_Extra)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad8ZX(src0xmm, src);
        Acc::m128iLoad4(sra0xmm, msk);
        Acc::m128iLoad8ZX(dst0xmm, dst);

        Acc::m128iUnpackMask2PI16(sra0xmm, sra0xmm);
        Acc::m128iMulDiv256PI16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iShufflePI16<3, 3, 3, 3>(sra0xmm, src0xmm);
        Acc::m128iNegate255PI16(sra0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        src += 8;
        msk += 4;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(A8_Extra)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(A8_Extra)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm, src1xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm;

        Acc::m128iLoad8ZX(src0xmm, src +  0);
        Acc::m128iLoad8ZX(sra0xmm, src +  8);

        Acc::m128iLoad8(dst0xmm, msk + 0);
        Acc::m128iLoad8(dst1xmm, msk + 8);

        Acc::m128iUnpackMask4PI16(dst0xmm, tmp0xmm, dst0xmm);
        Acc::m128iMulDiv256PI16(src0xmm, src0xmm, dst0xmm);
        Acc::m128iMulDiv256PI16(sra0xmm, sra0xmm, tmp0xmm);

        Acc::m128iUnpackMask4PI16(dst1xmm, tmp0xmm, dst1xmm);
        Acc::m128iLoad8ZX(src1xmm, src + 16);
        Acc::m128iLoad8ZX(sra1xmm, src + 24);
        
        Acc::m128iMulDiv256PI16(src1xmm, src1xmm, dst1xmm);
        Acc::m128iMulDiv256PI16(sra1xmm, sra1xmm, tmp0xmm);

        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iPackPU8FromPU16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iFill(sra0xmm);

        Acc::m128iPackPU8FromPU16(src1xmm, src1xmm, sra1xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst0xmm, dst0xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iFill(sra0xmm);
        Acc::m128iXor(sra0xmm, sra0xmm, src1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);

        Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
        Acc::m128iMulDiv255PI16_2x(dst1xmm, dst1xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, src1xmm);

        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        src += 32;
        msk += 16;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(A8_Extra)
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_ARGB32_GLYPH()
    {
      // TODO:
    }

    FOG_VBLIT_SPAN8_END()
  }

  // ==========================================================================
  // [PRGB32 - VBlit - A8 - Helpers]
  // ==========================================================================

  static FOG_INLINE void _prgb32_vblit_a8_line(
    uint8_t* dst, const uint8_t* src, int w)
  {
    FOG_BLIT_LOOP_32x8_SSE2_INIT()

    FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(C_Opaque)
      __m128i dst0xmm;
      __m128i src0xmm;
      __m128i sra0xmm;

      Acc::m128iLoad1(src0xmm, src);
      Acc::m128iLoad4ZX(dst0xmm, dst);
      Acc::m128iExtendPI16FromSI16Lo(src0xmm, src0xmm);
      Acc::m128iNegate255PI16(sra0xmm, src0xmm);
      Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
      Acc::m128iStore4(dst, dst0xmm);

      dst += 4;
      src += 1;
    FOG_BLIT_LOOP_32x8_SSE2_ONE_END(C_Opaque)

    FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(C_Opaque)
      __m128i dst0xmm;
      __m128i src0xmm;
      __m128i sra0xmm;

      Acc::m128iLoad2(src0xmm, src);
      Acc::m128iLoad8ZX(dst0xmm, dst);
      Acc::m128iUnpackMask2PI8(src0xmm, src0xmm);
      Acc::m128iNegate255PI16(sra0xmm, src0xmm);
      Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
      Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
      Acc::m128iStore8(dst, dst0xmm);

      dst += 8;
      src += 2;
    FOG_BLIT_LOOP_32x8_SSE2_TWO_END(C_Opaque)

    FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(C_Opaque)
      __m128i dst0xmm, dst1xmm;
      __m128i src0xmm, src1xmm;
      __m128i sra0xmm, sra1xmm;
      __m128i tmp0xmm;

      Acc::m128iLoad4(src0xmm, src + 0);
      Acc::m128iLoad4(src1xmm, src + 4);

      Acc::m128iLoad16a(dst0xmm, dst +  0);
      Acc::m128iLoad16a(dst1xmm, dst + 16);

      Acc::m128iFill(sra0xmm);

      Acc::m128iUnpackPI16FromPI8Lo(src0xmm, src0xmm, src0xmm);
      Acc::m128iUnpackPI16FromPI8Lo(src1xmm, src1xmm, src1xmm);

      Acc::m128iUnpackPI32FromPI16Lo(src0xmm, src0xmm, src0xmm);
      Acc::m128iUnpackPI32FromPI16Lo(src1xmm, src1xmm, src1xmm);

      Acc::m128iXor(sra0xmm, sra0xmm, src0xmm);
      Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
      Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);

      Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
      Acc::m128iMulDiv255PI16_2x(dst0xmm, dst0xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

      Acc::m128iFill(sra0xmm);
      Acc::m128iXor(sra0xmm, sra0xmm, src1xmm);
      Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

      Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
      Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);

      Acc::m128iUnpackAlphaPI16FromARGB32_PI8(sra0xmm, sra1xmm, sra0xmm);
      Acc::m128iMulDiv255PI16_2x(dst1xmm, dst1xmm, sra0xmm, tmp0xmm, tmp0xmm, sra1xmm);

      Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
      Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);
      Acc::m128iAddPI32(dst1xmm, dst1xmm, src1xmm);

      Acc::m128iStore16a(dst +  0, dst0xmm);
      Acc::m128iStore16a(dst + 16, dst1xmm);

      dst += 32;
      src += 8;
    FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(C_Opaque)
  }

  // ==========================================================================
  // [PRGB32 - VBlit - A8 - Line]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_vblit_a8_line(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    _prgb32_vblit_a8_line(dst, src, w);
  }

  // ==========================================================================
  // [PRGB32 - VBlit - A8 - Span]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_vblit_a8_span(
    uint8_t* dst, const RasterSpan* span, const RasterClosure* closure)
  {
    FOG_VBLIT_SPAN8_BEGIN(4)

    // ------------------------------------------------------------------------
    // [C-Opaque]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_C_OPAQUE()
    {
      _prgb32_vblit_a8_line(dst, src, w);
    }

    // ------------------------------------------------------------------------
    // [C-Mask]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_C_MASK()
    {
      __m128i msk0xmm;

      Acc::m128iCvtSI128FromSI(msk0xmm, msk0);
      Acc::m128iLShiftPU16<8>(msk0xmm, msk0xmm);
      Acc::m128iExpandPI16FromSI16(msk0xmm, msk0xmm);

      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(C_Mask)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad1(src0xmm, src);
        Acc::m128iLoad4ZX(dst0xmm, dst);
        Acc::m128iExtendPI16FromSI16(src0xmm, src0xmm);
        Acc::m128iMulHiPU16(src0xmm, src0xmm, msk0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        src += 1;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(C_Mask)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(C_Mask)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad2(src0xmm, src);
        Acc::m128iLoad8ZX(dst0xmm, dst);
        Acc::m128iUnpackMask2PI8(src0xmm, src0xmm);
        Acc::m128iMulHiPU16(src0xmm, src0xmm, msk0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        src += 2;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(C_Mask)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(C_Mask)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm, tmp1xmm;

        Acc::m128iLoad8ZX(src0xmm, src);
        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iMulHiPU16(src0xmm, src0xmm, msk0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulLoPI16(src0xmm, src0xmm, FOG_XMM_GET_CONST_PI(0101010101010101_0101010101010101));

        Acc::m128iCopy(sra1xmm, sra0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(sra0xmm, sra0xmm, sra0xmm);
        Acc::m128iUnpackPI32FromPI16Hi(sra1xmm, sra1xmm, sra1xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(tmp1xmm, sra0xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(sra0xmm, sra0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(tmp0xmm, tmp0xmm, tmp1xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(tmp1xmm, sra1xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(sra1xmm, sra1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);
        Acc::m128iMulDiv255PI16(dst1xmm, dst1xmm, sra1xmm);
        Acc::m128iMulDiv255PI16(tmp0xmm, tmp0xmm, tmp1xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);

        Acc::m128iCopy(tmp0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(src0xmm, src0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Hi(tmp0xmm, tmp0xmm, tmp0xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, tmp0xmm);

        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        src += 8;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(C_Mask)
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_A8_GLYPH()
    {
      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(A8_Glyph)
        uint32_t src0p;
        uint32_t msk0p;

        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::p32Load1b(msk0p, msk);
        Acc::p32Load1b(src0p, src);
        Acc::p32Cvt256SBWFrom255SBW(msk0p, msk0p);

        Acc::m128iLoad4ZX(dst0xmm, dst);
        Acc::p32Mul(src0p, src0p, msk0p);

        Acc::m128iCvtSI128FromSI(src0xmm, src0p);
        Acc::m128iExpandPI16FromSI16Lo(src0xmm, src0xmm);
        Acc::m128iRShiftPU16<8>(src0xmm, src0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);

        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        src += 1;
        msk += 1;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(A8_Glyph)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(A8_Glyph)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad2(sra0xmm, msk);
        Acc::m128iLoad2(src0xmm, src);

        Acc::m128iUnpackMask2PI8(sra0xmm, sra0xmm);
        Acc::m128iUnpackMask2PI8(src0xmm, src0xmm);
        Acc::m128iCvt256From255PI16(sra0xmm, sra0xmm);

        Acc::m128iLoad8ZX(dst0xmm, dst);
        Acc::m128iMulDiv256PI16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        src += 2;
        msk += 2;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(A8_Glyph)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(A8_Glyph)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm, tmp1xmm;

        Acc::m128iLoad8(tmp0xmm, src);
        Acc::m128iLoad8ZX(sra0xmm, msk);

        Acc::m128iZero(src0xmm);
        Acc::m128iCvt256From255PI16(sra0xmm, sra0xmm);

        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iUnpackPI16FromPI8Lo(src0xmm, src0xmm, tmp0xmm);
        Acc::m128iMulHiPU16(src0xmm, src0xmm, sra0xmm);

        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulLoPI16(src0xmm, src0xmm, FOG_XMM_GET_CONST_PI(0101010101010101_0101010101010101));

        Acc::m128iCopy(sra1xmm, sra0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(sra0xmm, sra0xmm, sra0xmm);
        Acc::m128iUnpackPI32FromPI16Hi(sra1xmm, sra1xmm, sra1xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(tmp1xmm, sra0xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(sra0xmm, sra0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(tmp0xmm, tmp0xmm, tmp1xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(tmp1xmm, sra1xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(sra1xmm, sra1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);
        Acc::m128iMulDiv255PI16(dst1xmm, dst1xmm, sra1xmm);
        Acc::m128iMulDiv255PI16(tmp0xmm, tmp0xmm, tmp1xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);

        Acc::m128iCopy(tmp0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(src0xmm, src0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Hi(tmp0xmm, tmp0xmm, tmp0xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, tmp0xmm);

        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        src += 8;
        msk += 8;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(A8_Glyph)
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_A8_EXTRA()
    {
      FOG_BLIT_LOOP_32x8_SSE2_INIT()

      FOG_BLIT_LOOP_32x8_SSE2_ONE_BEGIN(A8_Extra)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad1(src0xmm, src);
        Acc::m128iLoad2(sra0xmm, msk);
        Acc::m128iLoad4ZX(dst0xmm, dst);

        Acc::m128iExpandPI16FromSI16Lo(src0xmm, src0xmm);
        Acc::m128iExpandPI16FromSI16Lo(sra0xmm, sra0xmm);
        Acc::m128iMulDiv256PI16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore4(dst, dst0xmm);

        dst += 4;
        src += 1;
        msk += 2;
      FOG_BLIT_LOOP_32x8_SSE2_ONE_END(A8_Extra)

      FOG_BLIT_LOOP_32x8_SSE2_TWO_BEGIN(A8_Extra)
        __m128i dst0xmm;
        __m128i src0xmm;
        __m128i sra0xmm;

        Acc::m128iLoad2(src0xmm, src);
        Acc::m128iLoad4(sra0xmm, msk);
        Acc::m128iLoad8ZX(dst0xmm, dst);

        Acc::m128iUnpackMask2PI8(src0xmm, src0xmm);
        Acc::m128iUnpackMask2PI16(sra0xmm, sra0xmm);
        Acc::m128iMulDiv256PI16(src0xmm, src0xmm, sra0xmm);
        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm);
        Acc::m128iStore8(dst, dst0xmm);

        dst += 8;
        src += 2;
        msk += 4;
      FOG_BLIT_LOOP_32x8_SSE2_TWO_END(A8_Extra)

      FOG_BLIT_LOOP_32x8_SSE2_MAIN_BEGIN(A8_Extra)
        __m128i dst0xmm, dst1xmm;
        __m128i src0xmm;
        __m128i sra0xmm, sra1xmm;
        __m128i tmp0xmm, tmp1xmm;

        Acc::m128iLoad8(tmp0xmm, src);
        Acc::m128iLoad16uLoHi(sra0xmm, msk);

        Acc::m128iZero(src0xmm);
        Acc::m128iLoad16a(dst0xmm, dst +  0);
        Acc::m128iLoad16a(dst1xmm, dst + 16);

        Acc::m128iUnpackPI16FromPI8Lo(src0xmm, src0xmm, tmp0xmm);
        Acc::m128iMulHiPU16(src0xmm, src0xmm, sra0xmm);

        Acc::m128iNegate255PI16(sra0xmm, src0xmm);
        Acc::m128iMulLoPI16(src0xmm, src0xmm, FOG_XMM_GET_CONST_PI(0101010101010101_0101010101010101));

        Acc::m128iCopy(sra1xmm, sra0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(sra0xmm, sra0xmm, sra0xmm);
        Acc::m128iUnpackPI32FromPI16Hi(sra1xmm, sra1xmm, sra1xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(tmp1xmm, sra0xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(sra0xmm, sra0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst0xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst0xmm, dst0xmm);
        Acc::m128iMulDiv255PI16(dst0xmm, dst0xmm, sra0xmm);
        Acc::m128iMulDiv255PI16(tmp0xmm, tmp0xmm, tmp1xmm);

        Acc::m128iShufflePI32<3, 3, 2, 2>(tmp1xmm, sra1xmm);
        Acc::m128iShufflePI32<1, 1, 0, 0>(sra1xmm, sra1xmm);
        Acc::m128iPackPU8FromPU16(dst0xmm, dst0xmm, tmp0xmm);

        Acc::m128iUnpackPI16FromPI8Hi(tmp0xmm, dst1xmm);
        Acc::m128iUnpackPI16FromPI8Lo(dst1xmm, dst1xmm);
        Acc::m128iMulDiv255PI16(dst1xmm, dst1xmm, sra1xmm);
        Acc::m128iMulDiv255PI16(tmp0xmm, tmp0xmm, tmp1xmm);
        Acc::m128iPackPU8FromPU16(dst1xmm, dst1xmm, tmp0xmm);

        Acc::m128iCopy(tmp0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Lo(src0xmm, src0xmm, src0xmm);
        Acc::m128iUnpackPI32FromPI16Hi(tmp0xmm, tmp0xmm, tmp0xmm);

        Acc::m128iAddPI32(dst0xmm, dst0xmm, src0xmm);
        Acc::m128iAddPI32(dst1xmm, dst1xmm, tmp0xmm);

        Acc::m128iStore16a(dst +  0, dst0xmm);
        Acc::m128iStore16a(dst + 16, dst1xmm);

        dst += 32;
        src += 8;
        msk += 16;
      FOG_BLIT_LOOP_32x8_SSE2_MAIN_END(A8_Extra)
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_VBLIT_SPAN8_ARGB32_GLYPH()
    {
      // TODO:
    }

    FOG_VBLIT_SPAN8_END()
  }
};

} // RasterOps_SSE2 namespace
} // Fog namespace

// [Guard]
#endif // _FOG_G2D_PAINTING_RASTEROPS_SSE2_COMPOSITESRCOVER_P_H
