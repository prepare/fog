// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_TEXT_OPENTYPE_OTKERN_H
#define _FOG_G2D_TEXT_OPENTYPE_OTKERN_H

// [Dependencies]
#include <Fog/Core/Memory/MemZoneAllocator.h>
#include <Fog/G2d/Text/OpenType/OTApi.h>
#include <Fog/G2d/Text/OpenType/OTTypes.h>

namespace Fog {

// [Byte-Pack]
#include <Fog/Core/C++/PackByte.h>

//! @addtogroup Fog_G2d_Text_OpenType
//! @{

// ============================================================================
// [Fog::OTKernHeader]
// ============================================================================

//! @brief TrueType/OpenType 'kern' - Kerning header.
struct FOG_NO_EXPORT OTKernHeader
{
};

// ============================================================================
// [Fog::OTKern]
// ============================================================================

//! @brief TrueType/OpenType 'kern' - Kerning table.
//!
//! Specification:
//!   - http://www.microsoft.com/typography/otspec/kern.htm
//!   - https://developer.apple.com/fonts/ttrefman/RM06/Chap6kern.html
struct FOG_NO_EXPORT OTKern : public OTTable
{
  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE const OTKernHeader* getHeader() const { return reinterpret_cast<OTKernHeader*>(_data); }
};

//! @}

// [Byte-Pack]
#include <Fog/Core/C++/PackRestore.h>

} // Fog namespace

// [Guard]
#endif // _FOG_G2D_TEXT_OPENTYPE_OTKERN_H
