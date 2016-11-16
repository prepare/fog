// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_C_BASEACCESS_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_C_BASEACCESS_P_H

// [Dependencies]
#include <Fog/Core/Acc/AccC.h>

namespace Fog {
namespace RasterOps_C {

// ============================================================================
// [Fog::RasterOps_C - PixelPRGB32]
// ============================================================================

struct FOG_NO_EXPORT PixelPRGB32
{
  enum { SIZE = 4, HAS_RGB = 1, HAS_ALPHA = 1, HAS_X = 0, HAS_F = 0, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load4a(dst0p, src);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
    Acc::p32Load4a(dst0p_20, src);
    Acc::p32UnpackPBWFromPBB_2031(dst0p_20, dst0p_31, dst0p_20);
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    Acc::p32Store4a(dst, src0p);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
    uint32_t src0p;

    Acc::p32PackPBB2031FromPBW(src0p, src0p_20, src0p_31);
    Acc::p32Store4a(dst, src0p);
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load1b(dst0p, static_cast<const uint8_t*>(src) + PIXEL_ARGB32_POS_A);
  }

  static FOG_INLINE void p32StorePixelA8(void* dst, const uint32_t& src0p)
  {
    uint32_t a0p;
    Acc::p32ExtendPBBFromSBB(a0p, src0p);
    Acc::p32Store4a(dst, a0p);
  }
};

// ============================================================================
// [Fog::RasterOps_C - PixelXRGB32]
// ============================================================================

struct FOG_NO_EXPORT PixelXRGB32
{
  enum { SIZE = 4, HAS_RGB = 1, HAS_ALPHA = 0, HAS_X = 1, HAS_F = 0, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load4a(dst0p, src);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
    Acc::p32Load4a(dst0p_20, src);
    Acc::p32UnpackPBWFromPBB_2031(dst0p_20, dst0p_31, dst0p_20);
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    Acc::p32Store4a(dst, src0p);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
    uint32_t src0p;

    Acc::p32PackPBB2031FromPBW(src0p, src0p_20, src0p_31);
    Acc::p32Store4a(dst, src0p);
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
    dst0p = 0xFF;
  }
};

// ============================================================================
// [Fog::RasterOps_C - PixelFRGB32]
// ============================================================================

struct FOG_NO_EXPORT PixelFRGB32
{
  enum { SIZE = 4, HAS_RGB = 1, HAS_ALPHA = 0, HAS_X = 0, HAS_F = 1, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load4a(dst0p, src);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
    Acc::p32Load4a(dst0p_20, src);
    Acc::p32UnpackPBWFromPBB_2031(dst0p_20, dst0p_31, dst0p_20);
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    Acc::p32Store4a(dst, src0p);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
    uint32_t src0p;

    Acc::p32PackPBB2031FromPBW(src0p, src0p_20, src0p_31);
    Acc::p32Store4a(dst, src0p);
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
    dst0p = 0xFF;
  }
};

// ============================================================================
// [Fog::RasterOps_C - PixelRGB24]
// ============================================================================

struct FOG_NO_EXPORT PixelRGB24
{
  enum { SIZE = 3, HAS_RGB = 1, HAS_ALPHA = 0, HAS_X = 0, HAS_F = 0, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load3b(dst0p, src);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
    dst0p_20  = (static_cast<uint32_t>(static_cast<const uint8_t*>(src)[PIXEL_RGB24_BYTE_R]) << 16);
    dst0p_31  = (static_cast<uint32_t>(static_cast<const uint8_t*>(src)[PIXEL_RGB24_BYTE_G])      );
    dst0p_20 += (static_cast<uint32_t>(static_cast<const uint8_t*>(src)[PIXEL_RGB24_BYTE_B])      );
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    Acc::p32Store3b(dst, src0p);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
    static_cast<uint8_t*>(dst)[PIXEL_RGB24_BYTE_B] = static_cast<uint8_t>(src0p_20);
    static_cast<uint8_t*>(dst)[PIXEL_RGB24_BYTE_G] = static_cast<uint8_t>(src0p_31);
    static_cast<uint8_t*>(dst)[PIXEL_RGB24_BYTE_R] = static_cast<uint8_t>(src0p_20 >> 16);
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
    dst0p = 0xFF;
  }
};

// ============================================================================
// [Fog::RasterOps_C - PixelA8]
// ============================================================================

struct FOG_NO_EXPORT PixelA8
{
  enum { SIZE = 1, HAS_RGB = 0, HAS_ALPHA = 1, HAS_X = 0, HAS_F = 0, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load1b(dst0p, src);
    Acc::p32ExtendPBBFromSBB(dst0p, dst0p);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
    Acc::p32Load1b(dst0p_20, src);

