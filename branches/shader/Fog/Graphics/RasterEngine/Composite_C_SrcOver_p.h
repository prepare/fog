// [Fog-Graphics Library - Private API]
//
// [License]
// MIT, See COPYING file in package

// For some IDEs to enable code-assist.
#include <Fog/Core/Build.h>

#if defined(FOG_IDE)
#include <Fog/Graphics/RasterEngine/Defs_C_p.h>
#endif // FOG_IDE

namespace Fog {
namespace RasterEngine {

// ============================================================================
// [Fog::RasterEngine::C - Composite - SrcOver]
// ============================================================================

//! @internal
struct FOG_HIDDEN CompositeSrcOverC
{
  enum { CHARACTERISTICS = OPERATOR_SRC_OVER };

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - PRGB32 - CBlit - PRGB32]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL prgb32_cblit_full(
    uint8_t* dst, const RasterSolid* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    uint32_t srcp = src->prgb;
    uint32_t srcainv = ByteUtil::scalar_neg255(srcp >> 24);

    do {
      ((uint32_t*)dst)[0] = ByteUtil::packed_muldiv255(READ_32(dst), srcainv) + srcp;
      dst += 4;
    } while (--w);
  }

  static void FOG_FASTCALL prgb32_cblit_span(
    uint8_t* dst, const RasterSolid* src, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    uint32_t srcp = src->prgb;
    ByteUtil::byte1x2 src0orig, src1orig;
    ByteUtil::byte2x2_unpack_0213(src0orig, src1orig, srcp);

    C_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    C_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      uint32_t srcm = srcp;
      msk0 = ByteUtil::scalar_neg255(srcm >> 24);

      do {
        ((uint32_t*)dst)[0] = ByteUtil::packed_muldiv255(READ_32(dst), msk0) + srcm;

        dst += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    C_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      uint32_t srcm = ByteUtil::byte2x2_muldiv255_u_pack0213(src0orig, src1orig, msk0);
      msk0 = ByteUtil::scalar_neg255(srcm >> 24);

      do {
        ((uint32_t*)dst)[0] = ByteUtil::packed_muldiv255(READ_32(dst), msk0) + srcm;

        dst += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0;
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::byte2x2_muldiv255_u_pack0213(src0orig, src1orig, msk0);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = src0;

        dst += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        uint32_t msk0 = READ_8(msk);
        if (msk0 == 0x00) goto vMaskAlphaSparseSkip;

        src0 = srcp;
        if (msk0 == 0xFF) goto vMaskAlphaSparseFill;

        dst0 = READ_32(dst);
        src0 = ByteUtil::byte2x2_muldiv255_u_pack0213(src0orig, src1orig, msk0);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    C_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - PRGB32 - CBlit - XRGB32]
  // --------------------------------------------------------------------------

  // Redirect to:
  //   CompositeSrcC - PRGB - CBlit - PRGB.

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - PRGB32 - VBlit - PRGB32]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL prgb32_vblit_prgb32_full(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    do {
      uint32_t dst0;
      uint32_t src0;

      src0 = READ_32(src);
      if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
      if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

      dst0 = READ_32(dst);
      src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

cMaskOpaqueFill:
      ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL prgb32_vblit_prgb32_span(
    uint8_t* dst, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    V_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
        if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

        dst0 = READ_32(dst);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

cMaskOpaqueFill:
        ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      do {
        uint32_t dst0;
        uint32_t src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskAlphaSkip;

        dst0 = READ_32(dst);
        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = src0;

cMaskAlphaSkip:
        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0 = READ_32(src);
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto vMaskAlphaDenseSkip;

        dst0 = READ_32(dst);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaDenseSkip:
        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0 = READ_32(src);
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto vMaskAlphaSparseSkip;
        if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto vMaskAlphaSparseFill;

        dst0 = READ_32(dst);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    V_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - PRGB32 - VBlit - ARGB32]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL prgb32_vblit_argb32_full(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    do {
      uint32_t dst0;
      uint32_t src0;
      uint32_t srca;

      src0 = READ_32(src);
      if (RasterUtil::isAlpha0x00_ARGB32(src0)) goto cMaskOpaqueSkip;
      if (RasterUtil::isAlpha0xFF_ARGB32(src0)) goto cMaskOpaqueFill;

      srca = src0 >> 24;
      dst0 = READ_32(dst);

      src0 = ByteUtil::packed_muldiv255_0xxx(src0, srca) | (srca << 24);
      src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(srca));

cMaskOpaqueFill:
      ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL prgb32_vblit_argb32_span(
    uint8_t* dst, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    V_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;
        uint32_t srca;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_ARGB32(src0)) goto cMaskOpaqueSkip;
        if (RasterUtil::isAlpha0xFF_ARGB32(src0)) goto cMaskOpaqueFill;

        srca = src0 >> 24;
        dst0 = READ_32(dst);

        src0 = ByteUtil::packed_muldiv255_0xxx(src0, srca) | (srca << 24);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(srca));

cMaskOpaqueFill:
        ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      do {
        uint32_t dst0;
        uint32_t src0;
        uint32_t srca;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_ARGB32(src0)) goto cMaskAlphaSkip;

        dst0 = READ_32(dst);
        srca = ByteUtil::scalar_muldiv255(src0 >> 24, msk0);

        src0 = ByteUtil::packed_muldiv255_0xxx(src0, srca) | (srca << 24);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(srca));
        ((uint32_t*)dst)[0] = src0;

cMaskAlphaSkip:
        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;
        uint32_t msk0;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_ARGB32(src0)) goto vMaskAlphaDenseSkip;

        msk0 = READ_8(msk);
        dst0 = READ_32(dst);

        src0 = ByteUtil::packed_muldiv255(src0 | 0xFF000000, ByteUtil::scalar_muldiv255(src0 >> 24, msk0));
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaDenseSkip:
        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;
        uint32_t msk0;

        src0 = READ_32(src);
        msk0 = READ_8(msk);
        if ((RasterUtil::isAlpha0x00_ARGB32(src0)) | (msk0 == 0x00)) goto vMaskAlphaSparseSkip;
        if ((RasterUtil::isAlpha0xFF_ARGB32(src0)) & (msk0 == 0xFF)) goto vMaskAlphaSparseFill;

        dst0 = READ_32(dst);
        src0 = ByteUtil::packed_muldiv255(src0 | 0xFF000000, ByteUtil::scalar_muldiv255(src0 >> 24, msk0));
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    V_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - PRGB32 - VBlit - XRGB32]
  // --------------------------------------------------------------------------

  // Redirect to:
  //   CompositeSrcC - PRGB32 - VBlit - XRGB32.

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - PRGB32 - VBlit - I8]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL prgb32_vblit_i8_full(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    const uint32_t* pal = reinterpret_cast<const uint32_t*>(
      closure->srcPalette + Palette::INDEX_PRGB32);

    do {
      uint32_t dst0;
      uint32_t src0;

      src0 = pal[READ_8(src)];
      if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
      if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

      dst0 = READ_32(dst);
      src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

cMaskOpaqueFill:
      ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
      dst += 4;
      src += 1;
    } while (--w);
  }

