// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_C_BASEHELPERS_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_C_BASEHELPERS_P_H

// [Dependencies]
#include <Fog/G2d/Painting/RasterOps_C/BaseDefs_p.h>
#include <Fog/G2d/Painting/RasterOps_C/CompositeBase_p.h>

namespace Fog {
namespace RasterOps_C {

// ============================================================================
// [Fog::RasterOps_C - Helpers]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT Helpers
{
  static FOG_INLINE int p_repeat_integer(int x, int y)
  {
    if ((uint)x >= (uint)y)
    {
      x %= y;
      if (x < 0) x += y;
    }

    return x;
  }

  static FOG_INLINE void p_reflect_integer(int& a0, int& a1, int maxMinus1, int max2Minus1)
  {
    a1 = a0 + 1;

    if (a0 >= maxMinus1)
    {
      if (a0 > maxMinus1) a0 = max2Minus1 - a0;
      a1 = max2Minus1 - a1;
      if (a1 < 0) a1 = 0;
    }
  }

  // --------------------------------------------------------------------------
  // [Helpers - Pattern - Any - Repeat]
  // --------------------------------------------------------------------------

  static uint8_t* FOG_FASTCALL p_repeat_prgb32(uint8_t* dst, int baseWidth, int repeatWidth)
  {
    uint8_t* base = dst - ((uint)baseWidth * 4);
    uint8_t* src = base;

    for (;;)
    {
      int i = Math::min(baseWidth, repeatWidth);
      repeatWidth -= i;

      while ((i -= 4) >= 0)
      {
        MemOps::copy_16(dst, src);

        dst += 16;
        src += 16;
      }
      i += 4;

      while (i)
      {
        MemOps::copy_4(dst, src);

        dst += 4;
        src += 4;
        i--;
      }
      if (!repeatWidth) break;

      src = base;
      baseWidth <<= 1;
    }

    return dst;
  }

  static uint8_t* FOG_FASTCALL p_repeat_a8(uint8_t* dst, int baseWidth, int repeatWidth)
  {
    uint8_t* base = dst - ((uint)baseWidth);
    uint8_t* src = base;

    for (;;)
    {
      int i = Math::min(baseWidth, repeatWidth);
      repeatWidth -= i;

      memcpy(dst, src, i);
      dst += i;
      if (!repeatWidth) break;

      src = base;
      baseWidth <<= 1;
    }

    return dst;
  }

  // --------------------------------------------------------------------------
  // [Helpers - Pattern - Any - Fill]
  // --------------------------------------------------------------------------

  static FOG_INLINE uint8_t* p_fill_prgb32(uint8_t* dst, uint32_t c0, int w)
  {
    CompositeBase::MemFill32Context ctx;
    ctx.init(c0);
    return ctx.fill(dst, w);
  }

  static FOG_INLINE uint8_t* p_fill_a8(uint8_t* dst, uint32_t c0, int w)
  {
    memset(dst, (int)c0, (size_t)w);
    return dst + w;
  }

  // ==========================================================================
  // [Helpers - Pattern - Solid - Create / Destroy]
  // ==========================================================================

  static err_t FOG_FASTCALL p_solid_create_color(
    RasterPattern* ctx, uint32_t dstFormat, const Color* color)
  {
    RasterSolid solid;

    //! ${IMAGE_FORMAT:BEGIN}
    switch (dstFormat)
    {
      case IMAGE_FORMAT_PRGB32:
      case IMAGE_FORMAT_XRGB32:
      case IMAGE_FORMAT_RGB24:
      case IMAGE_FORMAT_A8:
      case IMAGE_FORMAT_I8:
        solid.prgb32.u32 = color->getArgb32().u32;
        Acc::p32PRGB32FromARGB32(solid.prgb32.u32, solid.prgb32.u32);
        break;

      case IMAGE_FORMAT_PRGB64:
      case IMAGE_FORMAT_RGB48:
      case IMAGE_FORMAT_A16:
        solid.prgb64.p64 = color->getArgb64().p64;
        Acc::p64PRGB64FromARGB64(solid.prgb64.p64, solid.prgb64.p64);
        break;
    }
    //! ${IMAGE_FORMAT:END}

    return p_solid_create_solid(ctx, dstFormat, &solid);
  }

  static err_t FOG_FASTCALL p_solid_create_solid(
    RasterPattern* ctx, uint32_t dstFormat, const RasterSolid* solid)
  {
    ctx->_initDst(dstFormat);
    ctx->_destroy = _api_raster.solid.destroy;
    ctx->_prepare = _api_raster.solid.prepare;

    switch (dstFormat)
    {
      case IMAGE_FORMAT_PRGB32:
      case IMAGE_FORMAT_XRGB32:
      case IMAGE_FORMAT_RGB24:
      {
        if (Acc::p32PRGB32IsAlphaFF(solid->prgb32.u32))
        {
          ctx->_srcFormat = IMAGE_FORMAT_XRGB32;
          ctx->_srcBPP = 4;
          ctx->_isOpaque = true;
        }
        else
        {
          ctx->_srcFormat = IMAGE_FORMAT_PRGB32;
          ctx->_srcBPP = 4;
          ctx->_isOpaque = false;
        }
        break;
      }

      case IMAGE_FORMAT_A8:
      {
        ctx->_srcFormat = IMAGE_FORMAT_A8;
        ctx->_srcBPP = 1;
        ctx->_isOpaque = false;
        break;
      }

      case IMAGE_FORMAT_PRGB64:
      case IMAGE_FORMAT_RGB48:
      {
        if (Acc::p64PRGB64IsAlphaFFFF(solid->prgb64.p64))
        {
          ctx->_srcFormat = IMAGE_FORMAT_RGB48;
          ctx->_srcBPP = 6;
          ctx->_isOpaque = true;
        }
        else
        {
          ctx->_srcFormat = IMAGE_FORMAT_PRGB64;
          ctx->_srcBPP = 8;
          ctx->_isOpaque = false;
        }
        break;
      }

      case IMAGE_FORMAT_A16:
      {
        ctx->_srcFormat = IMAGE_FORMAT_A16;
        ctx->_srcBPP = 2;
        ctx->_isOpaque = false;
        break;
      }

      default:
        FOG_ASSERT_NOT_REACHED();
    }

    return ERR_OK;
  }

  static void FOG_FASTCALL p_solid_destroy(
    RasterPattern* ctx)
  {
    FOG_ASSERT(ctx->isInitialized());
    ctx->reset();
  }

  // ==========================================================================
  // [Helpers - Pattern - Solid - Prepare]
  // ==========================================================================

  static void FOG_FASTCALL p_solid_prepare(
    const RasterPattern* ctx, RasterPatternFetcher* fetcher, int y, int delta, uint32_t mode)
  {
    fetcher->_ctx = ctx;
    fetcher->_fetch = _api_raster.solid.fetch[ctx->_srcFormat];
    fetcher->_skip = p_solid_skip;
    fetcher->_mode = mode;
  }

  // ==========================================================================
  // [Helpers - Pattern - Solid - Fetch]
  // ==========================================================================

  // --------------------------------------------------------------------------
  // [Helpers - Pattern - Solid - Fetch]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL p_solid_fetch_helper_prgb32(
    RasterSpan* span, uint8_t* buffer, uint32_t mode, const RasterSolid& solid)
  {
    uint32_t c0 = solid.prgb32.u32;

    if (mode == RASTER_FETCH_REFERENCE)
    {
      // If the buffer will not be modified then we can fetch only width needed
      // by the largest span here. If there is only one span then this is
      // without effect, if there are many small spans then we can save some
      // CPU cycles.
      int filledWidth = 0;

      P_FETCH_SPAN8_INIT()
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CUSTOM(buffer)

        if (filledWidth < w)
        {
          dst = p_fill_prgb32(dst, c0, w - filledWidth);
          filledWidth = w;
        }

        P_FETCH_SPAN8_NEXT()
      P_FETCH_SPAN8_END()
    }
    else
    {
      P_FETCH_SPAN8_INIT()
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()
        dst = p_fill_prgb32(dst, c0, w);
        P_FETCH_SPAN8_NEXT()
      P_FETCH_SPAN8_END()
    }
  }

  static void FOG_FASTCALL p_solid_fetch_prgb32_xrgb32(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    p_solid_fetch_helper_prgb32(span, buffer, fetcher->getMode(), fetcher->getContext()->_d.solid);
  }

  static void FOG_FASTCALL p_solid_fetch_a8(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    // TODO: A8 support.
  }

  static void FOG_FASTCALL p_solid_fetch_prgb64(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    // TODO: PRGB64 support.
  }

  static void FOG_FASTCALL p_solid_fetch_rgb48(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    // TODO: RGB48 support.
  }

  static void FOG_FASTCALL p_solid_fetch_a16(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    // TODO: A16 support.
  }

  // ==========================================================================
  // [Helpers - Pattern - Solid - Skip]
  // ==========================================================================

  static void FOG_FASTCALL p_solid_skip(
    RasterPatternFetcher* fetcher, int count)
  {
  }
};

} // RasterOps_C namespace
} // Fog namespace

// [Guard]
#endif // _FOG_G2D_PAINTING_RASTEROPS_C_BASEHELPERS_P_H
