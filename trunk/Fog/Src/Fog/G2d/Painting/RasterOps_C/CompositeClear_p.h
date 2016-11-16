// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_C_COMPOSITECLEAR_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_C_COMPOSITECLEAR_P_H

// [Dependencies]
#include <Fog/G2d/Painting/RasterOps_C/CompositeBase_p.h>

namespace Fog {
namespace RasterOps_C {

// ============================================================================
// [Fog::RasterOps_C - CompositeClear]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT CompositeClear
{
  enum { COMBINE_FLAGS = RASTER_COMBINE_OP_CLEAR };

  // ==========================================================================
  // [PRGB32 - Line]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_xblit_line(
    uint8_t* dst, const void* src, int w, const RasterClosure* closure)
  {
    FOG_BLIT_LOOP_32x1_INIT()

    uint32_t src0p;
    Acc::p32Copy(src0p, 0x00000000U);

    FOG_BLIT_LOOP_32x1_BEGIN(C_Opaque)
      Acc::p32Store4a(dst, src0p);
      dst += 4;
    FOG_BLIT_LOOP_32x1_END(C_Opaque)
  }

  // ==========================================================================
  // [PRGB32 - Span]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_cblit_span(
    uint8_t* dst, const void* src_dummy, const RasterSpan* span, const RasterClosure* closure)
  {
    FOG_CBLIT_SPAN8_BEGIN(4)

    // ------------------------------------------------------------------------
    // [C-Opaque]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_OPAQUE()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      uint32_t src0p;
      Acc::p32Copy(src0p, 0x00000000);

      FOG_BLIT_LOOP_32x1_BEGIN(C_Opaque)
        Acc::p32Store4a(dst, src0p);
        dst += 4;
      FOG_BLIT_LOOP_32x1_END(C_Opaque)
    }

    // ------------------------------------------------------------------------
    // [C-Mask]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_MASK()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      uint32_t msk0p;
      Acc::p32Copy(msk0p, msk0);
      Acc::p32Negate256SBW(msk0p, msk0p);

      FOG_BLIT_LOOP_32x1_BEGIN(C_Mask)
        uint32_t dst0p;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32MulDiv256PBB_SBW(dst0p, dst0p, msk0p);
        Acc::p32Store4a(dst, dst0p);

        dst += 4;
      FOG_BLIT_LOOP_32x1_END(C_Mask)
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_GLYPH()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      FOG_BLIT_LOOP_32x1_BEGIN(A8_Glyph)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load1b(msk0p, msk);

        if (msk0p == 0x00) goto _A8_Glyph_Skip;
        Acc::p32Negate255SBW(msk0p, msk0p);
        if (msk0p == 0x00) goto _A8_Glyph_Fill;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32Cvt256SBWFrom255SBW(msk0p, msk0p);
        Acc::p32MulDiv256PBB_SBW(msk0p, dst0p, msk0p);

_A8_Glyph_Fill:
        Acc::p32Store4a(dst, msk0p);

_A8_Glyph_Skip:
        dst += 4;
        msk += 1;
      FOG_BLIT_LOOP_32x1_END(A8_Glyph)
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_EXTRA()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      FOG_BLIT_LOOP_32x1_BEGIN(A8_Extra)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32Load2a(msk0p, msk);

        Acc::p32Negate256SBW(msk0p, msk0p);
        Acc::p32MulDiv256PBB_SBW(dst0p, dst0p, msk0p);
        Acc::p32Store4a(dst, dst0p);

        dst += 4;
        msk += 2;
      FOG_BLIT_LOOP_32x1_END(A8_Extra)
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_ARGB32_GLYPH()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      FOG_BLIT_LOOP_32x1_BEGIN(ARGB32_Glyph)
        uint32_t dst0p;
        uint32_t msk0p_20, msk0p_31;

        Acc::p32Load4a(msk0p_20, msk);

