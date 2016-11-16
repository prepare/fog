// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_SOURCE_COLORBASE_H
#define _FOG_G2D_SOURCE_COLORBASE_H

// [Dependencies]
#include <Fog/Core/Acc/AccC.h>
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/Core/Math/Fuzzy.h>

namespace Fog {

//! @addtogroup Fog_G2d_Source
//! @{

// ============================================================================
// [Fog::ArgbBase32]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
//! @brief ARGB32 color data (8-bit integer per component).
struct FOG_NO_EXPORT ArgbBase32
{
  union
  {
    struct
    {
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t a;
#else
      uint8_t a;
      uint8_t r;
      uint8_t g;
      uint8_t b;
#endif // FOG_BYTE_ORDER
    };
    uint32_t u32;
  };
};
#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::ArgbBase64]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
//! @brief ARGB64 color data (16-bit integer per component).
struct FOG_NO_EXPORT ArgbBase64
{
  union
  {
    struct
    {
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
      uint16_t b;
      uint16_t g;
      uint16_t r;
      uint16_t a;
#else
      uint16_t a;
      uint16_t r;
      uint16_t g;
      uint16_t b;
#endif // FOG_BYTE_ORDER
    };

    __p64 p64;
    uint64_t u64;
    uint32_t u32[2];
  };
};
#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::ArgbF]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
//! @brief ARGBF color data (32-bit float per component).
struct FOG_NO_EXPORT ArgbBaseF
{
  union
  {
    struct
    {
      float a;
      float r;
      float g;
      float b;
    };

    float data[4];
  };
};
#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::AhsvBaseF]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
//! @brief AHSVF color data.
struct FOG_NO_EXPORT AhsvBaseF
{
  union
  {
    struct
    {
      float a;
      float h;
      float s;
      float v;
    };
    float data[4];
  };
};
#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::AhsvBaseF]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
//! @brief AHSLF color data.
struct FOG_NO_EXPORT AhslBaseF
{
  union
  {
    struct
    {
      float a;
      float h;
      float s;
      float l;
    };
    float data[4];
  };
};
#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::AcmykBaseF]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
//! @brief ACMYKF color data.
struct FOG_NO_EXPORT AcmykBaseF
{
  union
  {
    struct
    {
      float a;
      float c;
      float m;
      float y;
      float k;
    };
    float data[5];
  };
};
#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::ColorBase]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>
struct FOG_NO_EXPORT ColorBase
{
  //! @brief The ARGB, AHSV, AHSL, or ACMYK components array.
  float _data[5];

  //! @brief The color model.
  uint32_t _model : 16;
  //! @brief The color hints.
  uint32_t _hints : 16;

  //! @brief The packed ARGB32 color (for fast use/access).
  ArgbBase32 _argb32;
};
#include <Fog/Core/C++/PackRestore.h>

//! @}

} // Fog namespace

// ============================================================================
// [Fog::Fuzzy<>]
// ============================================================================

FOG_FUZZY_DECLARE_F_VEC(Fog::ArgbBaseF, 4)
FOG_FUZZY_DECLARE_F_VEC(Fog::AhsvBaseF, 4)
FOG_FUZZY_DECLARE_F_VEC(Fog::AhslBaseF, 4)
FOG_FUZZY_DECLARE_F_VEC(Fog::AcmykBaseF, 5)

// [Guard]
#endif // _FOG_G2D_SOURCE_COLORBASE_H