  static void FOG_FASTCALL prgb32_vblit_i8_span(
    uint8_t* dst, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    const uint32_t* pal = reinterpret_cast<const uint32_t*>(
      closure->srcPalette + Palette::INDEX_PRGB32);

    V_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        src0 = pal[READ_8(src)];
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
        if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

        dst0 = READ_32(dst);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 += dst0;

cMaskOpaqueFill:
        ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
        dst += 4;
        src += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      do {
        uint32_t dst0;
        uint32_t src0 = pal[READ_8(src)];
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskAlphaSkip;

        dst0 = READ_32(dst);
        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = src0 + dst0;

cMaskAlphaSkip:
        dst += 4;
        src += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;
        uint32_t msk0 = READ_8(msk);

        src0 = pal[READ_8(src)];
        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto vMaskAlphaDenseSkip;

        dst0 = READ_32(dst);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 += dst0;
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaDenseSkip:
        dst += 4;
        src += 1;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        uint32_t msk0 = READ_8(msk);

        src0 = pal[READ_8(src)];
        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto vMaskAlphaSparseSkip;
        if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto vMaskAlphaSparseFill;

        dst0 = READ_32(dst);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 += dst0;

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        src += 1;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    V_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - XRGB32 - CBlit - PRGB32]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL xrgb32_cblit_full(
    uint8_t* dst, const RasterSolid* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    uint32_t srcp = src->prgb;
    uint32_t srcainv = ByteUtil::scalar_neg255(srcp >> 24);

    srcp |= 0xFF000000;
    do {
      ((uint32_t*)dst)[0] = ByteUtil::packed_muldiv255(READ_32(dst), srcainv) + srcp;
      dst += 4;
    } while (--w);
  }

  static void FOG_FASTCALL xrgb32_cblit_span(
    uint8_t* dst, const RasterSolid* src, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    uint32_t srcp = src->prgb;
    ByteUtil::byte1x2 src0orig, src1orig;
    ByteUtil::byte2x2_unpack_0213(src0orig, src1orig, srcp);

    C_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    C_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      uint32_t srcm = srcp;
      msk0 = ByteUtil::scalar_neg255(srcm >> 24);
      srcm |= 0xFF000000;

      do {
        ((uint32_t*)dst)[0] = ByteUtil::packed_muldiv255_0xxx(READ_32(dst), msk0) + srcm;

        dst += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    C_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      uint32_t srcm = srcm = ByteUtil::byte2x2_muldiv255_u_pack0213(src0orig, src1orig, msk0);
      msk0 = ByteUtil::scalar_neg255(srcm >> 24);
      srcm |= 0xFF000000;

      do {
        ((uint32_t*)dst)[0] = ByteUtil::packed_muldiv255_0xxx(READ_32(dst), msk0) + srcm;

        dst += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0;
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::byte2x2_muldiv255_u_pack0213(src0orig, src1orig, msk0);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

        ((uint32_t*)dst)[0] = (dst0 + src0) | 0xFF000000;

        dst += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        uint32_t msk0 = READ_8(msk);
        if (msk0 == 0x00) goto vMaskAlphaSparseSkip;

        dst0 = READ_32(dst);
        src0 = ByteUtil::byte2x2_muldiv255_u_pack0213(src0orig, src1orig, msk0);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 |= 0xFF000000;
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    C_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    C_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - XRGB32 - CBlit - XRGB32]
  // --------------------------------------------------------------------------
  
  // Redirect to:
  //   CompositeSrcC - XRGB32 - CBlit - XRGB32.

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - XRGB32 - VBlit - PRGB32]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL xrgb32_vblit_prgb32_full(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    do {
      uint32_t dst0;
      uint32_t src0;

      src0 = READ_32(src);
      if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
      if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

      dst0 = READ_32(dst);
      dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

      src0 += dst0;
      src0 |= 0xFF000000;

cMaskOpaqueFill:
      ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL xrgb32_vblit_prgb32_span(
    uint8_t* dst, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    V_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
        if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

        dst0 = READ_32(dst);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

        src0 += dst0;
        src0 |= 0xFF000000;

cMaskOpaqueFill:
        ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0 = READ_32(src);

        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        dst0 = ByteUtil::packed_muldiv255_0xxx(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = (src0 + dst0) | 0xFF000000;

        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0 = READ_32(src);
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

        src0 += dst0;
        src0 |= 0xFF000000;

        ((uint32_t*)dst)[0] = src0;

        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        uint32_t msk0 = READ_8(msk);
        if (msk0 == 0x00) goto vMaskAlphaSparseSkip;

        dst0 = READ_32(dst);
        src0 = READ_32(src);
        if (RasterUtil::isAlpha0xFF_PRGB32(src0) & (msk0 == 0xFF)) goto vMaskAlphaSparseFill;

        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 |= 0xFF000000;

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    V_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - XRGB32 - VBlit - ARGB32]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL xrgb32_vblit_argb32_full(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    do {
      uint32_t dst0;
      uint32_t src0;
      uint32_t srca;

      src0 = READ_32(src);
      if (RasterUtil::isAlpha0x00_ARGB32(src0)) goto cMaskOpaqueSkip;
      if (RasterUtil::isAlpha0xFF_ARGB32(src0)) goto cMaskOpaqueFill;

      srca = src0 >> 24;
      dst0 = READ_32(dst);

      src0 = ByteUtil::packed_muldiv255_0xxx(src0, srca) | (srca << 24);
      dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(srca));

      src0 += dst0;
      src0 |= 0xFF000000;

cMaskOpaqueFill:
      ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL xrgb32_vblit_argb32_span(
    uint8_t* dst, const Span* span, const RasterClosure* closure)
  {
    // [Begin] ----------------------------------------------------------------
    V_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;
        uint32_t srca;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0x00_ARGB32(src0)) goto cMaskOpaqueSkip;
        if (RasterUtil::isAlpha0xFF_ARGB32(src0)) goto cMaskOpaqueFill;

        srca = src0 >> 24;
        dst0 = READ_32(dst);

        src0 = ByteUtil::packed_muldiv255_0xxx(src0, srca) | (srca << 24);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(srca));

        src0 += dst0;
        src0 |= 0xFF000000;

cMaskOpaqueFill:
        ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0 = READ_32(src);
        uint32_t srca = ByteUtil::scalar_muldiv255(src0 >> 24, msk0);

        src0 = ByteUtil::packed_muldiv255_Fxxx(src0, srca);
        dst0 = ByteUtil::packed_muldiv255_0xxx(dst0, ByteUtil::scalar_neg255(srca));
        ((uint32_t*)dst)[0] = src0 + dst0;

        dst += 4;
        src += 4;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0 = READ_32(src);
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::packed_muldiv255(src0 | 0xFF000000, ByteUtil::scalar_muldiv255(src0 >> 24, msk0));
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

        src0 += dst0;
        src0 |= 0xFF000000;
        ((uint32_t*)dst)[0] = src0;

        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        uint32_t msk0 = READ_8(msk);
        if (msk0 == 0x00) goto vMaskAlphaSparseSkip;

        src0 = READ_32(src);
        if (RasterUtil::isAlpha0xFF_ARGB32(src0) & (msk0 == 0xFF)) goto vMaskAlphaSparseFill;

        dst0 = READ_32(dst);
        src0 = ByteUtil::packed_muldiv255(src0 | 0xFF000000, ByteUtil::scalar_muldiv255(src0 >> 24, msk0));
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 |= 0xFF000000;

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        src += 4;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    V_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }

  // --------------------------------------------------------------------------
  // [CompositeSrcOverC - XRGB32 - VBlit - I8]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL xrgb32_vblit_i8_full(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0);

    const uint32_t* pal = reinterpret_cast<const uint32_t*>(
      closure->srcPalette + Palette::INDEX_PRGB32);

    do {
      uint32_t dst0;
      uint32_t src0;

      src0 = pal[READ_8(src)];
      if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
      if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

      dst0 = READ_32(dst);
      dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

      src0 += dst0;
      src0 |= 0xFF000000;

cMaskOpaqueFill:
      ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
      dst += 4;
      src += 1;
    } while (--w);
  }

  static void FOG_FASTCALL xrgb32_vblit_i8_span(
    uint8_t* dst, const Span* span, const RasterClosure* closure)
  {
    const uint32_t* pal = reinterpret_cast<const uint32_t*>(
      closure->srcPalette + Palette::INDEX_PRGB32);

    // [Begin] ----------------------------------------------------------------
    V_BLIT_SPAN8_BEGIN(4)
    // ------------------------------------------------------------------------

    // [CMask - Opaque] -------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_OPAQUE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        src0 = pal[READ_8(src)];
        if (RasterUtil::isAlpha0x00_PRGB32(src0)) goto cMaskOpaqueSkip;
        if (RasterUtil::isAlpha0xFF_PRGB32(src0)) goto cMaskOpaqueFill;

        dst0 = READ_32(dst);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

        src0 += dst0;
        src0 |= 0xFF000000;

cMaskOpaqueFill:
        ((uint32_t*)dst)[0] = src0;

cMaskOpaqueSkip:
        dst += 4;
        src += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [CMask - Alpha] --------------------------------------------------------
    V_BLIT_SPAN8_CASE_CMASK_ALPHA()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0 = pal[READ_8(src)];

        src0 = ByteUtil::packed_muldiv255_Fxxx(src0, msk0);
        dst0 = ByteUtil::packed_muldiv255_0xxx(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        ((uint32_t*)dst)[0] = src0 + dst0;

        dst += 4;
        src += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Dense] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_DENSE()
    {
      do {
        uint32_t dst0 = READ_32(dst);
        uint32_t src0 = pal[READ_8(src)];
        uint32_t msk0 = READ_8(msk);

        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        dst0 = ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));

        src0 += dst0;
        src0 |= 0xFF000000;
        ((uint32_t*)dst)[0] = src0;

        dst += 4;
        src += 1;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - Alpha - Sparse] -----------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_A_SPARSE()
    {
      do {
        uint32_t dst0;
        uint32_t src0;

        uint32_t msk0 = READ_8(msk);
        if (msk0 == 0x00) goto vMaskAlphaSparseSkip;

        src0 = pal[READ_8(src)];
        if (RasterUtil::isAlpha0xFF_PRGB32(src0) & (msk0 == 0xFF)) goto vMaskAlphaSparseFill;

        dst0 = READ_32(dst);
        src0 = ByteUtil::packed_muldiv255(src0, msk0);
        src0 += ByteUtil::packed_muldiv255(dst0, ByteUtil::scalar_neg255(src0 >> 24));
        src0 |= 0xFF000000;

vMaskAlphaSparseFill:
        ((uint32_t*)dst)[0] = src0;

vMaskAlphaSparseSkip:
        dst += 4;
        src += 1;
        msk += 1;
      } while (--w);
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Dense] -------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_DENSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [VMask - ARGB - Sparse] ------------------------------------------------
    V_BLIT_SPAN8_CASE_VMASK_ARGB_SPARSE()
    {
      // TODO: VMASK-ARGB.
    }
    // ------------------------------------------------------------------------

    // [End] ------------------------------------------------------------------
    V_BLIT_SPAN8_END()
    // ------------------------------------------------------------------------
  }
};

} // RasterEngine namespace
} // Fog namespace