    Acc::p32ExtendPBWFromSBW(dst0p_20, dst0p_20);
    Acc::p32Copy(dst0p_31, dst0p_20);
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    uint32_t a0p = src0p >> 24;
    Acc::p32Store1b(dst, a0p);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
    static_cast<uint8_t*>(dst)[0] = static_cast<uint8_t>(src0p_31 >> 16);
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load1b(dst0p, static_cast<const uint8_t*>(src));
  }

  static FOG_INLINE void p32StorePixelA8(void* dst, const uint32_t& src0p)
  {
    Acc::p32Store1b(static_cast<uint8_t*>(dst), src0p);
  }
};

// ============================================================================
// [Fog::RasterOps_C - PixelPRGB64]
// ============================================================================

struct FOG_NO_EXPORT PixelPRGB64
{
  enum { SIZE = 8, HAS_RGB = 1, HAS_ALPHA = 1, HAS_X = 0, HAS_F = 0, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    uint32_t src0p_10, src0p_32;

    Acc::p32Load8a(src0p_10, src0p_32, src);
    Acc::p32ARGB32FromARGB64_1032(dst0p, src0p_10, src0p_32);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
#if defined(FOG_ARCH_NATIVE_P64)
    __p64 dst0p_lo;
    __p64 dst0p_hi;

    Acc::p64Load8a(dst0p_lo, src);
    Acc::p64Copy(dst0p_hi, dst0p_lo);

    Acc::p64And(dst0p_lo, dst0p_lo, FOG_UINT64_C(0x0000FF000000FF00));
    Acc::p64And(dst0p_hi, dst0p_hi, FOG_UINT64_C(0xFF000000FF000000));

    dst0p_20 = static_cast<uint32_t>(_FOG_ACC_COMBINE_2(dst0p_lo >> 24, dst0p_lo >>  8));
    dst0p_31 = static_cast<uint32_t>(_FOG_ACC_COMBINE_2(dst0p_hi >> 56, dst0p_hi >> 24));
#else
    uint32_t dst0p_lo;
    uint32_t dst0p_hi;

    Acc::p32Load8a(dst0p_lo, dst0p_hi, src);

    Acc::p32And(dst0p_lo, dst0p_lo, 0xFF00FF00);
    Acc::p32And(dst0p_hi, dst0p_hi, 0xFF00FF00);

    dst0p_20 = _FOG_ACC_COMBINE_2((dst0p_lo >> 8) & 0xFF, (dst0p_hi <<  8));
    dst0p_31 = _FOG_ACC_COMBINE_2((dst0p_hi >> 8) & 0xFF, (dst0p_lo >> 24));
#endif
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    uint32_t src0p_10, src0p_32;

    Acc::p32PRGB64FromPRGB32(src0p_10, src0p_32, src0p);
    Acc::p32Store8a(dst, src0p_10, src0p_32);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
#if defined(FOG_ARCH_NATIVE_P64)
    uint32_t t0 = _FOG_ACC_COMBINE_2(src0p_20 & 0x000000FF, src0p_31 << 16);
    uint32_t t1 = _FOG_ACC_COMBINE_2(src0p_20 >> 16, src0p_31 & 0x00FF0000); 

