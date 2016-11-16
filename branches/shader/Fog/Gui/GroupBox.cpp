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
#include <Fog/Gui/GroupBox.h>

FOG_IMPLEMENT_OBJECT(Fog::GroupBox)

namespace Fog {

// ============================================================================
// [Fog::GroupBox]
// ============================================================================

GroupBox::GroupBox(uint32_t createFlags) :
  Widget(createFlags)
{
}

GroupBox::~GroupBox()
{
}

void GroupBox::setLabel(const String& label)
{
  _label = label;
  update(WIDGET_UPDATE_NCPAINT);
}

void GroupBox::calcWidgetSize(IntSize& size) const
{
}

void GroupBox::calcClientGeometry(IntRect& geometry) const
{
}

} // Fog namespace
