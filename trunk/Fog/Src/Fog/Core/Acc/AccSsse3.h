// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_ACC_ACCSSSE3_H
#define _FOG_CORE_ACC_ACCSSSE3_H

// [Dependencies]
#include <Fog/Core/C++/Base.h>
#include <Fog/Core/C++/IntrinSsse3.h>

#include <Fog/Core/Acc/AccSse.h>
#include <Fog/Core/Acc/AccSse2.h>
#include <Fog/Core/Acc/AccSse3.h>

namespace Fog {
namespace Acc {

//! @addtogroup Fog_Core_Acc_Ssse3
//! @{

// ============================================================================
// [Fog::Acc - SSSE3 - Alignr]
// ============================================================================

template<int BYTES>
static FOG_INLINE void m128iAlignrPI8(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_alignr_epi8(x0, y0, BYTES);
}

// ============================================================================
// [Fog::Acc - SSSE3 - Shuffle]
// ============================================================================

static FOG_INLINE void m128iShufflePI8(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_shuffle_epi8(x0, y0);
}

template<
  uint M0, uint M1, uint M2 , uint M3 , uint M4 , uint M5 , uint M6 , uint M7 ,
  uint M8, uint M9, uint M10, uint M11, uint M12, uint M13, uint M14, uint M15>
static FOG_INLINE void m128iShufflePI8i(__m128i& dst0, const __m128i& x0)
{
  FOG_XMM_DECLARE_CONST_PI8_VAR(Tmp, M0, M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11, M12, M13, M14, M15);
  dst0 = _mm_shuffle_epi8(x0, FOG_XMM_GET_CONST_PI(Tmp));
}

// ============================================================================
// [Fog::Acc - SSSE3 - HAdd]
// ============================================================================

static FOG_INLINE void m128iHAddPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_hadd_epi16(x0, y0);
}

static FOG_INLINE void m128iHAddPI32(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_hadd_epi32(x0, y0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - HAdd-Saturate]
// ============================================================================

static FOG_INLINE void m128iHAddsPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_hadds_epi16(x0, y0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - HSub]
// ============================================================================

static FOG_INLINE void m128iHSubPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_hsub_epi16(x0, y0);
}

static FOG_INLINE void m128iHSubPI32(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_hsub_epi32(x0, y0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - HSub-Saturate]
// ============================================================================

static FOG_INLINE void m128iHSubsPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_hsubs_epi16(x0, y0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - Mul-HRS]
// ============================================================================

//! @verbatim
//! dst0.w[0..7] := (int16_t)( ((x0[0..7] * y0[0..7]) + 0x4000) >> 15 )
//! @endverbatim
static FOG_INLINE void m128iMulhrsPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_mulhrs_epi16(x0, y0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - MAdd-Saturate]
// ============================================================================

static FOG_INLINE void m128iMAddubsPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_maddubs_epi16(x0, y0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - Abs]
// ============================================================================

static FOG_INLINE void m128iAbsPI8(
  __m128i& dst0, const __m128i& x0)
{
  dst0 = _mm_abs_epi8(x0);
}

static FOG_INLINE void m128iAbsPI16(
  __m128i& dst0, const __m128i& x0)
{
  dst0 = _mm_abs_epi16(x0);
}

static FOG_INLINE void m128iAbsPI32(
  __m128i& dst0, const __m128i& x0)
{
  dst0 = _mm_abs_epi32(x0);
}

// ============================================================================
// [Fog::Acc - SSSE3 - Sign]
// ============================================================================

static FOG_INLINE void m128iSignPI8(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_sign_epi8(x0, y0);
}

static FOG_INLINE void m128iSignPI16(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_sign_epi16(x0, y0);
}

static FOG_INLINE void m128iSignPI32(
  __m128i& dst0, const __m128i& x0, const __m128i& y0)
{
  dst0 = _mm_sign_epi32(x0, y0);
}

//! @}

} // Acc namespace
} // Fog namespace

// MMX+SSE2 extensions require both, Face_MMX.h, and Face_SSE2.h headers.
#if defined(_FOG_CORE_ACC_ACCMMX_H)
# include <Fog/Core/Acc/AccMmxSsse3.h>
#endif // _FOG_CORE_ACC_ACCMMX_H

// [Guard]
#endif // _FOG_CORE_ACC_ACCSSSE3_H
