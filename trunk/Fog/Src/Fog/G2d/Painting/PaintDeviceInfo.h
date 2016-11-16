// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_PAINTDEVICEINFO_H
#define _FOG_G2D_PAINTING_PAINTDEVICEINFO_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Threading/Atomic.h>
#include <Fog/G2d/Geometry/Point.h>
#include <Fog/G2d/Tools/Dpi.h>

namespace Fog {

//! @addtogroup Fog_G2d_Painting
//! @{

// ============================================================================
// [Fog::PaintDeviceInfoData]
// ============================================================================

struct FOG_NO_EXPORT PaintDeviceInfoData
{
  //! @brief Reference count.
  mutable Atomic<size_t> reference;

  //! @brief Paint device ID (see @c PAINT_DEVICE).
  uint32_t paintDevice;

  //! @brief Font-kerning to substitute when creating physical-font that has
  //! kerning type set to @c FONT_KERNING_DETECT.
  uint8_t fontKerning;

  //! @brief Font-hinting to substitute when creating physical-font that has
  //! hinting type set to @c FONT_HINTING_DETECT.
  uint8_t fontHinting;

  //! @brief Quality of rendered fonts, see @c FONT_QUALITY.
  uint8_t fontQuality;

  //! @brief LCD-Order for screen based devices or @c LCD_ORDER_NONE.
  uint8_t lcdOrder;

  //! @brief DPI used by the device.
  //!
  //! @note There is only one DPI set per device info. The difference between
  //! the horizontal and vertical DPI is handled by using differen horizontal
  //! and vertical scaling. This means that font-info is matched properly and
  //! device can use any DPI combination without affecting the font handling
  //! and transformations.
  //!
  //! At this time the DPI is used only to translate the coordinates into the
  //! device pixels.
  Dpi dpiInfo;

  //! @brief Scale (x/y).
  PointF scale;

  //! @brief Scale aspect ratio.
  PointF aspectRatio;
};

// ============================================================================
// [Fog::PaintDeviceInfo]
// ============================================================================

struct FOG_API PaintDeviceInfo
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  PaintDeviceInfo();
  PaintDeviceInfo(const PaintDeviceInfo& other);
  ~PaintDeviceInfo();

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE float getDpi() const { return _d->dpiInfo.getDpi(); }
  FOG_INLINE const Dpi& getDpiInfo() const { return _d->dpiInfo; }

  // --------------------------------------------------------------------------
  // [Reset]
  // --------------------------------------------------------------------------

  void reset();

  // --------------------------------------------------------------------------
  // [Create]
  // --------------------------------------------------------------------------

  err_t create(uint32_t paintDevice,
    uint32_t fontKerning,
    uint32_t fontHinting,
    uint32_t fontQuality,
    uint32_t lcdOrder,
    float dpi,
    const PointF& scale);

  // --------------------------------------------------------------------------
  // [Operator Overload]
  // --------------------------------------------------------------------------

  PaintDeviceInfo& operator=(const PaintDeviceInfo& other);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  _FOG_CLASS_D(PaintDeviceInfoData)
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_G2D_PAINTING_PAINTDEVICEINFO_H
