// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_GLOBAL_UNINITIALIZED_H
#define _FOG_CORE_GLOBAL_UNINITIALIZED_H

// [Dependencies]
#include <Fog/Core/C++/Base.h>

namespace Fog {

//! @addtogroup Fog_Core_Global
//! @{

// ============================================================================
// [Fog::UNINITIALIZED]
// ============================================================================

struct FOG_NO_EXPORT _Uninitialized {};
static const _Uninitialized UNINITIALIZED = {};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_CORE_GLOBAL_UNINITIALIZED_H
