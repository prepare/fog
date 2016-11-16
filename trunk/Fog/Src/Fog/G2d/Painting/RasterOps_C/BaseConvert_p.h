// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_C_BASECONVERT_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_C_BASECONVERT_P_H

// [Dependencies]
#include <Fog/G2d/Painting/RasterOps_C/BaseDefs_p.h>
#include <Fog/G2d/Painting/RasterOps_C/CompositeBase_p.h>

namespace Fog {
namespace RasterOps_C {

// ============================================================================
// [Fog::RasterOps_C - Convert]
// ============================================================================

struct FOG_NO_EXPORT Convert
{
  // ==========================================================================
  // [Init]
  // ==========================================================================

  static err_t FOG_FASTCALL init(ImageConverterData* d)
  {
    // Data must be detached before initialization.
    FOG_ASSERT(d->reference.get() == 1);

    const ImageFormatDescription& df = d->dstFormatDescription;
    const ImageFormatDescription& sf = d->srcFormatDescription;

    // Maximum bytes per component.
    uint32_t dfMaxBpc = Math::max<uint32_t>(df.getASize(), df.getRSize(), df.getGSize(), df.getBSize());
    uint32_t sfMaxBpc = Math::max<uint32_t>(sf.getASize(), sf.getRSize(), sf.getGSize(), sf.getBSize());

    if (df.isIndexed()) dfMaxBpc = 8;
    if (sf.isIndexed()) sfMaxBpc = 8;

    uint32_t maxBpc = Math::max<uint32_t>(dfMaxBpc, sfMaxBpc);

    uint32_t keepColorSpace = ( (df.isPremultiplied() == sf.isPremultiplied()) ||
                                (df.isPremultiplied() && sf.getASize() == 0  ) ||
                                (sf.isPremultiplied() && df.getASize() == 0  ) );

    uint32_t premultiply    = ( !keepColorSpace && df.isPremultiplied() );

    // ------------------------------------------------------------------------
    // [Special Case - Built-In]
    // ------------------------------------------------------------------------

    if (df.getFormat() < IMAGE_FORMAT_COUNT && sf.getFormat() < IMAGE_FORMAT_COUNT)
    {
      d->blitFn = (ImageConverterBlitLineFunc)_api_raster.getCopyFullFunc(df.getFormat(), sf.getFormat());

      // We are blitting from Fog supported format into another Fog supported
      // format. This blit is realized using COMPOSITE_SRC operator which must
      // be always implemented. This assert is here mainly for the future to
      // detect the case that the image formats were extended but the no
      // appropriate implementation was found.
      FOG_ASSERT(d->blitFn);

      return ERR_OK;
    }

    // ------------------------------------------------------------------------
    // [Raster Format]
    // ------------------------------------------------------------------------

    uint32_t dfRasterFormat = getRasterFormat(df);
    uint32_t sfRasterFormat = getRasterFormat(sf);

    // ------------------------------------------------------------------------
    // [Special Case - Copy]
    // ------------------------------------------------------------------------

    if (d->isCopy)
    {
      // ImageConverterData::isCopy means that there is no conversion between
      // target and source pixels, thus pixel format depth must be equal as well.
      FOG_ASSERT(df.getDepth() == sf.getDepth());

      switch (df.getDepth())
      {
        case   8: d->blitFn = _api_raster.convert.copy[RASTER_COPY_8  ]; return ERR_OK;
        case  16: d->blitFn = _api_raster.convert.copy[RASTER_COPY_16 ]; return ERR_OK;
        case  24: d->blitFn = _api_raster.convert.copy[RASTER_COPY_24 ]; return ERR_OK;
        case  32: d->blitFn = _api_raster.convert.copy[RASTER_COPY_32 ]; return ERR_OK;
        case  48: d->blitFn = _api_raster.convert.copy[RASTER_COPY_48 ]; return ERR_OK;
        case  64: d->blitFn = _api_raster.convert.copy[RASTER_COPY_64 ]; return ERR_OK;
      }
    }

    // ------------------------------------------------------------------------
    // [Special Case - Fill]
    // ------------------------------------------------------------------------

    if ((df.getComponentMask() & IMAGE_COMPONENT_RGB  ) == 0 &&
        (sf.getComponentMask() & IMAGE_COMPONENT_ALPHA) == 0)
    {
      if (df.getFormat() == IMAGE_FORMAT_A8)
      {
        d->blitFn = _api_raster.convert.fill[RASTER_FILL_8];
        return ERR_OK;
      }

      if (df.getFormat() == IMAGE_FORMAT_A16)
      {
        d->blitFn = _api_raster.convert.fill[RASTER_FILL_16];
        return ERR_OK;
      }
    }

    // ------------------------------------------------------------------------
    // [Special Case - BSwap]
    // ------------------------------------------------------------------------

    if (d->isBSwap)
    {
      FOG_ASSERT(df.getDepth() == sf.getDepth());

      switch (df.getDepth())
      {
        case  16: d->blitFn = _api_raster.convert.bswap[RASTER_BSWAP_16  ]; return ERR_OK;
        case  24: d->blitFn = _api_raster.convert.bswap[RASTER_BSWAP_24  ]; return ERR_OK;
        case  32: d->blitFn = _api_raster.convert.bswap[RASTER_BSWAP_32  ]; return ERR_OK;
        case  48: d->blitFn = _api_raster.convert.bswap[RASTER_BSWAP_48  ]; return ERR_OK;
        case  64: d->blitFn = _api_raster.convert.bswap[RASTER_BSWAP_64  ]; return ERR_OK;
      }
    }

    // Prepare the structure.
    RasterConvertMulti* multi = reinterpret_cast<RasterConvertMulti*>(d->buffer);
    memset(multi, 0, sizeof(RasterConvertMulti));

    // ------------------------------------------------------------------------
    // [One or Two-Pass Conversion - Max 8-BPC]
    // ------------------------------------------------------------------------

    // Max 8-bit per component.
    if (maxBpc <= 8)
    {
      if (!keepColorSpace)
      {
        multi->middleware = premultiply ? _api_raster.convert.prgb32_from_argb32
                                        : _api_raster.convert.argb32_from_prgb32;
      }

      // Destination is PRGB32 or XRGB32.
      if (df.getFormat() == IMAGE_FORMAT_PRGB32 || df.getFormat() == IMAGE_FORMAT_XRGB32)
      {
        RasterVBlitLineFunc blitLine = _api_raster.convert.argb32_from[sfRasterFormat];

        initSinglePass(multi->pass[0], df, sf);

        if (keepColorSpace)
        {
          // One-pass run.
          d->blitFn = blitLine;
        }
        else
        {
          // One pass run, but including post-process (color-space conversion).
          multi->blit[0] = blitLine;
          d->blitFn = passOnePostprocess;
        }

        return ERR_OK;
      }

      // Source is PRGB32, or XRGB32.
      if (sf.getFormat() == IMAGE_FORMAT_PRGB32 || sf.getFormat() == IMAGE_FORMAT_XRGB32)
      {
        RasterVBlitLineFunc blitLine = _api_raster.convert.from_argb32[dfRasterFormat];
        initSinglePass(multi->pass[0], df, sf);

        if (keepColorSpace)
        {
          // One-pass run.
          d->blitFn = blitLine;
        }
        else
        {
          // One pass run, but including pre-process (color-space conversion).
          multi->step = RASTER_CONVERT_BUFFER_SIZE / 4;
          multi->dstAdvance = df.getBytesPerPixel() * multi->step;
          multi->srcAdvance = sf.getBytesPerPixel() * multi->step;
          multi->blit[0] = blitLine;
          d->blitFn = passOnePreprocess;
        }

        return ERR_OK;
      }

      // Multi-pass conversion.
      {
        const ImageFormatDescription& mf = ImageFormatDescription::getByFormat(
          sf.getASize() > 0 ? IMAGE_FORMAT_PRGB32 : IMAGE_FORMAT_XRGB32);

        RasterVBlitLineFunc mfFromSf = _api_raster.convert.argb32_from[sfRasterFormat];
        RasterVBlitLineFunc dfFromMf = _api_raster.convert.from_argb32[dfRasterFormat];

        initSinglePass(multi->pass[0], mf, sf);
        initSinglePass(multi->pass[1], df, mf);

        multi->step = RASTER_CONVERT_BUFFER_SIZE / 4;
        multi->dstAdvance = df.getBytesPerPixel() * multi->step;
        multi->srcAdvance = sf.getBytesPerPixel() * multi->step;

        multi->blit[0] = mfFromSf;
        multi->blit[1] = dfFromMf;
        d->blitFn = passTwo;

        return ERR_OK;
      }
    }

    // ------------------------------------------------------------------------
    // [One or Two-Pass Conversion - Max 16-BPC]
    // ------------------------------------------------------------------------

    // Max 16-bit per component.
    if (maxBpc <= 16)
    {
      if (!keepColorSpace)
      {
        multi->middleware = premultiply
          ? _api_raster.convert.prgb64_from_argb64
          : _api_raster.convert.argb64_from_prgb64;
      }

      // Destination is ARGB64 or PRGB64.
      if (df.getFormat() == IMAGE_FORMAT_PRGB64)
      {
        RasterVBlitLineFunc blitLine = _api_raster.convert.argb64_from[sfRasterFormat];
        initSinglePass(multi->pass[0], df, sf);

        if (keepColorSpace)
        {
          // One-pass run.
          d->blitFn = blitLine;
        }
        else
        {
          // One pass run, but including post-process (color-space conversion).
          multi->blit[0] = blitLine;
          d->blitFn = passOnePostprocess;
        }

        return ERR_OK;
      }

      // Source is ARGB64 or PRGB64.
      if (sf.getFormat() == IMAGE_FORMAT_PRGB64)
      {
        RasterVBlitLineFunc blitLine = _api_raster.convert.from_argb64[dfRasterFormat];
        initSinglePass(multi->pass[0], df, sf);

        if (keepColorSpace)
        {
          // One-pass run.
          d->blitFn = blitLine;
        }
        else
        {
          // One pass run, but including pre-process (color-space conversion).
          multi->step = RASTER_CONVERT_BUFFER_SIZE / 8;
          multi->dstAdvance = df.getBytesPerPixel() * multi->step;
          multi->srcAdvance = sf.getBytesPerPixel() * multi->step;
          multi->blit[0] = blitLine;
          d->blitFn = passOnePreprocess;
        }

        return ERR_OK;
      }

      // Multi-pass conversion.
      {
        const ImageFormatDescription& mf = ImageFormatDescription::getByFormat(IMAGE_FORMAT_PRGB64);

        RasterVBlitLineFunc mfFromSf = _api_raster.convert.argb64_from[sfRasterFormat];
        RasterVBlitLineFunc dfFromMf = _api_raster.convert.from_argb64[dfRasterFormat];

        initSinglePass(multi->pass[0], mf, sf);
        initSinglePass(multi->pass[1], df, mf);

        multi->step = RASTER_CONVERT_BUFFER_SIZE / 8;
        multi->dstAdvance = df.getBytesPerPixel() * multi->step;
        multi->srcAdvance = sf.getBytesPerPixel() * multi->step;

        multi->blit[0] = mfFromSf;
        multi->blit[1] = dfFromMf;
        d->blitFn = passTwo;

        return ERR_OK;
      }
    }

    return ERR_IMAGE_INVALID_FORMAT;
  }