        if (msk0p_20 == 0x00000000) goto _ARGB32_Glyph_Skip;
        Acc::p32Negate255PBB(msk0p_20, msk0p_20);
        if (msk0p_20 == 0x00000000) goto _ARGB32_Glyph_Fill;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32UnpackPBWFromPBB_2031(msk0p_20, msk0p_31, msk0p_20);
        Acc::p32Cvt256PBWFrom255PBW_2x(msk0p_20, msk0p_20, msk0p_31, msk0p_31);
        Acc::p32MulDiv256PBB_PBW_2031(msk0p_20, dst0p, msk0p_20, msk0p_31);

_ARGB32_Glyph_Fill:
        Acc::p32Store4a(dst, msk0p_20);

_ARGB32_Glyph_Skip:
        dst += 4;
        msk += 4;
      FOG_BLIT_LOOP_32x1_END(ARGB32_Glyph)
    }

    FOG_CBLIT_SPAN8_END()
  }

  static void FOG_FASTCALL prgb32_vblit_span(
    uint8_t* dst, const RasterSpan* span, const RasterClosure* closure)
  {
    prgb32_cblit_span(dst, NULL, span, closure);
  }

  // ==========================================================================
  // [XRGB32 - Line]
  // ==========================================================================

  static void FOG_FASTCALL xrgb32_xblit_line(
    uint8_t* dst, const void* src, int w, const RasterClosure* closure)
  {
    FOG_BLIT_LOOP_32x1_INIT()

    uint32_t src0p;
    Acc::p32Copy(src0p, 0xFF000000U);

    FOG_BLIT_LOOP_32x1_BEGIN(C_Opaque)
      Acc::p32Store4a(dst, src0p);
      dst += 4;
    FOG_BLIT_LOOP_32x1_END(C_Opaque)
  }

  // ==========================================================================
  // [XRGB32 - Span]
  // ==========================================================================

  static void FOG_FASTCALL xrgb32_cblit_span(
    uint8_t* dst, const void* src_dummy, const RasterSpan* span, const RasterClosure* closure)
  {
    FOG_CBLIT_SPAN8_BEGIN(4)

    // ------------------------------------------------------------------------
    // [C-Opaque]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_OPAQUE()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      uint32_t src0p;
      Acc::p32Copy(src0p, 0xFF000000U);

      FOG_BLIT_LOOP_32x1_BEGIN(C_Opaque)
        Acc::p32Store4a(dst, src0p);
        dst += 4;
      FOG_BLIT_LOOP_32x1_END(C_Opaque)
    }

    // ------------------------------------------------------------------------
    // [C-Mask]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_MASK()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      uint32_t msk0p;

      Acc::p32Copy(msk0p, msk0);
      Acc::p32Negate256SBW(msk0p, msk0p);

      FOG_BLIT_LOOP_32x1_BEGIN(CAExtended)
        uint32_t dst0p;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32MulDiv256PBB_SBW_Z210(dst0p, dst0p, msk0p);
        Acc::p32FillPBB3(dst0p, dst0p);
        Acc::p32Store4a(dst, dst0p);

        dst += 4;
      FOG_BLIT_LOOP_32x1_END(CAExtended)
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_GLYPH()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      FOG_BLIT_LOOP_32x1_BEGIN(A8_Glyph)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load1b(msk0p, msk);

        if (msk0p == 0x00) goto _A8_Glyph_Skip;
        Acc::p32Negate255SBW(msk0p, msk0p);
        if (msk0p == 0x00) goto _A8_Glyph_Fill;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32Cvt256SBWFrom255SBW(msk0p, msk0p);
        Acc::p32MulDiv256PBB_SBW(msk0p, dst0p, msk0p);

_A8_Glyph_Fill:
        Acc::p32FillPBB3(msk0p, msk0p);
        Acc::p32Store4a(dst, msk0p);

