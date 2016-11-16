// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_C_BASEDEFS_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_C_BASEDEFS_P_H

// [Dependencies]
#include <Fog/Core/Acc/AccC.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/Core/Memory/BSwap.h>
#include <Fog/Core/Memory/MemBuffer.h>
#include <Fog/Core/Memory/MemBufferTmp_p.h>
#include <Fog/Core/Memory/MemMgr.h>
#include <Fog/Core/Memory/MemOps.h>
#include <Fog/Core/Tools/Logger.h>
#include <Fog/Core/Tools/Swap.h>
#include <Fog/G2d/Acc/AccC.h>
#include <Fog/G2d/Geometry/Math2d.h>
#include <Fog/G2d/Geometry/Point.h>
#include <Fog/G2d/Imaging/Image.h>
#include <Fog/G2d/Imaging/ImageFilter.h>
#include <Fog/G2d/Imaging/ImageFilterScale.h>
#include <Fog/G2d/Imaging/ImagePalette.h>
#include <Fog/G2d/Imaging/Filters/FeBlur.h>
#include <Fog/G2d/Imaging/Filters/FeColorLut.h>
#include <Fog/G2d/Imaging/Filters/FeColorMatrix.h>
#include <Fog/G2d/Imaging/Filters/FeComponentTransfer.h>
#include <Fog/G2d/Imaging/Filters/FeConvolveMatrix.h>
#include <Fog/G2d/Imaging/Filters/FeConvolveSeparable.h>
#include <Fog/G2d/Imaging/Filters/FeMorphology.h>
#include <Fog/G2d/Painting/RasterApi_p.h>
#include <Fog/G2d/Painting/RasterSpan_p.h>
#include <Fog/G2d/Painting/RasterConstants_p.h>
#include <Fog/G2d/Painting/RasterStructs_p.h>
#include <Fog/G2d/Painting/RasterUtil_p.h>
#include <Fog/G2d/Source/Color.h>
#include <Fog/G2d/Source/ColorStop.h>
#include <Fog/G2d/Source/ColorStopCache.h>
#include <Fog/G2d/Source/ColorStopList.h>
#include <Fog/G2d/Source/ColorUtil.h>
#include <Fog/G2d/Source/Gradient.h>
#include <Fog/G2d/Source/Pattern.h>
#include <Fog/G2d/Source/Texture.h>

// [Dependencies - RasterOps_C]
#include <Fog/G2d/Painting/RasterOps_C/BaseAccess_p.h>

// ============================================================================
// [FOG_CBLIT_SPAN8]
// ============================================================================

// Macros to help creating blit functions.
//
// Usage:
//
//    FOG_CBLIT_SPAN8_BEGIN(bytes_per_pixel)
//
//    -------------------------------------------------------------------------
//
//    FOG_CBLIT_SPAN8_C_ANY()
//    {
//      'dst'  - The destination pointer.
//      'msk0' - The const-mask value.
//      'w'    - The number of pixels to process (width).
//    }
//
//      or
//
//    FOG_CBLIT_SPAN8_C_OPAQUE()
//    {
//      'dst'  - The destination pointer.
//      'w'    - The number of pixels to process (width).
//    }
//    FOG_CBLIT_SPAN8_C_MASK()
//    {
//      'dst'  - The destination pointer.
//      'msk0' - The const-mask value.
//      'w'    - The number of pixels to process (width).
//    }
//
//    -------------------------------------------------------------------------
//
//    FOG_CBLIT_SPAN8_A8_GLYPH()
//    {
//      'dst'  - The destination pointer.
//      'msk'  - The a8-mask pointer.
//      'w'    - The number of pixels to process (width).
//    }
//
//    -------------------------------------------------------------------------
//
//    FOG_CBLIT_SPAN8_A8_EXTRA()
//    {
//      'dst'  - The destination pointer.
//      'msk'  - The a8-extended-mask pointer. Omit checks for fully-transparent
//               and fully-opaque pixels, it happens rarely.
//      'w'    - The number of pixels to process (width).
//    }
//
//    -------------------------------------------------------------------------
//
//    FOG_CBLIT_SPAN8_ARGB32_GLYPH()
//    {
//      'dst'  - The destination pointer.
//      'msk'  - The argb32-mask pointer.
//      'w'    - The number of pixels to process (width).
//    }
//
//    -------------------------------------------------------------------------
//
//    FOG_CBLIT_SPAN8_END()
//
// The FOG_VBLIT_SPAN8_XXX functions are used when there is variable-source. The
// 'src' variable should be used inside of 'CASE' sections.

