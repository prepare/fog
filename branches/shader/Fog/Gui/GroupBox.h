// [Fog-Gui Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_GUI_GROUPBOX_H
#define _FOG_GUI_GROUPBOX_H

// [Dependencies]
#include <Fog/Core/String.h>
#include <Fog/Gui/Widget.h>

namespace Fog {

//! @addtogroup Fog_Gui_Widget
//! @{

// ============================================================================
// [Fog::GroupBox]
// ============================================================================

//! @brief Composite widget.
struct FOG_API GroupBox : public Widget
{
  FOG_DECLARE_OBJECT(GroupBox, Widget)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  GroupBox(uint32_t createFlags = 0);
  virtual ~GroupBox();

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE const String& getLabel() const { return _label; }
  void setLabel(const String& label);

  // --------------------------------------------------------------------------
  // [Client Geometry]
  // --------------------------------------------------------------------------

  virtual void calcWidgetSize(IntSize& size) const;
  virtual void calcClientGeometry(IntRect& geometry) const;

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  String _label;
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_GUI_GROUPBOX_H
