// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_BYTEARRAYFILTER_H
#define _FOG_CORE_BYTEARRAYFILTER_H

// [Dependencies]
#include <Fog/Build/Build.h>
#include <Fog/Core/Assert.h>
#include <Fog/Core/Basics.h>
#include <Fog/Core/ByteArray.h>

//! @addtogroup Fog_Core
//! @{

namespace Fog {

// ============================================================================
// [Fog::ByteArrayFilter]
// ============================================================================

struct FOG_API ByteArrayFilter
{
  // [Construction / Destruction]

  ByteArrayFilter();
  virtual ~ByteArrayFilter();

  // [Public]

  virtual Range indexOf(const char* str, sysuint_t length, uint cs = CASE_SENSITIVE, const Range& range = Range(0)) const;
  virtual Range lastIndexOf(const char* str, sysuint_t length, uint cs = CASE_SENSITIVE, const Range& range = Range(0)) const;

  // [Filter Implementation]

  virtual sysuint_t getLength() const;
  virtual Range match(const char* str, sysuint_t length, uint cs, const Range& range) const = 0;

private:
  FOG_DISABLE_COPY(ByteArrayFilter)
};

} // Fog namespace

//! @}

// [Guard]
#endif // _FOG_CORE_BYTEARRAYFILTER_H
