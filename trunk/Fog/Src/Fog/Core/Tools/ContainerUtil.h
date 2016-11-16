// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_TOOLS_CONTAINERUTIL_H
#define _FOG_CORE_TOOLS_CONTAINERUTIL_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Tools/Range.h>

namespace Fog {
namespace ContainerUtil {

//! @addtogroup Fog_Core_Tools
//! @{

// ============================================================================
// [Fog::ContainerUtil]
// ============================================================================

FOG_API size_t getGrowCapacity(size_t headerSize, size_t szItemT, size_t before, size_t after);

//! @}

} // ContainerUtil namespace
} // Fog namespace

// [Guard]
#endif // _FOG_CORE_TOOLS_CONTAINERUTIL_H