_A8_Glyph_Skip:
        dst += 4;
        msk += 1;
      FOG_BLIT_LOOP_32x1_END(A8_Glyph)
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_EXTRA()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      FOG_BLIT_LOOP_32x1_BEGIN(A8_Extra)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32Load2a(msk0p, msk);

        Acc::p32Negate256SBW(msk0p, msk0p);
        Acc::p32MulDiv256PBB_SBW_FillPBB3(dst0p, dst0p, msk0p);
        Acc::p32Store4a(dst, dst0p);

        dst += 4;
        msk += 2;
      FOG_BLIT_LOOP_32x1_END(A8_Extra)
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_ARGB32_GLYPH()
    {
      FOG_BLIT_LOOP_32x1_INIT()

      FOG_BLIT_LOOP_32x1_BEGIN(ARGB32_Glyph)
        uint32_t dst0p;
        uint32_t msk0p_20, msk0p_31;

        Acc::p32Load4a(msk0p_20, msk);

        if (msk0p_20 == 0x00000000) goto _ARGB32_Glyph_Skip;
        Acc::p32Negate255PBB(msk0p_20, msk0p_20);
        if (msk0p_20 == 0x00000000) goto _ARGB32_Glyph_Fill;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32UnpackPBWFromPBB_2031(msk0p_20, msk0p_31, msk0p_20);
        Acc::p32Cvt256PBWFrom255PBW_2x(msk0p_20, msk0p_20, msk0p_31, msk0p_31);
        Acc::p32MulDiv256PBB_PBW_20Z1(msk0p_20, dst0p, msk0p_20, msk0p_31);

_ARGB32_Glyph_Fill:
        Acc::p32FillPBB3(msk0p_20, msk0p_20);
        Acc::p32Store4a(dst, msk0p_20);

_ARGB32_Glyph_Skip:
        dst += 4;
        msk += 4;
      FOG_BLIT_LOOP_32x1_END(ARGB32_Glyph)
    }

    FOG_CBLIT_SPAN8_END()
  }

  static void FOG_FASTCALL xrgb32_vblit_span(
    uint8_t* dst, const RasterSpan* span, const RasterClosure* closure)
  {
    xrgb32_cblit_span(dst, NULL, span, closure);
  }

  // ==========================================================================
  // [RGB24 - Line]
  // ==========================================================================

  static void FOG_FASTCALL rgb24_xblit_line(
    uint8_t* dst, const void* src, int w, const RasterClosure* closure)
  {
    w *= 3;
    FOG_BLIT_LOOP_8x4_INIT()

    uint32_t src0p;
    Acc::p32Copy(src0p, 0x00000000);

    FOG_BLIT_LOOP_8x4_SMALL_BEGIN(C_Opaque)
      Acc::p32Store1b(dst, src0p);
      dst += 1;
    FOG_BLIT_LOOP_8x4_SMALL_END(C_Opaque)

    FOG_BLIT_LOOP_8x4_MAIN_BEGIN(C_Opaque)
      Acc::p32Store4a(dst, src0p);
      dst += 4;
    FOG_BLIT_LOOP_8x4_MAIN_END(C_Opaque)
  }

  // ==========================================================================
  // [RGB24 - Span]
  // ==========================================================================

  static void FOG_FASTCALL rgb24_cblit_span(
    uint8_t* dst, const void* src_dummy, const RasterSpan* span, const RasterClosure* closure)
  {
    FOG_CBLIT_SPAN8_BEGIN(3)

    // ------------------------------------------------------------------------
    // [C-Opaque]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_OPAQUE()
    {
      w *= 3;
      FOG_BLIT_LOOP_8x4_INIT()

      uint32_t src0p;
      Acc::p32Copy(src0p, 0x00000000);

      FOG_BLIT_LOOP_8x4_SMALL_BEGIN(C_Opaque)
        Acc::p32Store1b(dst, src0p);
        dst += 1;
      FOG_BLIT_LOOP_8x4_SMALL_END(C_Opaque)

      FOG_BLIT_LOOP_8x4_MAIN_BEGIN(C_Opaque)
        Acc::p32Store4a(dst, src0p);
        dst += 4;
      FOG_BLIT_LOOP_8x4_MAIN_END(C_Opaque)
    }

    // ------------------------------------------------------------------------
    // [C-Mask]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_MASK()
    {
      w *= 3;
      FOG_BLIT_LOOP_8x4_INIT()

      uint32_t msk0p;
      Acc::p32Copy(msk0p, msk0);
      Acc::p32Negate256SBW(msk0p, msk0p);

      FOG_BLIT_LOOP_8x4_SMALL_BEGIN(C_Mask)
        uint32_t dst0p;

        Acc::p32Load1b(dst0p, dst);
        Acc::p32MulDiv256SBW(dst0p, dst0p, msk0p);
        Acc::p32Store1b(dst, dst0p);

        dst += 1;
      FOG_BLIT_LOOP_8x4_SMALL_END(C_Mask)

      FOG_BLIT_LOOP_8x4_MAIN_BEGIN(C_Mask)
        uint32_t dst0p;

        Acc::p32Load4a(dst0p, dst);
        Acc::p32MulDiv256PBB_SBW(dst0p, dst0p, msk0p);
        Acc::p32Store4a(dst, dst0p);

        dst += 4;
      FOG_BLIT_LOOP_8x4_MAIN_END(C_Mask)
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_GLYPH()
    {
      FOG_BLIT_LOOP_24x1_INIT()

      FOG_BLIT_LOOP_24x1_BEGIN(A8_Glyph)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load1b(msk0p, msk);
        if (msk0p == 0x00) goto _A8_Glyph_Skip;
        Acc::p32Negate255SBW(msk0p, msk0p);
        if (msk0p == 0x00) goto _A8_Glyph_Fill;

        Acc::p32Load3b(dst0p, dst);
        Acc::p32Cvt256SBWFrom255SBW(msk0p, msk0p);
        Acc::p32MulDiv256PBB_SBW_Z210(msk0p, dst0p, msk0p);

_A8_Glyph_Fill:
        Acc::p32Store3b(dst, msk0p);

_A8_Glyph_Skip:
        dst += 3;
        msk += 1;
      FOG_BLIT_LOOP_24x1_END(A8_Glyph)
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_EXTRA()
    {
      FOG_BLIT_LOOP_24x1_INIT()

      FOG_BLIT_LOOP_24x1_BEGIN(A8_Extra)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load3b(dst0p, dst);
        Acc::p32Load2a(msk0p, msk);

        Acc::p32Negate256SBW(msk0p, msk0p);
        Acc::p32MulDiv256PBB_SBW_Z210(dst0p, dst0p, msk0p);
        Acc::p32Store3b(dst, dst0p);

        dst += 3;
        msk += 2;
      FOG_BLIT_LOOP_24x1_END(A8_Extra)
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_ARGB32_GLYPH()
    {
      FOG_BLIT_LOOP_24x1_INIT()

      FOG_BLIT_LOOP_24x1_BEGIN(ARGB32_Glyph)
        uint32_t dst0p;
        uint32_t msk0p_20, msk0p_31;

        Acc::p32Load4a(msk0p_20, msk);

        if (msk0p_20 == 0x00000000) goto _ARGB32_Glyph_Skip;
        Acc::p32Negate255PBB(msk0p_20, msk0p_20);
        if (msk0p_20 == 0x00000000) goto _ARGB32_Glyph_Fill;

        Acc::p32Load3b(dst0p, dst);
        Acc::p32UnpackPBWFromPBB_2031(msk0p_20, msk0p_31, msk0p_20);
        Acc::p32Cvt256PBWFrom255PBW_2x(msk0p_20, msk0p_20, msk0p_31, msk0p_31);
        Acc::p32MulDiv256PBB_PBW_20Z1(msk0p_20, dst0p, msk0p_20, msk0p_31);

_ARGB32_Glyph_Fill:
        Acc::p32Store3b(dst, msk0p_20);

_ARGB32_Glyph_Skip:
        dst += 3;
        msk += 4;
      FOG_BLIT_LOOP_24x1_END(ARGB32_Glyph)
    }

    FOG_CBLIT_SPAN8_END()
  }

  static void FOG_FASTCALL rgb24_vblit_span(
    uint8_t* dst, const RasterSpan* span, const RasterClosure* closure)
  {
    rgb24_cblit_span(dst, NULL, span, closure);
  }

  // ==========================================================================
  // [A8 - Line]
  // ==========================================================================

  static void FOG_FASTCALL a8_xblit_line(
    uint8_t* dst, const void* src, int w, const RasterClosure* closure)
  {
    FOG_BLIT_LOOP_8x8_INIT()

    uint32_t src0p;
    Acc::p32Copy(src0p, 0x00000000);

    FOG_BLIT_LOOP_8x8_SMALL_BEGIN(C_Opaque)
      Acc::p32Store1b(dst, src0p);
      dst += 1;
    FOG_BLIT_LOOP_8x8_SMALL_END(C_Opaque)

    FOG_BLIT_LOOP_8x8_MAIN_BEGIN(C_Opaque)
      Acc::p32Store4a(dst + 0, src0p);
      Acc::p32Store4a(dst + 4, src0p);
      dst += 8;
    FOG_BLIT_LOOP_8x8_MAIN_END(C_Opaque)
  }

  // ==========================================================================
  // [A8 - Span]
  // ==========================================================================

  static void FOG_FASTCALL a8_cblit_span(
    uint8_t* dst, const void* src, const RasterSpan* span, const RasterClosure* closure)
  {
    FOG_CBLIT_SPAN8_BEGIN(1)

    // ------------------------------------------------------------------------
    // [C-Opaque]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_OPAQUE()
    {
      FOG_BLIT_LOOP_8x8_INIT()

      uint32_t src0p;
      Acc::p32Copy(src0p, 0x00000000);

      FOG_BLIT_LOOP_8x8_SMALL_BEGIN(C_Opaque)
        Acc::p32Store1b(dst, src0p);
        dst += 1;
      FOG_BLIT_LOOP_8x8_SMALL_END(C_Opaque)

      FOG_BLIT_LOOP_8x8_MAIN_BEGIN(C_Opaque)
        Acc::p32Store4a(dst + 0, src0p);
        Acc::p32Store4a(dst + 4, src0p);
        dst += 8;
      FOG_BLIT_LOOP_8x8_MAIN_END(C_Opaque)
    }

    // ------------------------------------------------------------------------
    // [C-Mask]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_C_MASK()
    {
      FOG_BLIT_LOOP_8x8_INIT()

      uint32_t msk0p;
      Acc::p32Copy(msk0p, msk0);
      Acc::p32Negate256SBW(msk0p, msk0p);

      FOG_BLIT_LOOP_8x8_SMALL_BEGIN(C_Mask)
        uint32_t dst0p;

        Acc::p32Load1b(dst0p, dst);
        Acc::p32MulDiv256SBW(dst0p, dst0p, msk0p);
        Acc::p32Store1b(dst, dst0p);

        dst += 1;
      FOG_BLIT_LOOP_8x8_SMALL_END(C_Mask)

      FOG_BLIT_LOOP_8x8_MAIN_BEGIN(C_Mask)
        uint32_t dst0p, dst1p;

        Acc::p32Load4a(dst0p, dst + 0);
        Acc::p32Load4a(dst1p, dst + 4);
        Acc::p32MulDiv256PBB_SBW(dst0p, dst0p, msk0p);
        Acc::p32MulDiv256PBB_SBW(dst1p, dst1p, msk0p);
        Acc::p32Store4a(dst + 0, dst0p);
        Acc::p32Store4a(dst + 4, dst1p);

        dst += 8;
      FOG_BLIT_LOOP_8x8_MAIN_END(C_Mask)
    }

    // ------------------------------------------------------------------------
    // [A8-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_GLYPH()
    {
      FOG_BLIT_LOOP_8x1_INIT()

      FOG_BLIT_LOOP_8x1_BEGIN(A8_Glyph)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load1b(dst0p, dst);
        Acc::p32Load1b(msk0p, msk);
        Acc::p32Negate255SBW(msk0p, msk0p);
        Acc::p32Cvt256SBWFrom255SBW(msk0p, msk0p);
        Acc::p32MulDiv256SBW(dst0p, dst0p, msk0p);
        Acc::p32Store1b(dst, dst0p);

        dst += 1;
        msk += 1;
      FOG_BLIT_LOOP_8x1_END(A8_Glyph)
    }

    // ------------------------------------------------------------------------
    // [A8-Extra]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_A8_EXTRA()
    {
      FOG_BLIT_LOOP_8x1_INIT()

      FOG_BLIT_LOOP_8x1_BEGIN(A8_Extra)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load1b(dst0p, dst);
        Acc::p32Load2a(msk0p, msk);
        Acc::p32Negate256SBW(msk0p, msk0p);
        Acc::p32MulDiv256SBW(dst0p, dst0p, msk0p);
        Acc::p32Store1b(dst, dst0p);

        dst += 1;
        msk += 2;
      FOG_BLIT_LOOP_8x1_END(A8_Extra)
    }

    // ------------------------------------------------------------------------
    // [ARGB32-Glyph]
    // ------------------------------------------------------------------------

    FOG_CBLIT_SPAN8_ARGB32_GLYPH()
    {
      FOG_BLIT_LOOP_8x1_INIT()

      FOG_BLIT_LOOP_8x1_BEGIN(ARGB32_Glyph)
        uint32_t dst0p;
        uint32_t msk0p;

        Acc::p32Load1b(dst0p, dst);
        Acc::p32Load4a(msk0p, msk);
        Acc::p32RShift(msk0p, msk0p, 24);
        Acc::p32Negate255SBW(msk0p, msk0p);
        Acc::p32Cvt256SBWFrom255SBW(msk0p, msk0p);
        Acc::p32MulDiv256SBW(dst0p, dst0p, msk0p);
        Acc::p32Store1b(dst, dst0p);

        dst += 1;
        msk += 4;
      FOG_BLIT_LOOP_8x1_END(ARGB32_Glyph)
    }

    FOG_CBLIT_SPAN8_END()
  }

  static void FOG_FASTCALL a8_vblit_span(
    uint8_t* dst, const RasterSpan* span, const RasterClosure* closure)
  {
    a8_cblit_span(dst, NULL, span, closure);
  }
};

} // RasterOps_C namespace
} // Fog namespace

// [Guard]
#endif // _FOG_G2D_PAINTING_RASTEROPS_C_COMPOSITECLEAR_P_H
