// [Fog-Gui Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Graphics/Painter.h>
#include <Fog/Gui/ScrollBar.h>

FOG_IMPLEMENT_OBJECT(Fog::ScrollBar)

namespace Fog {

// ============================================================================
// [Fog::ScrollBar]
// ============================================================================

ScrollBar::ScrollBar(uint32_t createFlags) :
  Widget(createFlags)
{
}

ScrollBar::~ScrollBar()
{
}

} // Fog namespace