  // ==========================================================================
  // [Helpers - Initialization]
  // ==========================================================================

  static void FOG_FASTCALL initSinglePass(
    RasterConvertPass& pass,
    const ImageFormatDescription& df,
    const ImageFormatDescription& sf)
  {
    pass.fill = FOG_UINT64_C(0);

    if (df.getASize() != 0 && sf.getASize() == 0)
      pass.fill |= df.getAMask64();

    if (df.fillUnusedBits())
      pass.fill |= df.getUnusedBits64();

    pass.aDstShift = df.getAPos();
    pass.rDstShift = df.getRPos();
    pass.gDstShift = df.getGPos();
    pass.bDstShift = df.getBPos();

    pass.aSrcShift = sf.getAPos();
    pass.rSrcShift = sf.getRPos();
    pass.gSrcShift = sf.getGPos();
    pass.bSrcShift = sf.getBPos();

    pass.aSrcMask = sf.getAMax();
    pass.rSrcMask = sf.getRMax();
    pass.gSrcMask = sf.getGMax();
    pass.bSrcMask = sf.getBMax();

    pass.aScale = getScaleConstant(df.getASize(), sf.getASize());
    pass.rScale = getScaleConstant(df.getRSize(), sf.getRSize());
    pass.gScale = getScaleConstant(df.getGSize(), sf.getGSize());
    pass.bScale = getScaleConstant(df.getBSize(), sf.getBSize());
  }

  static uint32_t FOG_FASTCALL getRasterFormat(const ImageFormatDescription& d)
  {
    switch (d.getDepth())
    {
      case 8:
        if (d.getFormat() == IMAGE_FORMAT_A8)
          return RASTER_FORMAT_A8;

        if (d.getFormat() == IMAGE_FORMAT_I8)
          return RASTER_FORMAT_I8;

        break;

      case 16:
        if (d.getAMask32() == 0xFFFF)
          return RASTER_FORMAT_A16 + d.isByteSwapped();

        if (d.getAMask32() == 0x0000 &&
            d.getRMask32() == 0x7C00 &&
            d.getGMask32() == 0x03E0 &&
            d.getBMask32() == 0x001F)
          return RASTER_FORMAT_RGB16_555 + d.isByteSwapped();

        if (d.getAMask32() == 0x0000 &&
            d.getRMask32() == 0xF800 &&
            d.getGMask32() == 0x07E0 &&
            d.getBMask32() == 0x001F)
          return RASTER_FORMAT_RGB16_565 + d.isByteSwapped();

        if (d.getAMask32() == 0xF000 &&
            d.getRMask32() == 0x0F00 &&
            d.getGMask32() == 0x00F0 &&
            d.getBMask32() == 0x000F)
          return RASTER_FORMAT_ARGB16_4444 + d.isByteSwapped();

        return RASTER_FORMAT_ARGB16_CUSTOM + d.isByteSwapped();

      case 24:
        if (d.getAMask32() == 0x00000000 &&
            d.getRMask32() == 0x00FF0000 &&
            d.getGMask32() == 0x0000FF00 &&
            d.getBMask32() == 0x000000FF)
          return RASTER_FORMAT_RGB24_888;

        if (d.getAMask32() == 0x00000000 &&
            d.getRMask32() == 0x000000FF &&
            d.getGMask32() == 0x0000FF00 &&
            d.getBMask32() == 0x00FF0000)
          return RASTER_FORMAT_RGB24_888_BS;

        return RASTER_FORMAT_ARGB24_CUSTOM + d.isByteSwapped();

      case 32:
        if (d.getAMask32() == 0x00000000 &&
            d.getRMask32() == 0x00FF0000 &&
            d.getGMask32() == 0x0000FF00 &&
            d.getBMask32() == 0x000000FF)
          return RASTER_FORMAT_RGB32_888;

        if (d.getAMask32() == 0x00000000 &&
            d.getRMask32() == 0x000000FF &&
            d.getGMask32() == 0x0000FF00 &&
            d.getBMask32() == 0x00FF0000)
          return RASTER_FORMAT_RGB32_888_BS;

        if (d.getAMask32() == 0xFF000000 &&
            d.getRMask32() == 0x00FF0000 &&
            d.getGMask32() == 0x0000FF00 &&
            d.getBMask32() == 0x000000FF)
          return RASTER_FORMAT_ARGB32_8888;

        if (d.getAMask32() == 0x000000FF &&
            d.getRMask32() == 0x0000FF00 &&
            d.getGMask32() == 0x00FF0000 &&
            d.getBMask32() == 0xFF000000)
          return RASTER_FORMAT_ARGB32_8888_BS;

        return RASTER_FORMAT_ARGB32_CUSTOM + d.isByteSwapped();

      case 48:
        if (d.getAMask64() == FOG_UINT64_C(0x0000000000000000) &&
            d.getRMask64() == FOG_UINT64_C(0x0000FFFF00000000) &&
            d.getGMask64() == FOG_UINT64_C(0x00000000FFFF0000) &&
            d.getBMask32() == FOG_UINT64_C(0x000000000000FFFF))
          return RASTER_FORMAT_RGB48_161616 + d.isByteSwapped();

        if (d.getAMask64() == 0)
          return RASTER_FORMAT_RGB48_CUSTOM + d.isByteSwapped();

        return RASTER_FORMAT_ARGB48_CUSTOM + d.isByteSwapped();

      case 64:
        if (d.getAMask64() == FOG_UINT64_C(0xFFFF000000000000) &&
            d.getRMask64() == FOG_UINT64_C(0x0000FFFF00000000) &&
            d.getGMask64() == FOG_UINT64_C(0x00000000FFFF0000) &&
            d.getBMask32() == FOG_UINT64_C(0x000000000000FFFF))
          return RASTER_FORMAT_ARGB64_16161616 + d.isByteSwapped();

        return RASTER_FORMAT_ARGB64_CUSTOM + d.isByteSwapped();
    }

    // Failed, shouldn't happen.
    return RASTER_FORMAT_COUNT;
  }

  static uint32_t FOG_FASTCALL getScaleConstant(uint32_t dstSize, uint32_t srcSize)
  {
    if (dstSize == 0 || srcSize == 0)
      return 0;

    if (dstSize <= srcSize)
      // Just scale to match the destination, remaining pixels will be masked.
      return (uint32_t)1 << (16 + dstSize - srcSize);
    else
      return (uint32_t)( (uint64_t)((1 << dstSize) - 1) * 65537 / ((1 << srcSize) - 1) );
  }

  // ==========================================================================
  // [Helpers - Conversion]
  // ==========================================================================

  static void FOG_FASTCALL passOnePreprocess(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const ImageConverterData* d = reinterpret_cast<const ImageConverterData*>(closure->data);
    const RasterConvertMulti* multi = reinterpret_cast<const RasterConvertMulti*>(d->buffer);

    int step = multi->step;
    FOG_ASSUME(step > 0);

    RasterClosure passClosure;
    passClosure.ditherOrigin = closure->ditherOrigin;
    passClosure.palette = NULL;

    uint8_t buffer[RASTER_CONVERT_BUFFER_SIZE];

    for (;;)
    {
      step = Math::min<int>(step, w);

      // Color-space conversion (middleware).
      multi->middleware(buffer, src, step, &passClosure);

      // Convert.
      passClosure.data = (void*)&multi->pass[0];
      multi->blit[0](dst, buffer, step, &passClosure);

      // Advance.
      if ((w -= step) == 0) break;

      dst += multi->dstAdvance;
      src += multi->srcAdvance;
      passClosure.ditherOrigin.x += step;
    }
  }

  static void FOG_FASTCALL passOnePostprocess(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const ImageConverterData* d = reinterpret_cast<const ImageConverterData*>(closure->data);
    const RasterConvertMulti* multi = reinterpret_cast<const RasterConvertMulti*>(d->buffer);

    RasterClosure passClosure;
    passClosure.ditherOrigin = closure->ditherOrigin;
    passClosure.palette = NULL;

    // Convert.
    passClosure.data = (void*)&multi->pass[0];
    multi->blit[0](dst, src, w, &passClosure);

    // Color-space conversion (middleware).
    multi->middleware(dst, dst, w, &passClosure);
  }