    __p64 dst0p = static_cast<uint64_t>(t0) + (static_cast<uint64_t>(t1) << 32);
    Acc::p64Store8a(dst, dst0p);
#else
    uint32_t t0 = src0p_20 * 0x0101;
    uint32_t t1 = src0p_31 * 0x0101;
    uint32_t t2 = _FOG_ACC_COMBINE_2(src0p_20 & 0xFFFF, src0p_31 << 16);
    uint32_t t3 = _FOG_ACC_COMBINE_2(src0p_20 >> 16, src0p_31 & 0xFFFF0000);

    Acc::p32Store8a(dst, t2, t3);
#endif
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
    Acc::p32Load1b(dst0p, static_cast<const uint8_t*>(src) + PIXEL_ARGB64_BYTE_A_HI);
  }

  static FOG_INLINE void p32StorePixelA8(void* dst, const uint32_t& src0p)
  {
#if defined(FOG_ARCH_NATIVE_P64)
    __p64 a0p;

    Acc::p64ExtendPBBFromSBB(a0p, static_cast<uint64_t>(src0p));
    Acc::p64Store8a(static_cast<uint8_t*>(dst) + 0, a0p);
#else
    uint32_t a0p;

    Acc::p32ExtendPBBFromSBB(a0p, src0p);
    Acc::p32Store4a(static_cast<uint8_t*>(dst) + 0, a0p);
    Acc::p32Store4a(static_cast<uint8_t*>(dst) + 4, a0p);
#endif
  }
};

// ============================================================================
// [Fog::RasterOps_C - PixelA16]
// ============================================================================

struct FOG_NO_EXPORT PixelA16
{
  enum { SIZE = 1, HAS_RGB = 0, HAS_ALPHA = 1, HAS_X = 0, HAS_F = 0, IS_PREMULTIPLIED = 1 };

  // ==========================================================================
  // [Pixel32]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixel32(uint32_t& dst0p, const void* src)
  {
    p32LoadPixelA8(dst0p, src);
    Acc::p32ExtendPBBFromSBB(dst0p, dst0p);
  }

  static FOG_INLINE void p32LoadPixel32_2031(uint32_t& dst0p_20, uint32_t& dst0p_31, const void* src)
  {
    p32LoadPixelA8(dst0p_20, src);

    Acc::p32ExtendPBWFromSBW(dst0p_20, dst0p_20);
    Acc::p32Copy(dst0p_31, dst0p_20);
  }

  static FOG_INLINE void p32StorePixel32(void* dst, const uint32_t& src0p)
  {
    uint32_t a0p = src0p >> 24;
    Acc::p32ExtendPBBFromSBB_ZZ10(a0p, a0p);
    Acc::p32Store2a(dst, a0p);
  }

  static FOG_INLINE void p32StorePixel32_2031(void* dst, const uint32_t& src0p_20, const uint32_t& src0p_31)
  {
    uint32_t a0p = src0p_31 >> 16;
    Acc::p32ExtendPBBFromSBB_ZZ10(a0p, a0p);
    Acc::p32Store2a(dst, a0p);
  }

  // ==========================================================================
  // [PixelA8]
  // ==========================================================================

  static FOG_INLINE void p32LoadPixelA8(uint32_t& dst0p, const void* src)
  {
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
    Acc::p32Load1b(dst0p, static_cast<const uint8_t*>(src) + 1);
#else
    Acc::p32Load1b(dst0p, static_cast<const uint8_t*>(src) + 0);
#endif
  }

  static FOG_INLINE void p32StorePixelA8(void* dst, const uint32_t& src0p)
  {
    uint32_t a0p;

    Acc::p32ExtendPBBFromSBB_ZZ10(a0p, src0p);
    Acc::p32Store2a(static_cast<uint8_t*>(dst), a0p);
  }
};

} // RasterOps_C namespace
} // Fog namespace

// [Guard]
#endif // _FOG_G2D_PAINTING_RASTEROPS_C_BASEACCESS_P_H