//! @brief CBlit - RasterSpan8 - Begin.
#define FOG_CBLIT_SPAN8_BEGIN(_Bpp_) \
  uint8_t* dstBase = dst; \
  \
  do { \
    int x = (uint)span->getX0(); \
    int w = (int)(span->getX1() - x); \
    FOG_ASSUME(w > 0); \
    \
    dst = dstBase + (size_t)(uint)x * _Bpp_; \
    const uint8_t* _msk = (const uint8_t*)reinterpret_cast<const RasterSpan8*>(span)->getGenericMask(); \
    \
    switch (span->getType()) \
    {

// ----------------------------------------------------------------------------

//! @brief CBlit - RasterSpan8 - C-Any.
#define FOG_CBLIT_SPAN8_C_ANY() \
      case RASTER_SPAN_C: \
      { \
        uint32_t msk0 = RasterSpan8::getConstMaskFromPointer(_msk); \
        FOG_ASSUME(msk0 <= 0x100); \
        {

//! @brief CBlit - RasterSpan8 - C-Opaque.
#define FOG_CBLIT_SPAN8_C_OPAQUE() \
      case RASTER_SPAN_C: \
      { \
        uint32_t msk0 = RasterSpan8::getConstMaskFromPointer(_msk); \
        FOG_ASSUME(msk0 <= 0x100); \
        \
        if (msk0 == 0x100) \
        {

//! @brief CBlit - RasterSpan8 - C-Mask.
#define FOG_CBLIT_SPAN8_C_MASK() \
        } \
        else \
        {

// ----------------------------------------------------------------------------

//! @brief CBlit - RasterSpan8 - A8-Glyph.
#define FOG_CBLIT_SPAN8_A8_GLYPH() \
        } \
        break; \
      } \
      \
      case RASTER_SPAN_A8_GLYPH: \
      case RASTER_SPAN_AX_GLYPH: \
      { \
        const uint8_t* msk = _msk;

// ----------------------------------------------------------------------------

//! @brief CBlit - RasterSpan8 - A8-Extra.
#define FOG_CBLIT_SPAN8_A8_EXTRA() \
        break; \
      } \
      \
      case RASTER_SPAN_AX_EXTRA: \
      { \
        const uint8_t* msk = _msk;

// ----------------------------------------------------------------------------

//! @brief CBlit - RasterSpan8 - ARGB32-Glyph.
#define FOG_CBLIT_SPAN8_ARGB32_GLYPH() \
        break; \
      } \
      \
      case RASTER_SPAN_ARGB32_GLYPH: \
      case RASTER_SPAN_ARGBXX_GLYPH: \
      { \
        const uint8_t* msk = _msk;

// ----------------------------------------------------------------------------

//! @brief CBlit - End.
#define FOG_CBLIT_SPAN8_END() \
        break; \
      } \
    } \
  } while ((span = span->getNext()) != NULL);



// ============================================================================
// [FOG_VBLIT_SPAN8]
// ============================================================================

//! @brief VBlit - RasterSpan8 - Begin.
#define FOG_VBLIT_SPAN8_BEGIN(_Bpp_) \
  uint8_t* dstBase = dst; \
  \
  do { \
    uint x = (uint)span->getX0(); \
    int w = (int)((uint)span->getX1() - x); \
    FOG_ASSUME(w > 0); \
    \
    dst = dstBase + x * _Bpp_; \
    const uint8_t* _msk = (const uint8_t*)reinterpret_cast<const RasterSpan8*>(span)->getGenericMask(); \
    const uint8_t* src = (const uint8_t*)reinterpret_cast<const RasterSpan8*>(span)->getData(); \
    \
    switch (span->getType()) \
    {

// ----------------------------------------------------------------------------

//! @brief VBlit - RasterSpan8 - C-Any.
#define FOG_VBLIT_SPAN8_C_ANY() \
      case RASTER_SPAN_C: \
      { \
        uint32_t msk0 = RasterSpan8::getConstMaskFromPointer(_msk); \
        FOG_ASSERT(msk0 <= 0x100); \
        {

//! @brief VBlit - RasterSpan8 - C-Opaque.
#define FOG_VBLIT_SPAN8_C_OPAQUE() \
      case RASTER_SPAN_C: \
      { \
        uint32_t msk0 = RasterSpan8::getConstMaskFromPointer(_msk); \
        FOG_ASSERT(msk0 <= 0x100); \
        \
        if (msk0 == 0x100) \
        {

//! @brief VBlit - RasterSpan8 - C-Mask.
#define FOG_VBLIT_SPAN8_C_MASK() \
        } \
        else \
        {

// ----------------------------------------------------------------------------

//! @brief VBlit - RasterSpan8 - A8-Glyph.
#define FOG_VBLIT_SPAN8_A8_GLYPH() \
        } \
        break; \
      } \
      \
      case RASTER_SPAN_A8_GLYPH: \
      case RASTER_SPAN_AX_GLYPH: \
      { \
        const uint8_t* msk = _msk;

// ----------------------------------------------------------------------------

//! @brief VBlit - RasterSpan8 - A8-Extended.
#define FOG_VBLIT_SPAN8_A8_EXTRA() \
        break; \
      } \
      \
      case RASTER_SPAN_AX_EXTRA: \
      { \
        const uint8_t* msk = _msk;

// ----------------------------------------------------------------------------

//! @brief VBlit - RasterSpan8 - ARGB32-Glyph.
#define FOG_VBLIT_SPAN8_ARGB32_GLYPH() \
        break; \
      } \
      \
      case RASTER_SPAN_ARGB32_GLYPH: \
      case RASTER_SPAN_ARGBXX_GLYPH: \
      { \
        const uint8_t* msk = _msk;

// ----------------------------------------------------------------------------

//! @brief VBlit - RasterSpan8 - A8-Glyph which handles both - A8 and ARGB32 in
//! case that the target is A8 (there is no reason to use separate blit loops
//! for these spans).
#define FOG_VBLIT_SPAN8_A8_OR_ARGB32_GLYPH() \
        } \
        break; \
      } \
      \
      case RASTER_SPAN_A8_GLYPH: \
      case RASTER_SPAN_AX_GLYPH: \
      case RASTER_SPAN_ARGB32_GLYPH: \
      case RASTER_SPAN_ARGBXX_GLYPH: \
      { \
        const uint8_t* msk = _msk; \
        uint MskSize = 1; \
        \
        if (span->getType() == RASTER_SPAN_ARGB32_GLYPH || \
            span->getType() == RASTER_SPAN_ARGBXX_GLYPH  ) \
        { \
          MskSize = 4; \
          msk += PIXEL_ARGB32_POS_A; \
        }

// ----------------------------------------------------------------------------

//! @brief VBlit - End.
#define FOG_VBLIT_SPAN8_END() \
        break; \
      } \
    } \
  } while ((span = span->getNext()) != NULL);



// ============================================================================
// [FOG_BLIT_LOOP]
// ============================================================================

// These macros were designed to simplify blit functions. The idea is very simple.
// There are usually three loops per highly optimized blitter. The first loop
// aligns the destination buffer, usually to 16 bytes (for SSE2 processing).
// The second loop (main loop) processes more pixels per iteration (for example
// 4 ARGB pixels, 8 ARGB pixels, 16 A8 pixels, ...). The third loop is similar
// to first loop and its purpose is to process remaining pixels.
//
// There is an idea to merge align and tail loop. This is likely to save some
// binary space (about 1/4 of blitting functions size) and simplify development.
// So do not repeat dirty stuff and use FOG_BLIT_LOOP_... macros.
//
// The two loops are named SMALL and LARGE.
//
// 8-bit entities:
//   - 1 pixel at time:
//     - FOG_BLIT_LOOP_8x1_BEGIN(dst)        - Loop begin.
//     - FOG_BLIT_LOOP_8x1_END(dst)          - Loop end.
//
//   - 4 pixels at time:
//     - FOG_BLIT_LOOP_8x16_SMALL_BEGIN(dst) - Small loop begin.
//     - FOG_BLIT_LOOP_8x16_SMALL_END(dst)   - Small loop end.
//
//   - 16 pixels at time:
//     - FOG_BLIT_LOOP_8x16_MAIN_BEGIN(dst)  - Main loop begin.
//     - FOG_BLIT_LOOP_8x16_MAIN_END(dst)    - Main loop end.
//
// 32-bit entities:
//   - 1 pixel at time:
//     - FOG_BLIT_LOOP_32x4_SMALL_BEGIN(dst) - Small loop begin.
//     - FOG_BLIT_LOOP_32x4_SMALL_END(dst)   - Small loop end.
//
//   - 4 pixels at time:
//     - FOG_BLIT_LOOP_32x4_MAIN_BEGIN(dst)  - Main loop begin.
//     - FOG_BLIT_LOOP_32x4_MAIN_END(dst)    - Main loop end.
//
// Because compilers can be quite missed from our machinery, it's needed
// to follow some rules to help them to optimize this code:
// - declare temporary variables (mainly sse2 registers) in local loop scope.
// - do not add anything between FOG_BLIT_LOOP_32x4_SMALL_END and FOG_BLIT_LOOP_32x4_MAIN_BEGIN.

// ============================================================================
// [FOG_BLIT_LOOP - DstFx1 - DstF per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_DstFx1_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_DstFx1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_DstFx1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_DstFx1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  } \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 8x1 - 8-bits per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_8x1_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_8x1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_8x1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_8x1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  }; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 8x4 - 8-bits per pixel, 4 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_8x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_8x4_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 3) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_8x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 3) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_8x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 3) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_8x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_8x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_8x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 8x8 - 8-bits per pixel, 8 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_8x8_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_8x8_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 7) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_8x8_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_8x8_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_8x8_MAIN_BEGIN(_Group_) \
  w -= 8; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_8x8_MAIN_CONTINUE(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_8x8_MAIN_END(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 8; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 8x16 - 8-bits per pixel, 16 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_8x16_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_8x16_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_8x16_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_8x16_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_8x16_MAIN_BEGIN(_Group_) \
  w -= 16; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_8x16_MAIN_CONTINUE(_Group_) \
    if ((w -= 16) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_8x16_MAIN_END(_Group_) \
    if ((w -= 16) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 16; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 16x1 - 16-bits per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_16x1_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_16x1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_16x1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_16x1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  }; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 16x2 - 16-bits per pixel, 2 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_16x2_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_16x2_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 3) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_16x2_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    break;

#define FOG_BLIT_LOOP_16x2_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_16x2_MAIN_BEGIN(_Group_) \
  w -= 2; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_16x2_MAIN_CONTINUE(_Group_) \
    if ((w -= 2) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_16x2_MAIN_END(_Group_) \
    if ((w -= 2) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 2; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 16x4 - 16-bits per pixel, 4 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_16x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_16x4_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 7) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_16x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_16x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_16x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_16x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_16x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 16x8 - 16-bits per pixel, 8 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_16x8_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_16x8_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_16x8_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_16x8_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_16x8_MAIN_BEGIN(_Group_) \
  w -= 8; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_16x8_MAIN_CONTINUE(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_16x8_MAIN_END(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 8; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 24x1 - 24-bits per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_24x1_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_24x1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_24x1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_24x1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  }; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 24x4 - 24-bits per pixel, 4 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_24x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_24x4_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 3) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_24x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 3) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_24x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 3) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_24x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_24x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_24x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 24x8 - 24-bits per pixel, 8 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_24x8_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_24x8_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 7) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_24x8_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_24x8_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_24x8_MAIN_BEGIN(_Group_) \
  w -= 8; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_24x8_MAIN_CONTINUE(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_24x8_MAIN_END(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 8; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 32x1 - 32-bits per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_32x1_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_32x1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_32x1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_32x1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  }; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 32x4 - 32-bits per pixel, 4 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_32x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_32x4_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_32x4_SMALL_BEGIN_ALT(_Group_, _PrepareCode_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  _PrepareCode_ \
  for (;;) {

#define FOG_BLIT_LOOP_32x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_32x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \

#define FOG_BLIT_LOOP_32x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_32x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_32x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 32x4_VS_16 - 32-bits per pixel vs 16-bits per pixel]
// ============================================================================

#define FOG_BLIT_LOOP_32x4_VS_16_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_32x4_VS_16_SMALL_BEGIN(_Group_) \
  if (((size_t)src & 3) == 0 && w >= 4) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_32x4_VS_16_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (w < 4) continue; \
    break;

#define FOG_BLIT_LOOP_32x4_VS_16_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (w < 4) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \

#define FOG_BLIT_LOOP_32x4_VS_16_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_32x4_VS_16_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_32x4_VS_16_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 32x4_VS_24 - 32-bits per pixel vs 24-bits per pixel]
// ============================================================================

#define FOG_BLIT_LOOP_32v24x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_32v24x4_SMALL_BEGIN(_Group_) \
  if (((size_t)src & 3) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_32v24x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)src & 3) != 0) continue; \
    break;

#define FOG_BLIT_LOOP_32v24x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)src & 3) != 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \

#define FOG_BLIT_LOOP_32v24x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_32v24x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_32v24x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 32x16 - 32-bits per pixel, 16 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_32x16_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_32x16_ALIGN_BEGIN(_Group_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
  do {

#define FOG_BLIT_LOOP_32x16_ALIGN_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  } while (((size_t)dst & 15) != 0); \
  \
_##_Group_##_SmallSkip: \
  ;

#define FOG_BLIT_LOOP_32x16_MAIN_BEGIN(_Group_) \
  w -= 16; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  do {

#define FOG_BLIT_LOOP_32x16_MAIN_END(_Group_) \
  } while ((w -= 16) >= 0); \
  \
_##_Group_##_MainSkip: \
  w += 16;

#define FOG_BLIT_LOOP_32x16_TAIL_4(_Group_, _ProcessCode_) \
  switch (w >> 2) \
  { \
    case 3: _ProcessCode_ \
    case 2: _ProcessCode_ \
    case 1: _ProcessCode_ \
  }

#define FOG_BLIT_LOOP_32x16_TAIL_1(_Group_, _ProcessCode_) \
  switch (w & 3) \
  { \
    case 3: _ProcessCode_ \
    case 2: _ProcessCode_ \
    case 1: _ProcessCode_ \
  } \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 48x1 - 48-bits per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_48x1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_48x1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_48x1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  }; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 48x4 - 48-bits per pixel, 4 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_48x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_48x4_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 7) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_48x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) == 0) continue; \
    break;

#define FOG_BLIT_LOOP_48x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 7) == 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \

#define FOG_BLIT_LOOP_48x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
_##_Group_##_MainLoop: \
  for (;;) {

#define FOG_BLIT_LOOP_48x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_48x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 48x8 - 48-bits per pixel, 8 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_48x8_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_48x8_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_48x8_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) == 0) continue; \
    break;

#define FOG_BLIT_LOOP_48x8_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (((size_t)dst & 15) == 0) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \

#define FOG_BLIT_LOOP_48x8_MAIN_BEGIN(_Group_) \
  w -= 8; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
_##_Group_##_MainLoop: \
  for (;;) {

#define FOG_BLIT_LOOP_48x8_MAIN_CONTINUE(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_48x8_MAIN_END(_Group_) \
    if ((w -= 8) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 8; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 64x1 - 64-bits per pixel, 1 pixel in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_64x1_BEGIN(_Group_) \
  for (;;) {

#define FOG_BLIT_LOOP_64x1_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    continue;

#define FOG_BLIT_LOOP_64x1_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  }; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 64x2 - 64-bits per pixel, 2 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_64x2_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_64x2_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 15) != 0) \
  { \
_##_Group_##_SmallBegin: \

#define FOG_BLIT_LOOP_64x2_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
  } \

#define FOG_BLIT_LOOP_64x2_MAIN_BEGIN(_Group_) \
  w -= 2; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
_##_Group_##_MainLoop: \
  for (;;) {

#define FOG_BLIT_LOOP_64x2_MAIN_CONTINUE(_Group_) \
    if ((w -= 2) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_64x2_MAIN_END(_Group_) \
    if ((w -= 2) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 2; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [FOG_BLIT_LOOP - 64x4 - 64-bits per pixel, 4 pixels in a main loop]
// ============================================================================

#define FOG_BLIT_LOOP_64x4_INIT() \
  FOG_ASSUME(w > 0);

#define FOG_BLIT_LOOP_64x4_SMALL_BEGIN(_Group_) \
  if (((size_t)dst & 15) == 0) goto _##_Group_##_SmallSkip; \
  \
_##_Group_##_SmallBegin: \
  for (;;) {

#define FOG_BLIT_LOOP_64x4_SMALL_CONTINUE(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (w < 4) continue; \
    break;

#define FOG_BLIT_LOOP_64x4_SMALL_END(_Group_) \
    if (--w == 0) goto _##_Group_##_End; \
    if (w < 4) continue; \
    break; \
  } \
  \
_##_Group_##_SmallSkip: \

#define FOG_BLIT_LOOP_64x4_MAIN_BEGIN(_Group_) \
  w -= 4; \
  if (w < 0) goto _##_Group_##_MainSkip; \
  \
  for (;;) {

#define FOG_BLIT_LOOP_64x4_MAIN_CONTINUE(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break;

#define FOG_BLIT_LOOP_64x4_MAIN_END(_Group_) \
    if ((w -= 4) >= 0) continue; \
    break; \
  } \
  \
_##_Group_##_MainSkip: \
  w += 4; \
  if (w != 0) goto _##_Group_##_SmallBegin; \
  \
_##_Group_##_End: \
  ;

// ============================================================================
// [Fog::RasterOps_C - P_FETCH]
// ============================================================================

// Example of fetch function:
//
// static void FOG_FASTCALL fetch(
//   RasterPattern* ctx, RasterSpan* span, uint8_t* buffer, int y, uint32_t mode)
// {
//   // Initialize fetch function (some variables needed for span loop).
//   P_FETCH_SPAN8_INIT()
//
//   // Here place code that you need to initialize the fetcher. You can access
//   // these variables from here:
//   // 'x' - X position in target raster buffer
//   // 'y' - Y position in target raster buffer (you need inverted matrix to
//   //       X/Y to position in our pattern).
//   // 'w' - Width (number of pixels to fetch).
//   // 'dst' - Buffer
//   //
//   // This place should be used only for initialization, transforming X/Y
//   // to destination (texture) buffer, etc...
//
//   // Begin of span-loop.
//   P_FETCH_SPAN8_BEGIN()
//
//     // Here you must decide if you pass your own buffer to span (only
//     // possible if mode is @c RASTER_FETCH_REFERENCE). To set current
//     // buffer, use:
//
//     P_FETCH_SPAN8_SET_CURRENT()
//
//     // To set custom buffer (owned by pattern), use:
//
//     P_FETCH_SPAN8_SET_CUSTOM(yourCustomBuffer)
//
//     // If you used custom buffer you finished with this span, otherwise use
//     // 'dst' pointer (and increment it after each fetcher pixel). For
//     // example:
//
//     do {
//       ((uint32_t*)dst)[0] = 0x00000000;
//     } while (--w);
//
//     // Here is place to tell fetcher how is the preferred way of handling
//     // "holes". Hole is space between two spans. If spans are neighbours,
//     // then hole is zero pixels width (this is common). But hole can be
//     // also one or more pixels width (this happens if this span->x1 is less
//     // than next span->x1).
//     //
//     // If you are not interested about holes, use simply:
//
//     P_FETCH_SPAN8_NEXT()
//
//     // Otherwise use:
//
//     P_FETCH_SPAN8_HOLE(
//     {
//       // Your code, you can use goto to skip to specific part in loop.
//       // If you are using 'x' then you must update it.
//       //
//       // Use 'hole' variable that contains number of pixels to skip.
//       x += hole;
//     })
//
//   // End of span-loop.
//   P_FETCH_SPAN8_END()
// }
//

#define P_FETCH_SPAN8_INIT() \
  uint8_t* dst = buffer; \
  \
  int x = (int)span->getX0(); \
  int w = (int)((uint)span->getX1() - (uint)x); \
  FOG_ASSERT(w > 0);

#define P_FETCH_SPAN8_BEGIN() \
  for (;;) \
  {

#define P_FETCH_SPAN8_SET_CURRENT() \
    reinterpret_cast<RasterSpan8*>(span)->setData(dst);

#define P_FETCH_SPAN8_SET_CURRENT_AND_MERGE_NEIGHBORS(_Bpp_) \
    reinterpret_cast<RasterSpan8*>(span)->setData(dst); \
    \
    { \
      RasterSpan* next = span->getNext(); \
      if (next && next->getX0() == span->getX1()) \
      { \
        do { \
          reinterpret_cast<RasterSpan8*>(next)->setData(dst + (uint)w * _Bpp_); \
          span = next; \
          w = (int)((uint)next->getX1() - (uint)x); \
          next = span->getNext(); \
        } while (next && next->getX0() == span->getX1()); \
      } \
    }

#define P_FETCH_SPAN8_SET_CUSTOM(_Dst_) \
    reinterpret_cast<RasterSpan8*>(span)->setData((uint8_t*)(_Dst_));

#define P_FETCH_SPAN8_NEXT() \
    if ((span = span->getNext()) == NULL) break; \
    \
    x = (int)span->getX0(); \
    w = (int)((uint)span->getX1() - (uint)x); \
    FOG_ASSERT(w > 0);

#define P_FETCH_SPAN8_HOLE(_Code_) \
    { \
      int endX = (int)span->getX1(); \
      if ((span = span->getNext()) == NULL) break; \
      \
      int startX = (int)span->getX0(); \
      FOG_ASSERT(endX <= startX); \
      \
      w = (int)((uint)span->getX1() - (uint)startX); \
      FOG_ASSERT(w > 0); \
      \
      if (endX != startX) \
      { \
        int hole = (startX - endX); \
        _Code_ \
      } \
      \
    }

#define P_FETCH_SPAN8_END() \
  }

// ============================================================================
// [Fog::RasterOps_C - P_INTERPOLATE_C]
// ============================================================================

#if FOG_ARCH_BITS >= 64

#define P_INTERPOLATE_C_32_2(_Dst_, _Src0_, _Weight0_, _Src1_, _Weight1_) \
{ \
  __p64 _PixT0; \
  __p64 _PixT1; \
  \
  Acc::p64UnpackPBWFromPBB2031(_PixT0, (_Src0_)); \
  Acc::p64UnpackPBWFromPBB2031(_PixT1, (_Src1_)); \
  \
  Acc::p64Mul(_PixT0, _PixT0, _Weight0_); \
  Acc::p64Mul(_PixT1, _PixT1, _Weight1_); \
  Acc::p64Add(_PixT0, _PixT0, _PixT1); \
  Acc::p64And(_PixT0, _PixT0, Acc::p64FromU64(FOG_UINT64_C(0xFF00FF00FF00FF00))); \
  \
  Acc::p64Pack2031_RShift8(_Dst_, _PixT0); \
}

#define P_INTERPOLATE_C_32_2_WITH_ZERO(_Dst_, _Src0_, _Weight0_) \
{ \
  __p64 _PixT0; \
  \
  Acc::u64_1x4bUnpack0213(_PixT0, (_Src0_)); \
  Acc::p64Mul(_PixT0, _PixT0, _Weight0_); \
  Acc::p64And(_PixT0, _PixT0, Acc::p64FromU64(FOG_UINT64_C(0xFF00FF00FF00FF00))); \
  \
  Acc::p64Pack2031_RShift8(_Dst_, _PixT0); \
}

#define P_INTERPOLATE_C_32_4(_Dst_, _Src0_, _Weight0_, _Src1_, _Weight1_, _Src2_, _Weight2_, _Src3_, _Weight3_) \
{ \
  __p64 _PixT0; \
  __p64 _PixT1; \
  __p64 _PixT2; \
  \
  Acc::p64UnpackPBWFromPBB2031(_PixT0, (_Src0_)); \
  Acc::p64UnpackPBWFromPBB2031(_PixT1, (_Src1_)); \
  \
  Acc::p64Mul(_PixT0, _PixT0, Acc::p64FromU64(_Weight0_)); \
  Acc::p64Mul(_PixT1, _PixT1, Acc::p64FromU64(_Weight1_)); \
  Acc::p64Add(_PixT0, _PixT0, _PixT1); \
  \
  Acc::p64UnpackPBWFromPBB2031(_PixT1, (_Src2_)); \
  Acc::p64UnpackPBWFromPBB2031(_PixT2, (_Src3_)); \
  \
  Acc::p64Mul(_PixT1, _PixT1, Acc::p64FromU64(_Weight2_)); \
  Acc::p64Mul(_PixT2, _PixT2, Acc::p64FromU64(_Weight3_)); \
  Acc::p64Add(_PixT0, _PixT0, _PixT1); \
  Acc::p64Add(_PixT0, _PixT0, _PixT2); \
  \
  Acc::p64And(_PixT0, _PixT0, Acc::p64FromU64(FOG_UINT64_C(0xFF00FF00FF00FF00))); \
  \
  Acc::p64Pack2031_RShift8(_Dst_, _PixT0); \
}

#else

#define P_INTERPOLATE_C_32_2(_Dst_, _Src0_, _Weight0_, _Src1_, _Weight1_) \
{ \
  uint32_t _PixT0_0, _PixT0_1; \
  uint32_t _PixT1_0, _PixT1_1; \
  uint32_t _Weight; \
  \
  Acc::p32UnpackPBWFromPBB_2031(_PixT0_0, _PixT0_1, (_Src0_)); \
  Acc::p32UnpackPBWFromPBB_2031(_PixT1_0, _PixT1_1, (_Src1_)); \
  \
  _Weight = _Weight0_; \
  _PixT0_0 *= _Weight; \
  _PixT0_1 *= _Weight; \
  \
  _Weight = _Weight1_; \
  _PixT1_0 *= _Weight; \
  _PixT1_1 *= _Weight; \
  \
  _PixT0_0 += _PixT1_0; \
  _PixT0_1 += _PixT1_1; \
  \
  _PixT0_0 = (_PixT0_0 >> 8) & 0x00FF00FF; \
  _PixT0_1 = (_PixT0_1     ) & 0xFF00FF00; \
  \
  _Dst_ = _FOG_ACC_COMBINE_2(_PixT0_0, _PixT0_1); \
}

#define P_INTERPOLATE_C_32_2_WITH_ZERO(_Dst_, _Src0_, _Weight0_) \
{ \
  uint32_t _PixT0_0, _PixT0_1; \
  uint32_t _Weight; \
  \
  Acc::p32UnpackPBWFromPBB_2031(_PixT0_0, _PixT0_1, (_Src0_)); \
  \
  _Weight = _Weight0_; \
  _PixT0_0 *= _Weight; \
  _PixT0_1 *= _Weight; \
  \
  _PixT0_0 = (_PixT0_0 >> 8) & 0x00FF00FF; \
  _PixT0_1 = (_PixT0_1     ) & 0xFF00FF00; \
  \
  _Dst_ = _FOG_ACC_COMBINE_2(_PixT0_0, _PixT0_1); \
}

#define P_INTERPOLATE_C_32_4(_Dst_, _Src0_, _Weight0_, _Src1_, _Weight1_, _Src2_, _Weight2_, _Src3_, _Weight3_) \
{ \
  uint32_t _PixT0_0, _PixT0_1; \
  uint32_t _PixT1_0, _PixT1_1; \
  uint32_t _Weight; \
  \
  Acc::p32UnpackPBWFromPBB_2031(_PixT0_0, _PixT0_1, (_Src0_)); \
  Acc::p32UnpackPBWFromPBB_2031(_PixT1_0, _PixT1_1, (_Src1_)); \
  \
  _Weight = (_Weight0_); \
  _PixT0_0 *= _Weight; \
  _PixT0_1 *= _Weight; \
  \
  _Weight = (_Weight1_); \
  _PixT1_0 *= _Weight; \
  _PixT1_1 *= _Weight; \
  \
  _PixT0_0 += _PixT1_0; \
  _PixT0_1 += _PixT1_1; \
  \
  Acc::p32UnpackPBWFromPBB_2031(_PixT1_0, _PixT1_1, (_Src2_)); \
  \
  _Weight = (_Weight2_); \
  _PixT1_0 *= _Weight; \
  _PixT1_1 *= _Weight; \
  \
  _PixT0_0 += _PixT1_0; \
  _PixT0_1 += _PixT1_1; \
  \
  Acc::p32UnpackPBWFromPBB_2031(_PixT1_0, _PixT1_1, (_Src3_)); \
  \
  _Weight = (_Weight3_); \
  _PixT1_0 *= _Weight; \
  _PixT1_1 *= _Weight; \
  \
  _PixT0_0 += _PixT1_0; \
  _PixT0_1 += _PixT1_1; \
  \
  _PixT0_0 = (_PixT0_0 >> 8) & 0x00FF00FF; \
  _PixT0_1 = (_PixT0_1     ) & 0xFF00FF00; \
  \
  _Dst_ = _FOG_ACC_COMBINE_2(_PixT0_0, _PixT0_1); \
}

#endif // FOG_ARCH_BITS

// [Guard]
#endif // _FOG_G2D_PAINTING_RASTEROPS_C_BASEDEFS_P_H