  static void FOG_FASTCALL passTwo(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const ImageConverterData* d = reinterpret_cast<const ImageConverterData*>(closure->data);
    const RasterConvertMulti* multi = reinterpret_cast<const RasterConvertMulti*>(d->buffer);

    int step = multi->step;
    FOG_ASSUME(step > 0);

    RasterClosure passClosure;
    passClosure.ditherOrigin = closure->ditherOrigin;

    uint8_t buffer[RASTER_CONVERT_BUFFER_SIZE];

    for (;;)
    {
      step = Math::min<int>(step, w);

      // The first pass (convert from src to the middle format).
      passClosure.data = (void*)&multi->pass[0];
      passClosure.palette = closure->palette;
      multi->blit[0](buffer, src, step, &passClosure);
      passClosure.palette = NULL;

      // Color-space conversion (middleware).
      if (multi->middleware)
        multi->middleware(buffer, buffer, step, &passClosure);

      // The second pass (convert from the middle format to destination).
      passClosure.data = (void*)&multi->pass[1];
      multi->blit[1](dst, buffer, step, &passClosure);

      // Advance.
      if ((w -= step) == 0) break;

      dst += multi->dstAdvance;
      src += multi->srcAdvance;
      passClosure.ditherOrigin.x += step;
    }
  }

  // ==========================================================================
  // [MemCopy - Helpers]
  // ==========================================================================

  static FOG_INLINE uint32_t scalePixel(uint32_t pix0, uint32_t shift, uint32_t mask, uint32_t scale)
  {
    return ((pix0 >> shift) & mask) * scale;
  }

  static FOG_INLINE uint32_t scalePixel(uint64_t pix0, uint32_t shift, uint32_t mask, uint32_t scale)
  {
    return ((uint32_t)(pix0 >> shift) & mask) * scale;
  }

  // ==========================================================================
  // [MemCopy - 8]
  // ==========================================================================

  static void FOG_FASTCALL copy_8(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    while ( ((size_t)dst & (sizeof(size_t) - 1)) )
    {
      MemOps::copy_1(dst, src);
      dst += 1;
      src += 1;
      if (--w == 0) return;
    }

    while ((w -= 32) >= 0)
    {
      MemOps::copy_32(dst, src);
      dst += 32;
      src += 32;
    }

    w += 32;

    while ((w -= 4) >= 0)
    {
      MemOps::copy_4(dst, src);
      dst += 4;
      src += 4;
    }

    w += 4;
    if (w == 0)
      return;

    dst += w;
    src += w;

    switch (w)
    {
      case 3: MemOps::copy_1(dst - 3, src - 3);
      case 2: MemOps::copy_1(dst - 2, src - 2);
      case 1: MemOps::copy_1(dst - 1, src - 1);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [MemCopy - 16]
  // ==========================================================================

  static void FOG_FASTCALL copy_16(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    while ( ((size_t)dst & (sizeof(size_t) - 1)) )
    {
      MemOps::copy_2(dst, src);
      dst += 2;
      src += 2;
      if (--w == 0) return;
    }

    while ((w -= 16) >= 0)
    {
      MemOps::copy_32(dst, src);
      dst += 32;
      src += 32;
    }

    w += 16;

    while ((w -= 4) >= 0)
    {
      MemOps::copy_8(dst, src);
      dst += 8;
      src += 8;
    }

    w += 4;
    if (w == 0)
      return;

    dst += (uint)w * 2;
    src += (uint)w * 2;

    switch (w)
    {
      case 3: MemOps::copy_2(dst - 6, src - 6);
      case 2: MemOps::copy_2(dst - 4, src - 4);
      case 1: MemOps::copy_2(dst - 2, src - 2);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [MemCopy - 24]
  // ==========================================================================

  static void FOG_FASTCALL copy_24(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    w *= 3;

    while ( ((size_t)dst & (sizeof(size_t) - 1)) )
    {
      MemOps::copy_1(dst, src);
      dst += 1;
      src += 1;
      if (--w == 0) return;
    }

    while ((w -= 32) >= 0)
    {
      MemOps::copy_32(dst, src);
      dst += 32;
      src += 32;
    }

    w += 32;

    while ((w -= 4) >= 0)
    {
      MemOps::copy_4(dst, src);
      dst += 4;
      src += 4;
    }

    w += 4;
    if (w == 0)
      return;

    dst += w;
    src += w;

    switch (w)
    {
      case 3: MemOps::copy_1(dst - 3, src - 3);
      case 2: MemOps::copy_1(dst - 2, src - 2);
      case 1: MemOps::copy_1(dst - 1, src - 1);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [MemCopy - 32]
  // ==========================================================================

  static void FOG_FASTCALL copy_32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
#if FOG_ARCH_BITS >= 64
    // Align to 64-bits when running on 64-bit mode.
    if ((size_t)dst & 0x7)
    {
      MemOps::copy_4(dst, src);
      dst += 4;
      src += 4;
      if (--w == 0) return;
    }
#endif // FOG_ARCH_BITS

    while ((w -= 8) >= 0)
    {
      MemOps::copy_32(dst, src);
      dst += 32;
      src += 32;
    }

    w += 8;
    if (w == 0)
      return;

    dst += (uint)w * 4;
    src += (uint)w * 4;

    switch (w)
    {
      case 7: MemOps::copy_4(dst - 28, src - 28);
      case 6: MemOps::copy_4(dst - 24, src - 24);
      case 5: MemOps::copy_4(dst - 20, src - 20);
      case 4: MemOps::copy_4(dst - 16, src - 16);
      case 3: MemOps::copy_4(dst - 12, src - 12);
      case 2: MemOps::copy_4(dst -  8, src -  8);
      case 1: MemOps::copy_4(dst -  4, src -  4);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [MemCopy - 48]
  // ==========================================================================

  static void FOG_FASTCALL copy_48(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    w *= 3;

    while ( ((size_t)dst & (sizeof(size_t) - 1)) )
    {
      MemOps::copy_2(dst, src);
      dst += 2;
      src += 2;

      w--;
      FOG_ASSERT(w >= 0);
    }

    while ((w -= 16) >= 0)
    {
      MemOps::copy_32(dst, src);
      dst += 32;
      src += 32;
    }

    w += 16;

    while ((w -= 4) >= 0)
    {
      MemOps::copy_8(dst, src);
      dst += 8;
      src += 8;
    }

    w += 4;
    if (w == 0)
      return;

    dst += (uint)w * 2;
    src += (uint)w * 2;

    switch (w)
    {
      case 3: MemOps::copy_2(dst - 6, src - 6);
      case 2: MemOps::copy_2(dst - 4, src - 4);
      case 1: MemOps::copy_2(dst - 2, src - 2);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [MemCopy - 64]
  // ==========================================================================

  static void FOG_FASTCALL copy_64(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    while ((w -= 4) >= 0)
    {
      MemOps::copy_32(dst, src);
      dst += 32;
      src += 32;
    }

    w += 4;
    if (w == 0)
      return;

    dst += (uint)w * 8;
    src += (uint)w * 8;

    switch (w)
    {
      case 3: MemOps::copy_8(dst - 24, src - 24);
      case 2: MemOps::copy_8(dst - 16, src - 16);
      case 1: MemOps::copy_8(dst -  8, src -  8);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [Fill - 8]
  // ==========================================================================

  static void FOG_FASTCALL fill_8(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    uint32_t p0 = 0xFFFFFFFF;

    while ( ((size_t)dst & (sizeof(size_t) - 1)) )
    {
      Acc::p32Store1b(dst, p0);
      dst += 1;
      if (--w == 0) return;
    }

    while ((w -= 32) >= 0)
    {
      Acc::p32Store4a(dst +  0, p0);
      Acc::p32Store4a(dst +  4, p0);
      Acc::p32Store4a(dst +  8, p0);
      Acc::p32Store4a(dst + 12, p0);
      Acc::p32Store4a(dst + 16, p0);
      Acc::p32Store4a(dst + 20, p0);
      Acc::p32Store4a(dst + 24, p0);
      Acc::p32Store4a(dst + 28, p0);
      dst += 32;
    }

    w += 32;

    while ((w -= 4) >= 0)
    {
      Acc::p32Store4a(dst, p0);
      dst += 4;
    }

    w += 4;
    if (w == 0)
      return;

    dst += w;
    src += w;

    switch (w)
    {
      case 3: Acc::p32Store1b(dst - 3, p0);
      case 2: Acc::p32Store1b(dst - 2, p0);
      case 1: Acc::p32Store1b(dst - 1, p0);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [Fill - 16]
  // ==========================================================================

  static void FOG_FASTCALL fill_16(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    uint32_t p0 = 0xFFFFFFFF;

    while ( ((size_t)dst & (sizeof(size_t) - 1)) )
    {
      Acc::p32Store2a(dst, p0);
      dst += 2;
      if (--w == 0) return;
    }

    while ((w -= 16) >= 0)
    {
      Acc::p32Store4a(dst +  0, p0);
      Acc::p32Store4a(dst +  4, p0);
      Acc::p32Store4a(dst +  8, p0);
      Acc::p32Store4a(dst + 12, p0);
      Acc::p32Store4a(dst + 16, p0);
      Acc::p32Store4a(dst + 20, p0);
      Acc::p32Store4a(dst + 24, p0);
      Acc::p32Store4a(dst + 28, p0);
      dst += 32;
    }

    w += 16;

    while ((w -= 4) >= 0)
    {
      Acc::p32Store4a(dst + 0, p0);
      Acc::p32Store4a(dst + 4, p0);
      dst += 8;
    }

    w += 4;
    if (w == 0)
      return;

    dst += (uint)w * 2;

    switch (w)
    {
      case 3: Acc::p32Store2a(dst - 6, p0);
      case 2: Acc::p32Store2a(dst - 4, p0);
      case 1: Acc::p32Store2a(dst - 2, p0);
        break;

      default:
        FOG_ASSERT_NOT_REACHED();
    }
  }

  // ==========================================================================
  // [BSwap - 16]
  // ==========================================================================

  static void FOG_FASTCALL bswap_16(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      ((uint16_t*)dst)[0] = MemOps::bswap16(((const uint16_t*)src)[0]);

      dst += 2;
      src += 2;
    } while (--w);
  }

  // ==========================================================================
  // [BSwap - 24]
  // ==========================================================================

  static void FOG_FASTCALL bswap_24(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint8_t s0 = src[0];
      uint8_t s1 = src[1];
      uint8_t s2 = src[2];

      dst[0] = s2;
      dst[1] = s1;
      dst[2] = s0;

      dst += 3;
      src += 3;
    } while (--w);
  }

  // ==========================================================================
  // [BSwap - 32]
  // ==========================================================================

  static void FOG_FASTCALL bswap_32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      ((uint32_t*)dst)[0] = MemOps::bswap32(((const uint32_t*)src)[0]);

      dst += 4;
      src += 4;
    } while (--w);
  }

  // ==========================================================================
  // [BSwap - 48]
  // ==========================================================================

  static void FOG_FASTCALL bswap_48(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint16_t t0 = MemOps::bswap16(((const uint16_t*)src)[0]);
      uint16_t t1 = MemOps::bswap16(((const uint16_t*)src)[1]);
      uint16_t t2 = MemOps::bswap16(((const uint16_t*)src)[2]);

      ((uint16_t*)dst)[0] = t2;
      ((uint16_t*)dst)[1] = t1;
      ((uint16_t*)dst)[2] = t0;

      dst += 6;
      src += 6;
    } while (--w);
  }

  // ==========================================================================
  // [BSwap - 64]
  // ==========================================================================

  static void FOG_FASTCALL bswap_64(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
#if FOG_ARCH_BITS >= 64
    do {
      ((uint64_t*)dst)[0] = MemOps::bswap64(((const uint64_t*)src)[0]);

      dst += 8;
      src += 8;
    } while (--w);
#else
    do {
      uint32_t t0 = MemOps::bswap32(((const uint32_t*)src)[0]);
      uint32_t t1 = MemOps::bswap32(((const uint32_t*)src)[1]);

      ((uint32_t*)dst)[0] = t1;
      ((uint32_t*)dst)[1] = t0;

      dst += 8;
      src += 8;
    } while (--w);
#endif // FOG_ARCH_BITS
  }

  // ==========================================================================
  // [Convert - Premultiply / Demultiply]
  // ==========================================================================

  static void FOG_FASTCALL prgb32_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t c0;
      Acc::p32Load4a(c0, src);
      Acc::p32PRGB32FromARGB32(c0, c0);
      Acc::p32Store4a(dst, c0);

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_prgb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t c0;
      Acc::p32Load4a(c0, src);
      Acc::p32ARGB32FromPRGB32(c0, c0);
      Acc::p32Store4a(dst, c0);

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL prgb64_from_argb64(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      __p64 c0;
      Acc::p64Load8a(c0, src);
      Acc::p64PRGB64FromARGB64(c0, c0);
      Acc::p64Store8a(dst, c0);

      dst += 8;
      src += 8;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_prgb64(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      __p64 c0;
      Acc::p64Load8a(c0, src);
      Acc::p64ARGB64FromPRGB64(c0, c0);
      Acc::p64Store8a(dst, c0);

      dst += 8;
      src += 8;
    } while (--w);
  }

  // ==========================================================================
  // [Convert - ARGB32 <- Custom]
  // ==========================================================================

  static void FOG_FASTCALL argb32_from_rgb16_555(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load2a(pix0p, src);
      Acc::p32FRGB32FromRGB16_555(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb16_555_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load2aBSwap(pix0p, src);
      Acc::p32FRGB32FromRGB16_555(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb16_565(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load2a(pix0p, src);
      Acc::p32FRGB32FromRGB16_565(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb16_565_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load2aBSwap(pix0p, src);
      Acc::p32FRGB32FromRGB16_565(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb16_4444(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load2a(pix0p, src);
      Acc::p32ARGB32FromARGB16_4444(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb16_4444_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load2a(pix0p, src);
      Acc::p32ARGB32FromARGB16_4444_bs(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb16_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load2a(pix0p, src);
      pix0a = (scalePixel(pix0p, d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(pix0p, d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(pix0p, d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(pix0p, d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb16_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load2aBSwap(pix0p, src);
      pix0a = (scalePixel(pix0p, d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(pix0p, d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(pix0p, d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(pix0p, d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb24_888_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load3bBSwap(pix0p, src);
      Acc::p32FillPBB3(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 3;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb24_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load3b(pix0p, src);
      pix0a = (scalePixel(pix0p, d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(pix0p, d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(pix0p, d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(pix0p, d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 3;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb24_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load3bBSwap(pix0p, src);
      pix0a = (scalePixel(pix0p, d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(pix0p, d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(pix0p, d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(pix0p, d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 3;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb32_888_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4aBSwap(pix0p, src);
      Acc::p32FillPBB3(pix0p, pix0p);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb32_8888_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    uint32_t fill = (uint32_t)reinterpret_cast<const RasterConvertPass*>(closure->data)->fill;

    do {
      uint32_t pix0p;

      Acc::p32Load4aBSwap(pix0p, src);
      Acc::p32Or(pix0p, pix0p, fill);
      Acc::p32Store4a(dst, pix0p);

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb32_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(pix0p, d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(pix0p, d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(pix0p, d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb32_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load4aBSwap(pix0p, src);
      pix0a = (scalePixel(pix0p, d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(pix0p, d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(pix0p, d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(pix0p, d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb48_161616_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0r, pix0g, pix0b;

#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
      Acc::p32Load2a(pix0r, src + 0);
      Acc::p32Load2a(pix0g, src + 2);
      Acc::p32Load2a(pix0b, src + 4);
#else
      Acc::p32Load2a(pix0b, src + 0);
      Acc::p32Load2a(pix0g, src + 2);
      Acc::p32Load2a(pix0r, src + 4);
#endif // FOG_BYTE_ORDER

      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_3((pix0r << 16) | 0xFF000000, (pix0g & 0xFF) << 8, (pix0b & 0xFF)));

      dst += 4;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb48_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0r, pix0g, pix0b;

      Acc::p64Load6a(pix0p, dst);
      pix0r = (scalePixel(Acc::u64FromP64(pix0p), d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(Acc::u64FromP64(pix0p), d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(Acc::u64FromP64(pix0p), d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_4(0xFF000000, pix0r, pix0g, pix0b));

      dst += 4;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_rgb48_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0r, pix0g, pix0b;

      Acc::p64Load6aBSwap(pix0p, dst);
      pix0r = (scalePixel(Acc::u64FromP64(pix0p), d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(Acc::u64FromP64(pix0p), d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(Acc::u64FromP64(pix0p), d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_4(0xFF000000, pix0r, pix0g, pix0b));

      dst += 4;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb48_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load6a(pix0p, src);
      pix0a = (scalePixel(Acc::u64FromP64(pix0p), d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(Acc::u64FromP64(pix0p), d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(Acc::u64FromP64(pix0p), d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(Acc::u64FromP64(pix0p), d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb48_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load6aBSwap(pix0p, src);
      pix0a = (scalePixel(Acc::u64FromP64(pix0p), d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(Acc::u64FromP64(pix0p), d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(Acc::u64FromP64(pix0p), d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(Acc::u64FromP64(pix0p), d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb64_16161616_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    uint32_t fill = (uint32_t)reinterpret_cast<const RasterConvertPass*>(closure->data)->fill;

    do {
      uint32_t pix0p_10;
      uint32_t pix0p_32;

      Acc::p32Load8a(pix0p_10, pix0p_32, src);
      Acc::p32ARGB32FromARGB64_1032(pix0p_10, pix0p_10, pix0p_32);
      Acc::p32Or(pix0p_10, pix0p_10, fill);
      Acc::p32Store4a(dst, pix0p_10);

      dst += 4;
      src += 8;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb64_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load8a(pix0p, src);
      pix0a = (scalePixel(Acc::u64FromP64(pix0p), d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(Acc::u64FromP64(pix0p), d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(Acc::u64FromP64(pix0p), d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(Acc::u64FromP64(pix0p), d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 8;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_from_argb64_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load8aBSwap(pix0p, src);
      pix0a = (scalePixel(Acc::u64FromP64(pix0p), d->aSrcShift, d->aSrcMask, d->aScale) <<  8) & 0xFF000000;
      pix0r = (scalePixel(Acc::u64FromP64(pix0p), d->rSrcShift, d->rSrcMask, d->rScale)      ) & 0x00FF0000;
      pix0g = (scalePixel(Acc::u64FromP64(pix0p), d->gSrcShift, d->gSrcMask, d->gScale) >>  8) & 0x0000FF00;
      pix0b = (scalePixel(Acc::u64FromP64(pix0p), d->bSrcShift, d->bSrcMask, d->bScale) >> 16);
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 8;
    } while (--w);
  }

  // ==========================================================================
  // [Convert - Custom <- ARGB32]
  // ==========================================================================

  static void FOG_FASTCALL rgb16_555_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32RGB16_555FromXRGB32(pix0p, pix0p);
      Acc::p32Store2a(dst, pix0p);

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_555_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32RGB16_555FromXRGB32(pix0p, pix0p);
      Acc::p32Store2aBSwap(dst, pix0p);

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_565_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32RGB16_565FromXRGB32(pix0p, pix0p);
      Acc::p32Store2a(dst, pix0p);

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_565_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32RGB16_565FromXRGB32(pix0p, pix0p);
      Acc::p32Store2aBSwap(dst, pix0p);

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb16_4444_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32ARGB16_4444FromARGB32(pix0p, pix0p);
      Acc::p32Store2a(dst, pix0p);

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb16_4444_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32ARGB16_4444_bsFromARGB32(pix0p, pix0p);
      Acc::p32Store2a(dst, pix0p);

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb16_custom_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift;
      pix0r = (scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift;
      pix0g = (scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift;
      pix0b = (scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift;
      Acc::p32Store2a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb16_custom_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift;
      pix0r = (scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift;
      pix0g = (scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift;
      pix0b = (scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift;
      Acc::p32Store2aBSwap(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb24_888_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32Store3bBSwap(dst, pix0p);

      dst += 3;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb24_custom_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift;
      pix0r = (scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift;
      pix0g = (scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift;
      pix0b = (scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift;
      Acc::p32Store3b(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 3;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb24_custom_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift;
      pix0r = (scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift;
      pix0g = (scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift;
      pix0b = (scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift;
      Acc::p32Store3bBSwap(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 3;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb32_888_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32Or(pix0p, pix0p, (uint32_t)d->fill);
      Acc::p32Store4aBSwap(dst, pix0p);

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_8888_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32Store4aBSwap(dst, pix0p);

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_custom_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift;
      pix0r = (scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift;
      pix0g = (scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift;
      pix0b = (scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift;
      Acc::p32Store4a(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb32_custom_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = (scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift;
      pix0r = (scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift;
      pix0g = (scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift;
      pix0b = (scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift;
      Acc::p32Store4aBSwap(dst, _FOG_ACC_COMBINE_5(pix0a, pix0r, pix0g, pix0b, (uint32_t)d->fill));

      dst += 4;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb48_161616_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p;

      Acc::p32Load4a(pix0p, src);
      Acc::p32RGB48StoreBSwapFromXRGB32(dst, pix0p);

      dst += 6;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb48_custom_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      __p64 pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0r = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift);
      pix0g = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift);
      pix0b = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift);

      Acc::p64Combine(pix0r, pix0r, pix0g);
      Acc::p64Combine(pix0b, pix0b, Acc::p64FromU64(d->fill));
      Acc::p64Combine(pix0r, pix0r, pix0b);

      Acc::p64Store6a(dst, pix0r);

      dst += 6;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb48_custom_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      __p64 pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0r = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift);
      pix0g = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift);
      pix0b = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift);

      Acc::p64Combine(pix0r, pix0r, pix0g);
      Acc::p64Combine(pix0b, pix0b, Acc::p64FromU64(d->fill));
      Acc::p64Combine(pix0r, pix0r, pix0b);

      Acc::p64Store6aBSwap(dst, pix0r);

      dst += 6;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb48_custom_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      __p64 pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift);
      pix0r = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift);
      pix0g = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift);
      pix0b = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift);

      Acc::p64Combine(pix0r, pix0r, pix0g);
      Acc::p64Combine(pix0b, pix0b, Acc::p64FromU64(d->fill));
      Acc::p64Combine(pix0r, pix0r, pix0a);
      Acc::p64Combine(pix0r, pix0r, pix0b);

      Acc::p64Store6a(dst, pix0r);

      dst += 6;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb48_custom_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      __p64 pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift);
      pix0r = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift);
      pix0g = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift);
      pix0b = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift);

      Acc::p64Combine(pix0r, pix0r, pix0g);
      Acc::p64Combine(pix0b, pix0b, Acc::p64FromU64(d->fill));
      Acc::p64Combine(pix0r, pix0r, pix0a);
      Acc::p64Combine(pix0r, pix0r, pix0b);

      Acc::p64Store6aBSwap(dst, pix0r);

      dst += 6;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_16161616_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    uint32_t fill = (uint32_t)reinterpret_cast<const RasterConvertPass*>(closure->data)->fill;

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load4a(pix0p, src);
      Acc::p32PRGB64FromPRGB32(pix0p_10, pix0p_32, pix0p);
      Acc::p32Store8a(dst, MemOps::bswap32(pix0p_32), MemOps::bswap32(pix0p_10));

      dst += 8;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_custom_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      __p64 pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift);
      pix0r = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift);
      pix0g = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift);
      pix0b = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift);

      Acc::p64Combine(pix0r, pix0r, pix0g);
      Acc::p64Combine(pix0b, pix0b, Acc::p64FromU64(d->fill));
      Acc::p64Combine(pix0r, pix0r, pix0a);
      Acc::p64Combine(pix0r, pix0r, pix0b);

      Acc::p64Store8a(dst, pix0r);

      dst += 8;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_custom_bs_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      __p64 pix0a, pix0r, pix0g, pix0b;

      Acc::p32Load4a(pix0p, src);
      pix0a = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 24, 0xFF, d->aScale) >> 16) << d->aDstShift);
      pix0r = Acc::p64FromU64((uint64_t)(scalePixel(pix0p, 16, 0xFF, d->rScale) >> 16) << d->rDstShift);
      pix0g = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  8, 0xFF, d->gScale) >> 16) << d->gDstShift);
      pix0b = Acc::p64FromU64((uint64_t)(scalePixel(pix0p,  0, 0xFF, d->bScale) >> 16) << d->bDstShift);

      Acc::p64Combine(pix0r, pix0r, pix0g);
      Acc::p64Combine(pix0b, pix0b, Acc::p64FromU64(d->fill));
      Acc::p64Combine(pix0r, pix0r, pix0a);
      Acc::p64Combine(pix0r, pix0r, pix0b);

      Acc::p64Store8aBSwap(dst, pix0r);

      dst += 8;
      src += 4;
    } while (--w);
  }

  // ==========================================================================
  // [Convert - ARGB64 <- Custom]
  // ==========================================================================

  static void FOG_FASTCALL argb64_from_rgb16_555(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load2a(pix0p_10, src);
      Acc::p32FRGB64FromRGB16_555(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb16_555_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load2aBSwap(pix0p_10, src);
      Acc::p32FRGB64FromRGB16_555(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb16_565(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load2a(pix0p_10, src);
      Acc::p32FRGB64FromRGB16_565(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb16_565_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load2aBSwap(pix0p_10, src);
      Acc::p32FRGB64FromRGB16_565(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb16_4444(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load2a(pix0p_10, src);
      Acc::p32ARGB64FromARGB16_4444(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb16_4444_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load2a(pix0p_10, src);
      Acc::p32ARGB64FromARGB16_4444_bs(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb16_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load2a(pix0p, src);

      pix0a = (((pix0p >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (((pix0p >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (((pix0p >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (((pix0p >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb16_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load2aBSwap(pix0p, src);

      pix0a = (((pix0p >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (((pix0p >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (((pix0p >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (((pix0p >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 2;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb24_888_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;
      uint32_t pix0p_t0;

#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
      Acc::p32Load1b(pix0p_32, src + 0);
      Acc::p32Load1b(pix0p_t0, src + 1);
      Acc::p32Load1b(pix0p_10, src + 2);
#else
      Acc::p32Load1b(pix0p_10, src + 0);
      Acc::p32Load1b(pix0p_t0, src + 1);
      Acc::p32Load1b(pix0p_32, src + 2);
#endif // FOG_BYTE_ORDER

      Acc::p32LShift(pix0p_t0, pix0p_t0, 16);
      Acc::p32Combine(pix0p_10, pix0p_10, pix0p_t0);

      Acc::p32Mul(pix0p_10, pix0p_10, 257);
      Acc::p32Mul(pix0p_32, pix0p_32, 257);
      Acc::p32FillPWW1(pix0p_32, pix0p_32);

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 3;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb24_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load3b(pix0p, src);

      pix0a = (((pix0p >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (((pix0p >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (((pix0p >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (((pix0p >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 3;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb24_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load3bBSwap(pix0p, src);

      pix0a = (((pix0p >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (((pix0p >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (((pix0p >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (((pix0p >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 3;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb32_888_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load4aBSwap(pix0p_10, src);
      Acc::p32FRGB64FromXRGB32(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb32_8888_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t pix0p_10, pix0p_32;

      Acc::p32Load4aBSwap(pix0p_10, src);
      Acc::p32ARGB64FromARGB32(pix0p_10, pix0p_32, pix0p_10);
      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb32_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load4a(pix0p, src);

      pix0a = (((pix0p >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (((pix0p >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (((pix0p >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (((pix0p >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb32_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      uint32_t pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p32Load4aBSwap(pix0p, src);

      pix0a = (((pix0p >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (((pix0p >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (((pix0p >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (((pix0p >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb48_161616_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      __p64 pix0p;

      Acc::p64Load6aBSwap(pix0p, src);
      Acc::p64FillPWW3(pix0p, pix0p);
      Acc::p64Store8a(dst, pix0p);

      dst += 8;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb48_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0p_10;
      uint32_t pix0p_32;

      Acc::p64Load6a(pix0p, src);

      pix0p_10 = _FOG_ACC_COMBINE_2( (((uint32_t)(Acc::u64FromP64(pix0p) >> d->gSrcShift) & d->gSrcMask) * d->gScale) & 0xFFFF0000,
                                      (((uint32_t)(Acc::u64FromP64(pix0p) >> d->bSrcShift) & d->bSrcMask) * d->bScale) >> 16 );
      pix0p_32 = _FOG_ACC_COMBINE_2( (((uint32_t)(Acc::u64FromP64(pix0p) >> d->rSrcShift) & d->rSrcMask) * d->rScale) >> 16,
                                      0xFFFF0000);

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_rgb48_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0p_10;
      uint32_t pix0p_32;

      Acc::p64Load6aBSwap(pix0p, src);

      pix0p_10 = _FOG_ACC_COMBINE_2( (((uint32_t)(Acc::u64FromP64(pix0p) >> d->gSrcShift) & d->gSrcMask) * d->gScale) & 0xFFFF0000,
                                      (((uint32_t)(Acc::u64FromP64(pix0p) >> d->bSrcShift) & d->bSrcMask) * d->bScale) >> 16 );
      pix0p_32 = _FOG_ACC_COMBINE_2( (((uint32_t)(Acc::u64FromP64(pix0p) >> d->rSrcShift) & d->rSrcMask) * d->rScale) >> 16,
                                      0xFFFF0000);

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb48_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load6a(pix0p, src);

      pix0a = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb48_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load6aBSwap(pix0p, src);

      pix0a = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 6;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb64_16161616_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      __p64 pix0p;

      Acc::p64Load8aBSwap(pix0p, src);
      Acc::p64Store8a(dst, pix0p);

      dst += 8;
      src += 8;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb64_custom(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load8a(pix0p, src);

      pix0a = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 8;
    } while (--w);
  }

  static void FOG_FASTCALL argb64_from_argb64_custom_bs(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const RasterConvertPass* d = reinterpret_cast<const RasterConvertPass*>(closure->data);

    do {
      __p64 pix0p;
      uint32_t pix0p_10, pix0p_32;

      uint32_t pix0r, pix0g;
      uint32_t pix0b, pix0a;

      Acc::p64Load8aBSwap(pix0p, src);

      pix0a = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->aSrcShift) * d->aScale )      ) & 0xFFFF0000;
      pix0r = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->rSrcShift) * d->rScale ) >> 16);
      pix0g = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->gSrcShift) * d->gScale )      ) & 0xFFFF0000;
      pix0b = (( (uint32_t)(Acc::u64FromP64(pix0p) >> d->bSrcShift) * d->bScale ) >> 16);

      pix0p_10 = _FOG_ACC_COMBINE_2(pix0g, pix0b);
      pix0p_32 = _FOG_ACC_COMBINE_3(pix0a, pix0r, (uint32_t)(d->fill >> 32));

      Acc::p32Store8a(dst, pix0p_10, pix0p_32);

      dst += 8;
      src += 8;
    } while (--w);
  }

  // ==========================================================================
  // [Convert - Custom <- ARGB64]
  // ==========================================================================
};






// TODO: Port
#if 0
// ============================================================================
// [Fog::RasterOps_C - Dib]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT DibC
{
  // --------------------------------------------------------------------------
  // [DibC - Rect]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL cblit_rect_32_helper(
    uint8_t* dst, ssize_t dstStride,
    uint32_t src0,
    int w, int h)
  {
    FOG_ASSERT(w > 0 && h > 0);
    dstStride -= w * 4;

#if FOG_ARCH_BITS == 64
    uint64_t src0_64 = (uint64_t)src0 | ((uint64_t)src0 << 32);

    do {
      int i = w;

      while ((i -= 8) >= 0)
      {
        ((uint64_t*)dst)[0] = src0_64;
        ((uint64_t*)dst)[1] = src0_64;
        ((uint64_t*)dst)[2] = src0_64;
        ((uint64_t*)dst)[3] = src0_64;

        dst += 32;
      }
      i += 8;

      switch (i)
      {
        case 7: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 6: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 5: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 4: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 3: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 2: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 1: ((uint32_t*)dst)[0] = src0; dst += 4;
      }

      dst += dstStride;
    } while (--h);
#else
    do {
      int i = w;

      while ((i -= 8) >= 0)
      {
        ((uint32_t*)dst)[0] = src0;
        ((uint32_t*)dst)[1] = src0;
        ((uint32_t*)dst)[2] = src0;
        ((uint32_t*)dst)[3] = src0;
        ((uint32_t*)dst)[4] = src0;
        ((uint32_t*)dst)[5] = src0;
        ((uint32_t*)dst)[6] = src0;
        ((uint32_t*)dst)[7] = src0;

        dst += 32;
      }
      i += 8;

      switch (i)
      {
        case 7: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 6: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 5: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 4: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 3: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 2: ((uint32_t*)dst)[0] = src0; dst += 4;
        case 1: ((uint32_t*)dst)[0] = src0; dst += 4;
      }

      dst += dstStride;
    } while (--h);
#endif
  }

  static void FOG_FASTCALL cblit_rect_32_prgb(
    uint8_t* dst, ssize_t dstStride,
    const RasterSolid* src,
    int w, int h, const RasterClosure* closure)
  {
    cblit_rect_32_helper(dst, dstStride, src->prgb, w, h);
  }

  static void FOG_FASTCALL cblit_rect_32_argb(
    uint8_t* dst, ssize_t dstStride,
    const RasterSolid* src,
    int w, int h, const RasterClosure* closure)
  {
    cblit_rect_32_helper(dst, dstStride, src->argb, w, h);
  }

  static void FOG_FASTCALL cblit_rect_8(
    uint8_t* dst, ssize_t dstStride,
    const RasterSolid* src,
    int w, int h, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0 && h > 0);
    dstStride -= w;

#if FOG_ARCH_BITS == 64
    uint64_t src0 = (uint64_t)((src->prgb) >> 24) * FOG_UINT64_C(0x0101010101010101);
#else
    uint32_t src0 = (uint32_t)((src->prgb) >> 24) * 0x01010101;
#endif

    do {
      int i = w;

      while (((size_t)dst & 0x3) && i)
      {
        dst[0] = (uint8_t)src0;
        dst++;
        i--;
      }

      while ((i -= 32) >= 0)
      {
#if FOG_ARCH_BITS == 64
        ((uint64_t*)dst)[0] = src0;
        ((uint64_t*)dst)[1] = src0;
        ((uint64_t*)dst)[2] = src0;
        ((uint64_t*)dst)[3] = src0;
#else
        ((uint32_t*)dst)[0] = src0;
        ((uint32_t*)dst)[1] = src0;
        ((uint32_t*)dst)[2] = src0;
        ((uint32_t*)dst)[3] = src0;
        ((uint32_t*)dst)[4] = src0;
        ((uint32_t*)dst)[5] = src0;
        ((uint32_t*)dst)[6] = src0;
        ((uint32_t*)dst)[7] = src0;
#endif
        dst += 32;
      }
      i += 32;

      while ((i -= 4) >= 0)
      {
        ((uint32_t*)dst)[0] = (uint32_t)src0;
        dst += 4;
      }
      i += 4;

      switch (i)
      {
        case 3: ((uint8_t*)dst)[0] = (uint8_t)src0; dst += 1;
        case 2: ((uint8_t*)dst)[0] = (uint8_t)src0; dst += 1;
        case 1: ((uint8_t*)dst)[0] = (uint8_t)src0; dst += 1;
      }

      dst += dstStride;
    } while (--h);
  }

  static void FOG_FASTCALL vblit_rect_32(
    uint8_t* dst, ssize_t dstStride,
    const uint8_t* src, ssize_t srcStride,
    int w, int h, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0 && h > 0);

    dstStride -= w * 4;
    srcStride -= w * 4;

    do {
      int i = w;

      while ((i -= 8) >= 0)
      {
        MemOps::copy_64(dst, src);

        dst += 32;
        src += 32;
      }
      i += 8;

      switch (i)
      {
        case 7: MemOps::copy_4(dst, src); dst += 4; src += 4;
        case 6: MemOps::copy_4(dst, src); dst += 4; src += 4;
        case 5: MemOps::copy_4(dst, src); dst += 4; src += 4;
        case 4: MemOps::copy_4(dst, src); dst += 4; src += 4;
        case 3: MemOps::copy_4(dst, src); dst += 4; src += 4;
        case 2: MemOps::copy_4(dst, src); dst += 4; src += 4;
        case 1: MemOps::copy_4(dst, src); dst += 4; src += 4;
      }

      dst += dstStride;
      src += srcStride;
    } while (--h);
  }

  static void FOG_FASTCALL vblit_rect_8(
    uint8_t* dst, ssize_t dstStride,
    const uint8_t* src, ssize_t srcStride,
    int w, int h, const RasterClosure* closure)
  {
    FOG_ASSERT(w > 0 && h > 0);

    do {
      copy(dst, src, w);
      dst += dstStride;
      src += srcStride;
    } while (--h);
  }

















  static void FOG_FASTCALL frgb32_from_rgb24(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
    while (w >= 4)
    {
      // Following table illustrates how this works on Little Endian:
      //
      // [B G R B] [G R B G] [R B G R]
      // [B G R X] [B G R X] [B G R X] [B G R X]

      uint32_t s0 = ((const uint32_t*)src)[0];
      uint32_t s1 = ((const uint32_t*)src)[1];
      uint32_t s2 = ((const uint32_t*)src)[2];

      ((uint32_t*)dst)[0] = (s0      )              | 0xFF000000U;
      ((uint32_t*)dst)[1] = (s0 >> 24) | (s1 <<  8) | 0xFF000000U;
      ((uint32_t*)dst)[2] = (s1 >> 16) | (s2 << 16) | 0xFF000000U;
      ((uint32_t*)dst)[3] =              (s2 >>  8) | 0xFF000000U;

      dst += 16;
      src += 12;
      w -= 4;
    }
#endif // FOG_BYTE_ORDER

    while (w)
    {
      ((uint32_t*)dst)[0] = RGB24_NATIVE_IO::fetch(src) | 0xFF000000U;
      dst += 4;
      src += 3;
      w--;
    }
  }

  static void FOG_FASTCALL argb32_from_prgb32(
    uint8_t* _dst, const uint8_t* _src, int w, const RasterClosure* closure)
  {
    uint32_t* dst = reinterpret_cast<uint32_t*>(_dst);
    const uint32_t* src = reinterpret_cast<const uint32_t*>(_src);

    int i = 0;

    while (i < w)
    {
      // We want to identify long runs of constant input pixels and
      // cache the unpremultiplied.
      uint32_t const_in, const_out;

      // Diff is the or of all bitwise differences from const_in
      // during the probe period.  If it is zero after the probe
      // period then every input pixel was identical in the
      // probe.
      unsigned diff = 0;

      // Accumulator for all alphas of the probe period pixels,
      // biased to make the sum zero if the
      unsigned accu = -2*255;

      {
        uint32_t rgba, a, r, g, b, recip;

        rgba = const_in = src[i];
        a = (rgba >> 24);
        accu += a;
        r = (rgba >> 16) & 0xFFU;
        g = (rgba >> 8 ) & 0xFFU;
        b = (rgba      ) & 0xFFU;
        recip = _render_demultiply_reciprocal_table_d[a];

        r = ((r * recip)      ) & 0x00FF0000;
        g = ((g * recip) >>  8) & 0x0000FF00;
        b = ((b * recip) >> 16) & 0x000000FFU;

        dst[i] = const_out = r | g | b | (a << 24);
      }

      if (++i == w) return;

      {
        uint32_t rgba, a, r, g, b, recip;
        rgba = src[i];
        a = (rgba >> 24);
        accu += a;
        r = (rgba >> 16) & 0xFFU;
        g = (rgba >>  8) & 0xFFU;
        b = (rgba      ) & 0xFFU;
        recip = _render_demultiply_reciprocal_table_d[a];

        diff = rgba ^ const_in;

        r = ((r * recip)      ) & 0x00FF0000;
        g = ((g * recip) >>  8) & 0x0000FF00;
        b = ((b * recip) >> 16) & 0x000000FFU;

        dst[i] = r | g | b | (a << 24);
      }

      if (++i == w) return;

      // Fall into special cases if we have special circumstances.
      if (0 != (accu & diff)) continue;

      if (0 == accu)
      {
        // a run of solid pixels.
        uint32_t in;
        while (0xFF000000U == ((in = src[i]) & 0xFF000000U))
        {
          dst[i] = in;
          if (++i == w) return;
        }
      }
      else if (0 == diff)
      {
        // a run of constant pixels.
        while (src[i] == const_in)
        {
          dst[i] = const_out;
          if (++i == w) return;
        }
      }
    }
  }

  static void FOG_FASTCALL rgb24_from_xrgb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
    // Align.
    while (w && (size_t)dst & 0x3)
    {
      uint32_t s0 = ((const uint32_t*)src)[0];

      dst[RGB24_NATIVE_RBYTE] = (uint8_t)(s0 >> 16);
      dst[RGB24_NATIVE_GBYTE] = (uint8_t)(s0 >> 8);
      dst[RGB24_NATIVE_BBYTE] = (uint8_t)(s0);

      dst += 3;
      src += 4;
      w--;
    }

    // 4 Pixels per time.
    while (w >= 4)
    {
      // Following table illustrates how this works on Little Endian:
      //
      // [B G R X] [B G R X] [B G R X] [B G R X]
      // [B G R B] [G R B G] [R B G R]
      uint32_t s0 = ((const uint32_t*)src)[0];
      uint32_t s1 = ((const uint32_t*)src)[1];
      uint32_t s2 = ((const uint32_t*)src)[2];
      uint32_t s3 = ((const uint32_t*)src)[3];

      ((uint32_t*)dst)[0] = (s0 & 0x00FFFFFFU) | (s1 << 24); s1 >>= 8;
      ((uint32_t*)dst)[1] = (s1 & 0x0000FFFFU) | (s2 << 16); s2 >>= 16;
      ((uint32_t*)dst)[2] = (s2 & 0x000000FFU) | (s3 <<  8);

      dst += 12;
      src += 16;
      w -= 4;
    }
#endif // FOG_BYTE_ORDER

    while (w)
    {
      uint32_t s0 = ((const uint32_t*)src)[0];

      dst[RGB24_NATIVE_RBYTE] = (uint8_t)(s0 >> 16);
      dst[RGB24_NATIVE_GBYTE] = (uint8_t)(s0 >> 8);
      dst[RGB24_NATIVE_BBYTE] = (uint8_t)(s0);

      dst += 3;
      src += 4;
      w--;
    }
  }

  static void FOG_FASTCALL rgb24_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
    // Align.
    while (w && (size_t)dst & 0x3)
    {
      uint32_t s0 = ColorUtil::premultiply(((const uint32_t*)src)[0]);

      dst[RGB24_NATIVE_RBYTE] = (uint8_t)(s0 >> 16);
      dst[RGB24_NATIVE_GBYTE] = (uint8_t)(s0 >> 8);
      dst[RGB24_NATIVE_BBYTE] = (uint8_t)(s0);

      dst += 3;
      src += 4;
      w--;
    }

    // 4 Pixels per time.
    while (w >= 4)
    {
      // Following table illustrates how this works on Little Endian:
      //
      // [B G R X] [B G R X] [B G R X] [B G R X]
      // [B G R B] [G R B G] [R B G R]
      uint32_t s0 = ColorUtil::premultiply(((const uint32_t*)src)[0]);
      uint32_t s1 = ColorUtil::premultiply(((const uint32_t*)src)[1]);
      uint32_t s2 = ColorUtil::premultiply(((const uint32_t*)src)[2]);
      uint32_t s3 = ColorUtil::premultiply(((const uint32_t*)src)[3]);

      ((uint32_t*)dst)[0] = (s0 & 0x00FFFFFFU) | (s1 << 24); s1 >>= 8;
      ((uint32_t*)dst)[1] = (s1 & 0x0000FFFFU) | (s2 << 16); s2 >>= 16;
      ((uint32_t*)dst)[2] = (s2 & 0x000000FFU) | (s3 <<  8);

      dst += 12;
      src += 16;
      w -= 4;
    }
#endif // FOG_BYTE_ORDER

    while (w)
    {
      uint32_t s0 = ColorUtil::premultiply(((const uint32_t*)src)[0]);

      dst[RGB24_NATIVE_RBYTE] = (uint8_t)(s0 >> 16);
      dst[RGB24_NATIVE_GBYTE] = (uint8_t)(s0 >> 8);
      dst[RGB24_NATIVE_BBYTE] = (uint8_t)(s0);

      dst += 3;
      src += 4;
      w--;
    }
  }

  static void FOG_FASTCALL rgb24_from_i8(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    const uint32_t* srcPal = closure->srcPalette + ImagePalette::INDEX_PRGB32;

#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
    // Align.
    while (w && (size_t)dst & 0x3)
    {
      uint32_t s0 = srcPal[src[0]];

      dst[RGB24_NATIVE_RBYTE] = (uint8_t)(s0 >> 16);
      dst[RGB24_NATIVE_GBYTE] = (uint8_t)(s0 >> 8);
      dst[RGB24_NATIVE_BBYTE] = (uint8_t)(s0);

      dst += 3;
      src += 1;
      w--;
    }

    // 4 Pixels per time.
    while (w >= 4)
    {
      // Following table illustrates how this works on Little Endian:
      //
      // [X R G B] [X R G B] [X R G B] [X R G B]
      // [B G R B] [G R B G] [R B G R]
      uint32_t s0 = srcPal[src[0]];
      uint32_t s1 = srcPal[src[1]];
      uint32_t s2 = srcPal[src[2]];
      uint32_t s3 = srcPal[src[3]];

      ((uint32_t*)dst)[0] = (s0 & 0x00FFFFFFU) | (s1 << 24); s1 >>= 8;
      ((uint32_t*)dst)[1] = (s1 & 0x0000FFFFU) | (s2 << 16); s2 >>= 16;
      ((uint32_t*)dst)[2] = (s2 & 0x000000FFU) | (s3 <<  8);

      dst += 12;
      src += 4;
      w -= 4;
    }
#endif // FOG_BYTE_ORDER

    while (w)
    {
      uint32_t s0 = srcPal[src[0]];

      dst[RGB24_NATIVE_RBYTE] = (uint8_t)(s0 >> 16);
      dst[RGB24_NATIVE_GBYTE] = (uint8_t)(s0 >> 8);
      dst[RGB24_NATIVE_BBYTE] = (uint8_t)(s0);

      dst += 3;
      src += 1;
      w--;
    }
  }

  // Greyscale from RGB conversion.
  //
  // This formula is used:
  //  0.212671 * R + 0.715160 * G + 0.072169 * B;
  // As:
  //  (13938 * R + 46868 * G + 4730 * B) / 65536

  static void FOG_FASTCALL grey8_from_xrgb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t s0 = ((const uint32_t*)src)[0];
      uint32_t grey = ((s0 >> 16) & 0xFFU) * 13938 +
                      ((s0 >>  8) & 0xFFU) * 46868 +
                      ((s0      ) & 0xFFU) *  4730 ;
      dst[0] = (uint8_t)(grey >> 16);

      dst += 1;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL grey8_from_argb32(
    uint8_t* dst, const uint8_t* src, int w, const RasterClosure* closure)
  {
    do {
      uint32_t s0 = ColorUtil::premultiply(((const uint32_t*)src)[0]);
      uint32_t grey = ((s0 >> 16) & 0xFFU) * 13938 +
                      ((s0 >>  8) & 0xFFU) * 46868 +
                      ((s0      ) & 0xFFU) *  4730 ;
      dst[0] = (uint8_t)(grey >> 16);

      dst += 1;
      src += 4;
    } while (--w);
  }

  // --------------------------------------------------------------------------
  // [DibC - Dither]
  // --------------------------------------------------------------------------

  static void FOG_FASTCALL i8rgb232_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin, const uint8_t* palConv)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, d, dith2, dith3;

      d = dt[dx & DitherTable::MASK];

      dith2 = DitherTable::shf_dither(d, DitherTable::shf_arg(2));
      dith3 = DitherTable::shf_dither(d, DitherTable::shf_arg(3));

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      if (((r0 & 0x3F) >= dith2) && (r0 < 0xC0)) r0 += 64;
      if (((g0 & 0x1F) >= dith3) && (g0 < 0xE0)) g0 += 32;
      if (((b0 & 0x3F) >= dith2) && (b0 < 0xC0)) b0 += 64;

      ((uint8_t *)dst)[0] = palConv[((r0 & 0xC0) >> 1) | ((g0 & 0xE0) >> 3) | ((b0 & 0xC0) >> 6)];

      dx++;
      dst += 1;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL i8rgb222_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin, const uint8_t* palConv)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, dith2;
      uint32_t r0t, g0t, b0t;

      dith2 = DitherTable::shf_dither(dt[dx & DitherTable::MASK], DitherTable::shf_arg(2)) * 4 / 3;

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      // c = c / 85
      r0t = (r0 * 0x00030000) >> 16;
      g0t = (g0 * 0x00030000) >> 16;
      b0t = (b0 * 0x00030000) >> 16;

      if ((r0 - (r0t * 85)) > dith2) r0t++;
      if ((g0 - (g0t * 85)) > dith2) g0t++;
      if ((b0 - (b0t * 85)) > dith2) b0t++;
      ((uint8_t *)dst)[0] = palConv[(r0t<<4)|(g0t<<2)|(b0t)];

      dx++;
      dst += 1;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL i8rgb111_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin, const uint8_t* palConv)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, dith1;

      dith1 = DitherTable::shf_dither(dt[dx & DitherTable::MASK], DitherTable::shf_arg(0));

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      if (r0 > dith1) r0 = 255;
      if (g0 > dith1) g0 = 255;
      if (b0 > dith1) b0 = 255;

      ((uint8_t *)dst)[0] = palConv[
        (((r0 + 1) >> 8) << 2) |
        (((g0 + 1) >> 8) << 1) |
        (((b0 + 1) >> 8)     ) ];

      dx++;
      dst += 1;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_555_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, dith;

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      dith = DitherTable::shf_dither(dt[dx & DitherTable::MASK], DitherTable::shf_arg(5));
      if (((r0 & 7) >= dith) && (r0 < 0xF8)) r0 += 8;
      if (((g0 & 7) >= dith) && (g0 < 0xF8)) g0 += 8;
      if (((b0 & 7) >= dith) && (b0 < 0xF8)) b0 += 8;

      ((uint16_t *)dst)[0] = ((r0 & 0xF8) << 7) | ((g0 & 0xF8) << 2) | ((b0 & 0xF8) >> 3);

      dx++;
      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_565_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, d, dith5, dith6;

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      d = dt[dx & DitherTable::MASK];
      dith5 = DitherTable::shf_dither(d, DitherTable::shf_arg(5));
      dith6 = DitherTable::shf_dither(d, DitherTable::shf_arg(6));
      if (((r0 & 7) >= dith5) && (r0 < 0xF8)) r0 += 8;
      if (((g0 & 3) >= dith6) && (g0 < 0xFC)) g0 += 4;
      if (((b0 & 7) >= dith5) && (b0 < 0xF8)) b0 += 8;

      ((uint16_t *)dst)[0] = ((r0 & 0xF8) << 8) | ((g0 & 0xFC) << 3) | ((b0 & 0xF8) >> 3);

      dx++;
      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_555_swapped_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, dith;

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      dith = DitherTable::shf_dither(dt[dx & DitherTable::MASK], DitherTable::shf_arg(5));
      if (((r0 & 7) >= dith) && (r0 < 0xF8)) r0 += 8;
      if (((g0 & 7) >= dith) && (g0 < 0xF8)) g0 += 8;
      if (((b0 & 7) >= dith) && (b0 < 0xF8)) b0 += 8;

      ((uint16_t *)dst)[0] = MemOps::bswap16(((r0 & 0xF8) << 7) | ((g0 & 0xF8) << 2) | ((b0 & 0xF8) >> 3));

      dx++;
      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL rgb16_565_swapped_from_xrgb32_dither(
    uint8_t* dst, const uint8_t* src, int w, const PointI& origin)
  {
    const uint8_t* dt = DitherTable::matrix[origin.getY() & DitherTable::MASK];
    int dx = origin.getX();

    do {
      uint32_t c0;
      uint32_t r0, g0, b0, d, dith5, dith6;

      c0 = ((uint32_t *)src)[0];
      r0 = (c0 >> 16) & 0xFF;
      g0 = (c0 >>  8) & 0xFF;
      b0 = (c0      ) & 0xFF);

      d = dt[dx & DitherTable::MASK];
      dith5 = DitherTable::shf_dither(d, DitherTable::shf_arg(5));
      dith6 = DitherTable::shf_dither(d, DitherTable::shf_arg(6));
      if (((r0 & 7) >= dith5) && (r0 < 0xF8)) r0 += 8;
      if (((g0 & 3) >= dith6) && (g0 < 0xFC)) g0 += 4;
      if (((b0 & 7) >= dith5) && (b0 < 0xF8)) b0 += 8;

      ((uint16_t *)dst)[0] = MemOps::bswap16(((r0 & 0xF8) << 8) | ((g0 & 0xFC) << 3) | ((b0 & 0xF8) >> 3));

      dx++;
      dst += 2;
      src += 4;
    } while (--w);
  }

  static void FOG_FASTCALL prgb32_from_argb32_span(
    uint8_t* _dst, const RasterSpan* span, const RasterClosure* closure)
  {
    uint32_t* pixels = reinterpret_cast<uint32_t*>(_dst);

    do {
      uint x0 = (uint)span->getX0();
      uint x2 = (uint)span->getX1();

      while (x1 < x2)
      {
        uint32_t pix0 = pixels[x1];

        if (!Acc::p32ARGB32IsAlphaFF(pix0))
          pixels[x1] = ColorUtil::premultiply(pix0);
        x1++;
      }
    } while ((span = span->getNext()) != NULL);
  }

  static void FOG_FASTCALL argb32_from_prgb32_span(
    uint8_t* _dst, const RasterSpan* span, const RasterClosure* closure)
  {
    uint32_t* pixels = reinterpret_cast<uint32_t*>(_dst);

    do {
      uint x0 = (uint)span->getX0();
      uint x1 = (uint)span->getX1();

      while (x0 < x1)
      {
        // We want to identify long runs of constant input pixels and
        // cache the unpremultiplied.
        uint32_t const_in, const_out;

        // Diff is the or of all bitwise differences from const_in
        // during the probe period.  If it is zero after the probe
        // period then every input pixel was identical in the
        // probe.
        unsigned diff = 0;

        // Accumulator for all alphas of the probe period pixels,
        // biased to make the sum zero if the
        unsigned accu = -2*255;

        {
          uint32_t rgba, a, r, g, b, recip;

          rgba = const_in = pixels[x1];
          a = (rgba >> 24);
          accu += a;

          r = (rgba >> 16) & 0xFFU;
          g = (rgba >> 8 ) & 0xFFU;
          b = (rgba      ) & 0xFFU;
          recip = _render_demultiply_reciprocal_table_d[a];

          r = ((r * recip)      ) & 0x00FF0000U;
          g = ((g * recip) >>  8) & 0x0000FF00U;
          b = ((b * recip) >> 16) & 0x000000FFU;

          pixels[x0] = const_out = _FOG_ACC_COMBINE_4(r, g, b, a << 24);
        }

        if (++x0 == x1) break;

        {
          uint32_t rgba, a, r, g, b, recip;
          rgba = pixels[x1];
          a = (rgba >> 24);
          accu += a;

          r = (rgba >> 16) & 0xFFU;
          g = (rgba >>  8) & 0xFFU;
          b = (rgba      ) & 0xFFU;
          recip = _render_demultiply_reciprocal_table_d[a];

          diff = rgba ^ const_in;

          r = ((r * recip)      ) & 0x00FF0000U;
          g = ((g * recip) >>  8) & 0x0000FF00U;
          b = ((b * recip) >> 16) & 0x000000FFU;

          pixels[x1] = _FOG_ACC_COMBINE_4(r, g, b, a << 24);
        }

        if (++x0 == x1) break;

        // Fall into special cases if we have special circumstances.
        if (0 != (accu & diff)) continue;

        if (0 == accu)
        {
          // a run of solid pixels.
          uint32_t in;
          while (0xFF000000U == ((in = pixels[x1]) & 0xFF000000U))
          {
            pixels[x1] = in;
            if (++x1 == x2) goto _NextSpan;
          }
        }
        else if (0 == diff)
        {
          // a run of constant pixels.
          while (pixels[x0] == const_in)
          {
            pixels[x0] = const_out;
            if (++x0 == x1) goto _NextSpan;
          }
        }
      }
_NextSpan:
      ;
    } while ((span = span->getNext()) != NULL);
  }
};

#endif // 0

} // RasterOps_C namespace
} // Fog namespace

#endif // _FOG_G2D_PAINTING_RASTEROPS_C_BASECONVERT_P_H
