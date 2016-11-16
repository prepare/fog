// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_GLOBAL_ENUMG2D_H
#define _FOG_CORE_GLOBAL_ENUMG2D_H

// [Dependencies]
#include <Fog/Core/Global/EnumCore.h>

namespace Fog {

//! @addtogroup Fog_Core_Global
//! @{

// ============================================================================
// [Fog::ALPHA_DISTRIBUTION]
// ============================================================================

//! @brief Distribution of alpha channel values in an image or in a color table.
enum ALPHA_DISTRIBUTION
{
  //! @brief Alpha values are 0.
  ALPHA_DISTRIBUTION_ZERO = 0,
  //! @brief Alpha values are 1.
  ALPHA_DISTRIBUTION_FULL = 1,
  //! @brief Alpha values are 0 (transparent) or 1 (opaque).
  ALPHA_DISTRIBUTION_ZERO_OR_FULL = 2,
  //! @brief Alpha values are variant.
  ALPHA_DISTRIBUTION_VARIANT = 3,

  //! @brief Count of alpha distribution types.
  ALPHA_DISTRIBUTION_COUNT = 4
};

// ============================================================================
// [Fog::AXIS]
// ============================================================================

//! @brief Axis.
enum AXIS
{
  //! @brief X axis.
  AXIS_X = 0x1,
  //! @brief Y axis.
  AXIS_Y = 0x2,
  //! @brief Z axis.
  AXIS_Z = 0x4
};

// ============================================================================
// [Fog::CLIP_OP]
// ============================================================================

//! @brief Clip operation.
enum CLIP_OP
{
  //! @brief Replace the current clipping area.
  CLIP_OP_REPLACE = 0,
  //! @brief Intersect the current clipping area.
  CLIP_OP_INTERSECT = 1,

  //! @brief Count of clip operators.
  CLIP_OP_COUNT = 2
};

// ============================================================================
// [Fog::CLIP_SHIFT]
// ============================================================================

enum CLIP_SHIFT
{
  CLIP_SHIFT_X0 = 2,
  CLIP_SHIFT_Y0 = 3,
  CLIP_SHIFT_X1 = 0,
  CLIP_SHIFT_Y1 = 1
};

// ============================================================================
// [Fog::CLIP_SIDE]
// ============================================================================

enum CLIP_SIDE
{
  CLIP_SIDE_NONE   = 0x00,

  CLIP_SIDE_X0     = 1 << CLIP_SHIFT_X0,
  CLIP_SIDE_Y0     = 1 << CLIP_SHIFT_Y0,
  CLIP_SIDE_X1     = 1 << CLIP_SHIFT_X1,
  CLIP_SIDE_Y1     = 1 << CLIP_SHIFT_Y1,

  CLIP_SIDE_X      = CLIP_SIDE_X0 | CLIP_SIDE_X1,
  CLIP_SIDE_Y      = CLIP_SIDE_Y0 | CLIP_SIDE_Y1,

  CLIP_SIDE_LEFT   = CLIP_SIDE_X0,
  CLIP_SIDE_TOP    = CLIP_SIDE_Y0,
  CLIP_SIDE_RIGHT  = CLIP_SIDE_X1,
  CLIP_SIDE_BOTTOM = CLIP_SIDE_Y1,

  CLIP_SIDE_LT     = CLIP_SIDE_LEFT | CLIP_SIDE_TOP,
  CLIP_SIDE_RT     = CLIP_SIDE_RIGHT | CLIP_SIDE_TOP,

  CLIP_SIDE_LB     = CLIP_SIDE_LEFT | CLIP_SIDE_BOTTOM,
  CLIP_SIDE_RB     = CLIP_SIDE_RIGHT | CLIP_SIDE_BOTTOM
};

// ============================================================================
// [Fog::COLOR_ADJUST_OP]
// ============================================================================

enum COLOR_ADJUST_OP
{
  COLOR_ADJUST_OP_NONE = 0,
  COLOR_ADJUST_OP_LIGHTEN = 1,
  COLOR_ADJUST_OP_DARKEN = 2,
  COLOR_ADJUST_OP_HUE = 3,

  COLOR_ADJUST_OP_COUNT = 4
};

// ============================================================================
// [Fog::COLOR_CHANNEL]
// ============================================================================

//! @brief Argb32 color channels.
enum COLOR_CHANNEL
{
  COLOR_CHANNEL_NONE  = 0x0,

  COLOR_CHANNEL_ALPHA = 0x1,
  COLOR_CHANNEL_RED   = 0x2,
  COLOR_CHANNEL_GREEN = 0x4,
  COLOR_CHANNEL_BLUE  = 0x8,

  COLOR_CHANNEL_RGB   = COLOR_CHANNEL_RED | COLOR_CHANNEL_GREEN | COLOR_CHANNEL_BLUE,
  COLOR_CHANNEL_ARGB  = COLOR_CHANNEL_RGB | COLOR_CHANNEL_ALPHA,

  COLOR_CHANNEL_COUNT = COLOR_CHANNEL_ARGB + 1
};

// ============================================================================
// [Fog::COLOR_INDEX]
// ============================================================================

enum COLOR_INDEX
{
  COLOR_INDEX_ALPHA = 0,
  COLOR_INDEX_RED   = 1,
  COLOR_INDEX_GREEN = 2,
  COLOR_INDEX_BLUE  = 3,

  COLOR_INDEX_COUNT = 4
};

// ============================================================================
// [Fog::COLOR_MODEL]
// ============================================================================

//! @brief Color model used by @c Color class.
enum COLOR_MODEL
{
  // --------------------------------------------------------------------------
  // [Public]
  // --------------------------------------------------------------------------

  COLOR_MODEL_NONE = 0,
  COLOR_MODEL_ARGB = 1,
  COLOR_MODEL_AHSV = 2,
  COLOR_MODEL_AHSL = 3,
  COLOR_MODEL_ACMYK = 4,

  COLOR_MODEL_COUNT = 5,

  // --------------------------------------------------------------------------
  // [Private]
  // --------------------------------------------------------------------------

  // Used internally by the Fog::Color conversion classes, reserved one color
  // model for the future binary compatibility.

  //! @internal
  _COLOR_MODEL_ARGB32 = 6,
  //! @internal
  _COLOR_MODEL_ARGB64 = 7,
  //! @internal
  _COLOR_MODEL_COUNT = 8
};

// ============================================================================
// [Fog::COLOR_NAME]
// ============================================================================

//! @brief Contains flags which might be used by @c Color.parse().
enum COLOR_NAME
{
  //! @brief Color in the CSS-RGB form "rgb(R, G, B)".
  COLOR_NAME_CSS_RGB = 0x00000001,
  //! @brief Color in the CSS-RGBA form "rgba(R, G, B, A)".
  COLOR_NAME_CSS_RGBA = 0x00000002,
  //! @brief Color in the CSS-HSL form "hsl(H, S, L)".
  COLOR_NAME_CSS_HSL = 0x00000004,
  //! @brief Color in the CSS-HSLA form "hsla(H, S, L, A)".
  COLOR_NAME_CSS_HSLA = 0x00000008,

  //! @brief Color in the CSS-HEX form "#RRGGBB" or "#RGB".
  COLOR_NAME_CSS_HEX = 0x00000010,

  //! @brief Color that matches the CSS color keywords.
  COLOR_NAME_CSS_KEYWORD = 0x00000020,
  //! @brief Color that matches the 'transparent' keyword.
  COLOR_NAME_CSS_TRANSPARENT = 0x00000040,

  //! @brief Strict mode.
  //!
  //! Strict mode does following:
  //!   - Error is returned if the given string starts with white-space
  //!   - Error is returned if the given string not ends with the color
  //!     keyword or data (continues).
  //!   - Error is returned for an non-css compliant color combination,
  //!     for example rgb(255, 100%, 100%) is invalid color string,
  //!     because percent notation is mixed with integer notation.
  COLOR_NAME_STRICT = 0x00001000,
  //! @brief Ignore case of the color string.
  COLOR_NAME_IGNORE_CASE = 0x00002000,

  //! @brief Use all rules to parse the color in CSS format.
  COLOR_NAME_CSS =
    COLOR_NAME_CSS_RGB         |
    COLOR_NAME_CSS_RGBA        |
    COLOR_NAME_CSS_HSL         |
    COLOR_NAME_CSS_HSLA        |
    COLOR_NAME_CSS_HEX         |
    COLOR_NAME_CSS_KEYWORD     |
    COLOR_NAME_CSS_TRANSPARENT ,

  //! @brief Use all rules to parse the color in ANY format.
  COLOR_NAME_ANY =
    COLOR_NAME_CSS             |
    COLOR_NAME_IGNORE_CASE
};

// ============================================================================
// [Fog::COLOR_MIX_ALPHA]
// ============================================================================

enum COLOR_MIX_ALPHA
{
  //! @brief Preserve the destination alpha and do not use the source alpha.
  COLOR_MIX_ALPHA_PRESERVE = 0,

  //! @brief Preserve the destination alpha and multiply the source alpha with
  //! the mask.
  COLOR_MIX_ALPHA_MASK = 1,

  //! @brief Treat the alpha as a RGB component, using the same mix function.
  COLOR_MIX_ALPHA_COMPONENT = 2
};

// ============================================================================
// [Fog::COLOR_MIX_OP]
// ============================================================================

enum COLOR_MIX_OP
{
  COLOR_MIX_OP_NONE = 0,
  COLOR_MIX_OP_BLEND = 1,
  COLOR_MIX_OP_PLUS = 2,
  COLOR_MIX_OP_MINUS = 3,
  COLOR_MIX_OP_MULTIPLY = 4,
  COLOR_MIX_OP_DIVIDE = 5,
  COLOR_MIX_OP_SCREEN = 6,
  COLOR_MIX_OP_OVERLAY = 7,
  COLOR_MIX_OP_DARKEN = 8,
  COLOR_MIX_OP_LIGHTEN = 9,
  COLOR_MIX_OP_DODGE = 10,
  COLOR_MIX_OP_BURN = 11,
  COLOR_MIX_OP_DIFFERENCE = 12,

  COLOR_MIX_OP_COUNT = 13
};

// ============================================================================
// [Fog::COMPOSITE_OP]
// ============================================================================

//! @brief The compositing operator.
enum COMPOSITE_OP
{
  //! @brief The source is copied to the destination.
  //!
  //! The source pixel is copied to destination pixel. If destination pixel
  //! format not supports alpha channel, the source alpha value is ignored.
  //! If there is alpha channel mask the composition is done using LERP
  //! operator.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SRC):
  //!   Dca' = Sca.Da + Sca.(1 - Da)
  //!        = Sca
  //!   Da'  = Sa.Da + Sa.(1 - Da)
  //!        = Sa
  //! @endverbatim
  COMPOSITE_SRC = 0,

  //! @brief The source is composited over the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SRC_OVER):
  //!   Dca' = Sca.Da + Sca.(1 - Da) + Dca.(1 - Sa)
  //!        = Sca + Dca.(1 - Sa)
  //!   Da'  = Sa.Da + Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Sa + Da.(1 - Sa)
  //!        = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_SRC_OVER = 1,

  //! @brief The part of the source lying inside of the destination replaces
  //! the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SRC_IN):
  //!   Dca' = Sca.Da
  //!   Da'  = Sa.Da
  //! @endverbatim
  COMPOSITE_SRC_IN = 2,

  //! @brief The part of the source lying outside of the destination replaces
  //! the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SRC_OUT):
  //!   Dca' = Sca.(1 - Da)
  //!   Da'  = Sa.(1 - Da)
  //! @endverbatim
  COMPOSITE_SRC_OUT = 3,

  //! @brief The part of the source lying inside of the destination is
  //! composited onto the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SRC_ATOP):
  //!   Dca' = Sca.Da + Dca.(1 - Sa)
  //!   Da'  = Sa.Da + Da.(1 - Sa)
  //!        = Sa.Da + Da - Da.Sa
  //!        = Da
  //! @endverbatim
  COMPOSITE_SRC_ATOP = 4,

  //! @brief The destination is left untouched.
  //!
  //! Destination pixels remains unchanged.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (NOP):
  //!   Dca' = Dca
  //!   Da'  = Da
  //! @endverbatim
  COMPOSITE_DST = 5,

  //! @brief The destination is composited over the source and the result
  //! replaces the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (DST_OVER):
  //!   Dca' = Dca.Sa + Sca.(1 - Da) + Dca.(1 - Sa)
  //!        = Dca + Sca.(1 - Da)
  //!   Da'  = Da.Sa + Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Da + Sa.(1 - Da)
  //!        = Da + Sa - Da.Sa
  //! @endverbatim
  COMPOSITE_DST_OVER = 6,

  //! @brief The part of the destination lying inside of the source replaces
  //! the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (DST_IN):
  //!   Dca' = Dca.Sa
  //!   Da'  = Da.Sa
  //! @endverbatim
  COMPOSITE_DST_IN = 7,

  //! @brief The part of the destination lying outside of the source replaces
  //! the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (DST_OUT):
  //!   Dca' = Dca.(1 - Sa)
  //!   Da'  = Da.(1 - Sa)
  //! @endverbatim
  COMPOSITE_DST_OUT = 8,

  //! @brief The part of the destination lying inside of the source is
  //! composited over the source and replaces the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (DST_ATOP):
  //!   Dca' = Dca.Sa + Sca.(1 - Da)
  //!   Da'  = Da.Sa + Sa.(1 - Da)
  //!        = Sa.(Da + 1 - Da)
  //!        = Sa
  //! @endverbatim
  COMPOSITE_DST_ATOP = 9,

  //! @brief The part of the source that lies outside of the destination is
  //! combined with the part of the destination that lies outside of the source.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (XOR):
  //!   Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Sa + Da - 2.Sa.Da
  //! @endverbatim
  COMPOSITE_XOR = 10,

  //! @brief Clear the destination not using the source.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (CLEAR):
  //!   Dca' = 0
  //!   Da'  = 0
  //! @endverbatim
  COMPOSITE_CLEAR = 11,

  //! @brief The source is added to the destination and replaces the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (PLUS):
  //!   Dca' = Sca.Da + Dca.Sa + Sca.(1 - Da) + Dca.(1 - Sa)
  //!        = Dca + Sca
  //!   Da'  = Sa.Da + Da.Sa + Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Da + Sa
  //! @endverbatim
  COMPOSITE_PLUS = 12,

  //! @brief The source is subtracted from the destination and replaces
  //! the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (MINUS):
  //!   Dca' = Dca - Sca
  //!   Da'  = 1 - (1 - Sa).(1 - Da)
  //!        = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_MINUS = 13,

  //! @brief The source is multiplied by the destination and replaces
  //! the destination.
  //!
  //! The resultant color is always at least as dark as either of the two
  //! constituent colors. Multiplying any color with black produces black.
  //! Multiplying any color with white leaves the original color unchanged.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (MULTIPLY):
  //!   Dca' = Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa.Da + Sa.(1 - Da) + Da.(1 - Sa)
  //! @endverbatim
  COMPOSITE_MULTIPLY = 14,

  //! @brief The source and destination are complemented and then multiplied
  //! and then replace the destination.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SCREEN):
  //!   Dca' = Sca + Dca.(1 - Sca)
  //!   Da'  = Sa + Da.(1 - Sa)
  //! @endverbatim
  COMPOSITE_SCREEN = 15,

  //! @brief Multiplies or screens the colors, dependent on the destination
  //! color.
  //!
  //! Source colors overlay the destination whilst preserving its highlights
  //! and shadows. The destination color is not replaced, but is mixed with
  //! the source color to reflect the lightness or darkness of the destination.
  //!
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (OVERLAY):
  //!
  //! @verbatim
  //! Tca = Sca.(1 - Da) + Dca.(1 - Sa)
  //! Ta  = Sa .(1 - Da) + Da .(1 - Sa)
  //!
  //! if (2.Dca < Da)
  //!   Dca' = 2.Sca.Dca + Tca
  //!   Da'  = Sa.Da     + Ta
  //!        = Sa + Da - Sa.Da
  //! else
  //!   Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Tca
  //!   Da'  = Sa.Da - 2.(Da - Da ).(Sa - Sa ) + Ta
  //!        = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_OVERLAY = 16,

  //! @brief Selects the darker of the destination and source colors.
  //!
  //! The destination is replaced with the source when the source is
  //! darker, otherwise it is left unchanged.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (DARKEN):
  //!   Dca' = min(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = min(Sa.Da, Da.Sa) + Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Sa.Da + Sa - Sa.Da + Da - Sa.Da
  //!        = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_DARKEN = 17,

  //! @brief Selects the lighter of the destination and source colors.
  //!
  //! The destination is replaced with the source when the source is
  //! lighter, otherwise it is left unchanged.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (LIGHTEN):
  //!   Dca' = max(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = max(Sa.Da, Da.Sa) + Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Sa.Da + Sa - Sa.Da + Da - Sa.Da
  //!        = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_LIGHTEN = 18,

  //! @brief Brightens the destination color to reflect the source color.
  //!
  //! Painting with black produces no change.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (COLOR_DODGE):
  //!
  //! if (Sca.Da + Dca.Sa >= Sa.Da)
  //!   Dca' = Sa.Da + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! else
  //!   Dca' = Dca.Sa / (1 - Sca / Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Dca' = [Dca.(Sa - Sca.(1 - Sa)) + Sca((Sa - Sca).(1 - Da))] / [Sa - Sca]
  //!   Da'  = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_COLOR_DODGE = 19,

  //! @brief Darkens the destination color to reflect the source color.
  //! Painting with white produces no change.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (COLOR_BURN):
  //!
  //! if (Sca.Da + Dca.Sa <= Sa.Da)
  //!   Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! else
  //!   Dca' = Sa.(Sca.Da + Dca.Sa - Sa.Da)/Sca + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_COLOR_BURN = 20,

  //! @brief Multiplies or screens the colors, dependent on the source color value.
  //!
  //! If the source color is lighter than 0.5, the destination is lightened as
  //! if it were screened. If the source color is darker than 0.5, the destination
  //! is darkened, as if it were multiplied. The degree of lightening or darkening
  //! is proportional to the difference between the source color and 0.5. If it is
  //! equal to 0.5 the destination is unchanged. Painting with pure black or white
  //! produces black or white.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (HARD_LIGHT):
  //!
  //! if (2.Sca <= Sa)
  //!   Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! else
  //!   Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_HARD_LIGHT = 21,

  //! @brief Darkens or lightens the colors, dependent on the source color value.
  //!
  //! If the source color is lighter than 0.5, the destination is lightened. If
  //! the source color is darker than 0.5, the destination is darkened, as if
  //! it were burned in. The degree of darkening or lightening is proportional
  //! to the difference between the source color and 0.5. If it is equal to 0.5,
  //! the destination is unchanged. Painting with pure black or white produces
  //! a distinctly darker or lighter area, but does not result in pure black or
  //! white.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (SOFT_LIGHT):
  //!
  //! if (2.Sca <= Sa)
  //!   Dca' = Dca.Sa - (Sa.Da - 2.Sca.Da).Dca.Sa.(Sa.Da - Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! else if (4.Dca <= Da)
  //!   Dca' = Dca.Sa + (2.Sca.Da - Sa.Da).((((16.Dsa.Sa - 12).Dsa.Sa + 4).Dsa.Da) - Dsa.Da) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! else
  //!   Dca' = Dca.Sa + (2.Sca.Da - Sa.Da).((Dca.Sa)^0.5 - Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!   Da'  = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_SOFT_LIGHT = 22,

  //! @brief Subtracts the darker of the two constituent colors from the
  //! lighter.
  //!
  //! Painting with white inverts the destination color. Painting with
  //! black produces no change.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (DIFFERENCE):
  //!   Dca' = abs(Dca.Sa - Sca.Da) + Sca.(1 - Da) + Dca.(1 - Sa)
  //!        = Dca + Sca - 2.min(Sca.Da, Dca.Sa)
  //!   Da'  = abs(Da.Sa - Sa.Da) + Sa.(1 - Da) + Da.(1 - Sa)
  //!        = Sa + Da - min(Sa.Da, Da.Sa)
  //!        = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_DIFFERENCE = 23,

  //! @brief Produces an effect similar to that of 'difference', but appears
  //! as lower contrast.
  //!
  //! Painting with white inverts the destination color. Painting with black
  //! produces no change.
  //!
  //! @verbatim
  //! Formulas for PRGB(dst), PRGB(src) colorspaces (EXCLUSION):
  //!   Dca' = Sca.Da + Dca - 2.Sca.Dca
  //!   Da'  = Sa + Da - Sa.Da
  //! @endverbatim
  COMPOSITE_EXCLUSION = 24,

  //! @brief Count of compositing operators (for error checking).
  COMPOSITE_COUNT = 25
};

// ============================================================================
// [Fog::DITHER_TYPE]
// ============================================================================

enum DITHER_TYPE
{
  DITHER_TYPE_NONE = 0,
  DITHER_TYPE_PATTERN = 1
};

// ============================================================================
// [Fog::FE_BLUR_LIMIT]
// ============================================================================

//! @brief Blur limitations
enum FE_BLUR_LIMIT
{
  //! @brief Maximum blur filter radius.
  FE_BLUR_LIMIT_RADIUS = 254
};

// ============================================================================
// [Fog::FE_BLUR_TYPE]
// ============================================================================

//! @brief Type of blur, see @c ImageFxFilter.
enum FE_BLUR_TYPE
{
  //! @brief Box blur effect (default).
  //!
  //! The box-blur effect is low-level quality blur, but very efficient. Radius
  //! has generally no effect on performance (or only little effect to process
  //! edge cases).
  //!
  //! The kernel of box blur is constant, for example the 5x5 kernel would be:
  //!
  //! @verbatim
  //!     | 1 1 1 1 1 |   | 1 |
  //!     | 1 1 1 1 1 |   | 1 |
  //! K = | 1 1 1 1 1 | = | 1 | * | 1 1 1 1 1 |
  //!     | 1 1 1 1 1 |   | 1 |
  //!     | 1 1 1 1 1 |   | 1 |
  //! @endverbatim
  FE_BLUR_TYPE_BOX = 0,

  //! @brief Stack blur effect.
  //!
  //! The stack blur effect`s quality is between box blur and gaussian blur.
  //! The result and the performance is optimal for the most operations,
  //! because like box blur, the radius has no effect on the blur performance.
  //!
  //! The kernel of stack box looks like:
  //!
  //! @verbatim
  //!     | 1 2 3 2 1 |   | 1 |
  //!     | 2 4 6 4 2 |   | 2 |
  //! K = | 3 6 9 6 3 | = | 3 | * | 1 2 3 2 1 |
  //!     | 2 4 6 4 2 |   | 2 |
  //!     | 1 2 3 2 1 |   | 1 |
  //! @endverbatim
  //!
  //! @note The stack blur was invented by Mario Klingemann, see:
  //!
  //!   - http://incubator.quasimondo.com/processing/fast_blur_deluxe.php
  //!   - http://www.quasimondo.com/StackBlurForCanvas/StackBlurDemo.html
  //!
  //! Please fix the documentation in case that the links are invalid.
  FE_BLUR_TYPE_STACK = 1,

  //! @brief Exponential blur effect.
  //!
  //! The exponential blur is fast compromise between box blur and gaussian
  //! blur, but it looks slightly different than stack blur. The reason why
  //! it's included in Fog-Framework is that it's already used by other
  //! libraries (KDE) so it's possible to get the similar result like these
  //! libs.
  //!
  //! The performance of exponential blur should be better than stack-blur,
  //! so it's generally the preferred blur algorithm for the most purposes.
  FE_BLUR_TYPE_EXPONENTIAL = 2,

  //! @brief True gaussian blur effect.
  //!
  //! @note Gaussian blur is implemented using approximation defined in SVG
  //! filters - <feGaussianBlur> element. Internally three box-blur passes
  //! are used, so the radius won't affect the overall performance.
  FE_BLUR_TYPE_GAUSSIAN = 3,

  //! @brief The default blur type.
  FE_BLUR_TYPE_DEFAULT = FE_BLUR_TYPE_BOX,

  //! @brief Count of blur effects.
  FE_BLUR_TYPE_COUNT = 4
};

// ============================================================================
// [Fog::FE_COLOR_MATRIX]
// ============================================================================

//! @brief Color matrix characteristics.
enum FE_COLOR_MATRIX
{
  //! @brief Matrix contains RGB shear part.
  //!
  //! RGB shear part is illustrated here:
  //!   [n X X n n]
  //!   [X n X n n]
  //!   [X X n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  FE_COLOR_MATRIX_SHEAR_RGB = 0x01,

  //! @brief Matrix contains alpha shear part.
  //!
  //! Alpha shear part is illustrated here:
  //!   [n n n X n]
  //!   [n n n X n]
  //!   [n n n X n]
  //!   [X X X n n]
  //!   [n n n n n]
  FE_COLOR_MATRIX_SHEAR_ALPHA = 0x02,

  //! @brief Matrix contains ARGB shear part.
  //!
  //! ARGB shear part is illustrated here:
  //!   [n X X X n]
  //!   [X n X X n]
  //!   [X X n X n]
  //!   [X X X n n]
  //!   [n n n n n]
  //!
  //! @note ARGB shear is combination of RGB and Alpha shear parts.
  FE_COLOR_MATRIX_SHEAR_ARGB = 0x03,

  //! @brief Matrix contains RGB lut part.
  //!
  //! RGB lut part is illustrated here:
  //!   [X n n n n]
  //!   [n X n n n]
  //!   [n n X n n]
  //!   [n n n n n]
  //!   [n n n n n]
  FE_COLOR_MATRIX_LUT_RGB = 0x04,

  //! @brief Matrix contains RGB lut part.
  //!
  //! Alpha lut part is illustrated here:
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n X n]
  //!   [n n n n n]
  FE_COLOR_MATRIX_LUT_ALPHA = 0x08,

  //! @brief Matrix contains ARGB lut part.
  //!
  //! ARGB lut part is illustrated here:
  //!   [X n n n n]
  //!   [n X n n n]
  //!   [n n X n n]
  //!   [n n n X n]
  //!   [n n n n n]
  //!
  //! @note ARGB lut is combination of RGB and Alpha lut parts.
  FE_COLOR_MATRIX_LUT_ARGB = 0x0C,

  //! @brief Matrix contains const RGB lut part (all cells are set to 1.0).
  //!
  //! Const RGB lut part is illustrated here:
  //!   [1 n n n n]
  //!   [n 1 n n n]
  //!   [n n 1 n n]
  //!   [n n n n n]
  //!   [n n n n n]
  FE_COLOR_MATRIX_CONST_RGB = 0x10,

  //! @brief Matrix contains const alpha lut part (cell set to 1.0).
  //!
  //! Const alpha lut part is illustrated here:
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n 1 n]
  //!   [n n n n n]
  FE_COLOR_MATRIX_CONST_ALPHA = 0x20,

  //! @brief Matrix contains const ARGB lut part (all cells are set to 1.0).
  //!
  //! Const ARGB lut part is illustrated here:
  //!   [1 n n n n]
  //!   [n 1 n n n]
  //!   [n n 1 n n]
  //!   [n n n 1 n]
  //!   [n n n n n]
  //!
  //! @note ARGB const lut is combination of RGB a Alpha const lut.
  FE_COLOR_MATRIX_CONST_ARGB = 0x30,

  //! @brief Matrix contains RGB translation part
  //!
  //! RGB translation part is illustrated here:
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [X X X n n]
  FE_COLOR_MATRIX_TRANSLATE_RGB  = 0x40,

  //! @brief Matrix contains alpha translation part
  //!
  //! Alpha translation part is illustrated here:
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n X n]
  FE_COLOR_MATRIX_TRANSLATE_ALPHA = 0x80,

  //! @brief Matrix contains ARGB translation part
  //!
  //! ARGB translation part is illustrated here:
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [n n n n n]
  //!   [X X X X n]
  //!
  //! @note ARGB translation is combination of RGB and Alpha translation parts.
  FE_COLOR_MATRIX_TRANSLATE_ARGB = 0xC0
};

// ============================================================================
// [Fog::FE_COMPONENT_FUNCTION]
// ============================================================================

enum FE_COMPONENT_FUNCTION
{
  FE_COMPONENT_FUNCTION_IDENTITY = 0,
  FE_COMPONENT_FUNCTION_TABLE = 1,
  FE_COMPONENT_FUNCTION_DISCRETE = 2,
  FE_COMPONENT_FUNCTION_LINEAR = 3,
  FE_COMPONENT_FUNCTION_GAMMA = 4,

  FE_COMPONENT_FUNCTION_COUNT = 5
};

// ============================================================================
// [Fog::FE_EXTEND]
// ============================================================================

//! @brief Border extend mode used by image effects (convolution and blurs).
enum FE_EXTEND
{
  //! @brief Borders are extended by a color.
  FE_EXTEND_COLOR = 0,

  //! @brief Borders are extended using pad.
  FE_EXTEND_PAD = 1,

  //! @brief Borders are extended using repead.
  FE_EXTEND_REPEAT = 2,

  //! @brief Borders are extended using reflect.
  FE_EXTEND_REFLECT = 3,

  //! @brief Default border extend type.
  FE_EXTEND_DEFAULT = FE_EXTEND_COLOR,

  //! @brief Count of border extend types (for error checking).
  FE_EXTEND_COUNT = 4
};

// ============================================================================
// [Fog::FE_MORPHOLOGY_TYPE]
// ============================================================================

enum FE_MORPHOLOGY_TYPE
{
  FE_MORPHOLOGY_TYPE_ERODE = 0,
  FE_MORPHOLOGY_TYPE_DILATE = 1,

  FE_MORPHOLOGY_TYPE_DEFAULT = FE_MORPHOLOGY_TYPE_ERODE,
  FE_MORPHOLOGY_TYPE_COUNT = 2
};

// ============================================================================
// [Fog::FE_TURBULENCE_TYPE]
// ============================================================================

enum FE_TURBULENCE_TYPE
{
  FE_TURBULENCE_TYPE_TURBULENCE = 0,
  FE_TURBULENCE_TYPE_FRACTAL_NOISE = 1,

  FE_TURBULENCE_TYPE_DEFAULT = FE_TURBULENCE_TYPE_TURBULENCE,
  FE_TURBULENCE_TYPE_COUNT = 2
};

// ============================================================================
// [Fog::FE_TYPE]
// ============================================================================

//! @brief Type of image filter.
enum FE_TYPE
{
  //! @brief No filter.
  FE_TYPE_NONE = 0,

  // --------------------------------------------------------------------------
  // [Color Filters]
  // --------------------------------------------------------------------------

  //! @brief @ref FeColorLut filter.
  FE_TYPE_COLOR_LUT = 1,
  //! @brief @ref FeColorMatrix filter.
  FE_TYPE_COLOR_MATRIX = 2,
  //! @brief @ref FeComponentTransfer filter
  FE_TYPE_COMPONENT_TRANSFER = 3,

  // --------------------------------------------------------------------------
  // [Image Filters]
  // --------------------------------------------------------------------------

  //! @brief @ref FeBlur filter.
  FE_TYPE_BLUR = 4,
  //! @brief @ref FeConvolveMatrix filter.
  FE_TYPE_CONVOLVE_MATRIX = 5,
  //! @brief @ref FeConvolveSeparable filter.
  FE_TYPE_CONVOLVE_SEPARABLE = 6,
  //! @brief @ref FeMorphology filter
  FE_TYPE_MORPHOLOGY = 7,

  // --------------------------------------------------------------------------
  // [Image Generators]
  // --------------------------------------------------------------------------

  //! @brief @ref FeTurbulence filter.
  FE_TYPE_TURBULENCE = 8,

  //! @brief Count of image filter types.
  FE_TYPE_COUNT = 9
};

// ============================================================================
// [Fog::FILL_RULE]
// ============================================================================

//! @brief Fill rule.
enum FILL_RULE
{
  //! @brief Fill using non-zero rule.
  FILL_RULE_NON_ZERO = 0,
  //! @brief Fill using even-odd rule.
  FILL_RULE_EVEN_ODD = 1,

  //! @brief Default fill-rule.
  FILL_RULE_DEFAULT = FILL_RULE_NON_ZERO,
  //! @brief Used to catch invalid arguments.
  FILL_RULE_COUNT = 2
};

// ============================================================================
// [Fog::FONT_CAPS]
// ============================================================================

//! @brief Font caps.
enum FONT_CAPS
{
  //! @brief Normal.
  FONT_CAPS_NORMAL = 0,

  //! @brief Enables display of small capitals (OpenType feature: smcp).
  //!
  //! Small-caps glyphs typically use the form of uppercase letters but are 
  //! reduced to the size of lowercase letters.
  FONT_CAPS_SMALL = 1,
  //! @brief Enables display of small capitals for both upper and lowercase
  //! letters (OpenType features: c2sc, smcp).
  FONT_CAPS_ALL_SMALL = 2,

  //! @brief Enables display of petite capitals (OpenType feature: pcap).
  FONT_CAPS_PETITE = 3,
  //! @brief Enables display of petite capitals for both upper and lowercase
  //! letters (OpenType features: c2pc, pcap).
  FONT_CAPS_ALL_PETITE = 4,

  //! Enables display of titling capitals (OpenType feature: titl).
  //!
  //! Uppercase letter glyphs are often designed for use with lowercase letters.
  //! When used in all uppercase titling sequences they can appear too strong.
  //! Titling capitals are designed specifically for this situation.
  FONT_CAPS_TITLING = 5,

  //! @brief Enables display of mixture of small capitals for uppercase letters
  //! with normal lowercase letters (OpenType feature: unic).
  FONT_CAPS_UNICASE = 6,

  //! @brief Count of FONT_CAPS values.
  FONT_CAPS_COUNT = 7
};

// ============================================================================
// [Fog::FONT_DECORATION]
// ============================================================================

enum FONT_DECORATION
{
  FONT_DECORATION_NONE = 0x00,
  FONT_DECORATION_UNDERLINE = 0x01,
  FONT_DECORATION_STRIKETHROUGH = 0x02,

  FONT_DECORATION_MAX = 0x03
};

// ============================================================================
// [Fog::FONT_EAST_ASIAN_VARIANT]
// ============================================================================

enum FONT_EAST_ASIAN_VARIANT
{
  FONT_EAST_ASIAN_VARIANT_JIS78 = 0,
  FONT_EAST_ASIAN_VARIANT_JIS83 = 1,
  FONT_EAST_ASIAN_VARIANT_JIS90 = 2,
  FONT_EAST_ASIAN_VARIANT_JIS04 = 3,
  FONT_EAST_ASIAN_VARIANT_SIMPLIFIED = 4,
  FONT_EAST_ASIAN_VARIANT_TRADITIONAL = 5,

  FONT_EAST_ASIAN_VARIANT_COUNT = 6
};

// ============================================================================
// [Fog::FONT_EAST_ASIAN_WIDTH]
// ============================================================================

enum FONT_EAST_ASIAN_WIDTH
{
  FONT_EAST_ASIAN_WIDTH_FULL = 0,
  FONT_EAST_ASIAN_WIDTH_PROPORTIONAL = 1,

  FONT_EAST_ASIAN_WIDTH_COUNT = 2
};

// ============================================================================
// [Fog::FONT_ENGINE]
// ============================================================================

//! @brief Font-provider IDs.
enum FONT_ENGINE
{
  //! @brief Null font engine.
  //!
  //! Created only if something failed.
  FONT_ENGINE_NULL = 0,
  //! @brief Windows font engine.
  FONT_ENGINE_WINDOWS = 1,
  //! @brief Mac font engine.
  FONT_ENGINE_MAC = 2,
  //! @brief Freetype font engine.
  FONT_ENGINE_FREETYPE = 3
};

// ============================================================================
// [Fog::FONT_FAMILY]
// ============================================================================

//! @brief Standard font family IDs (defined by CSS).
enum FONT_FAMILY
{
  //! @brief Unknown font (not categorized family).
  FONT_FAMILY_UNKNOWN = 0,

  //! @brief "serif" font.
  //!
  //! Glyphs of serif fonts, as the term is used in CSS, tend to have finishing
  //! strokes, flared or tapering ends, or have actual serifed endings (including
  //! slab serifs). Serif fonts are typically proportionately-spaced. They often
  //! display a greater variation between thick and thin strokes than fonts from
  //! the "sans-serif" generic font family. CSS uses the term "serif" to apply to
  //! a font for any script, although other names may be more familiar for
  //! particular scripts, such as Mincho (Japanese), Sung or Song (Chinese),
  //! Totum or Kodig (Korean). Any font that is so described may be used to
  //! represent the generic "serif" family.
  //!
  //! Latin fonts
  //!   - Times New Roman,
  //!   - Bodoni, Garamond,
  //!   - Minion Web,
  //!   - ITC Stone Serif,
  //!   - MS Georgia,
  //!   - Bitstream Cyberbit.
  //!
  //! Greek fonts
  //!   - Bitstream Cyberbit.
  //!
  //! Cyrillic fonts
  //!   - Adobe Minion Cyrillic,
  //!   - Excelsior Cyrillic Upright,
  //!   - Monotype Albion 70,
  //!   - Bitstream Cyberbit,
  //!   - ER Bukinist.
  //!
  //! Hebrew fonts
  //!   - New Peninim,
  //!   - Raanana,
  //!   - Bitstream Cyberbit.
  //!
  //! Japanese fonts
  //!   - Ryumin Light-KL,
  //!   - Kyokasho ICA,
  //!   - Futo Min A101.
  //!
  //! Arabic fonts
  //!   - Bitstream Cyberbit.
  FONT_FAMILY_SERIF = 1,

  //! @brief "sans-serif" font.
  //!
  //! Glyphs in sans-serif fonts, as the term is used in CSS, tend to have
  //! stroke endings that are plain - with little or no flaring, cross stroke,
  //! or other ornamentation. Sans-serif fonts are typically proportionately-
  //! spaced. They often have little variation between thick and thin strokes,
  //! compared to fonts from the "serif" family. CSS uses the term 'sans-serif'
  //! to apply to a font for any script, although other names may be more
  //! familiar for particular scripts, such as Gothic (Japanese), Kai (Chinese),
  //! or Pathang (Korean). Any font that is so described may be used to
  //! represent the generic ?sans-serif" family.
  //!
  //! Latin fonts:
  //!   - MS Trebuchet,
  //!   - ITC Avant Garde Gothic,
  //!   - MS Arial,
  //!   - MS Verdana,
  //!   - Univers,
  //!   - Futura,
  //!   - ITC Stone Sans,
  //!   - Gill Sans,
  //!   - Akzidenz Grotesk,
  //!   - Helvetica.
  //!
  //! Greek fonts
  //!   - Attika,
  //!   - Typiko New Era,
  //!   - MS Tahoma,
  //!   - Monotype Gill Sans 571,
  //!   - Helvetica Greek.
  //!
  //! Cyrillic fonts
  //!   - Helvetica Cyrillic,
  //!   - ER Univers,
  //!   - Lucida Sans Unicode,
  //!   - Bastion.
  //!
  //! Hebrew fonts
  //!   - Arial Hebrew,
  //!   - MS Tahoma.
  //!
  //! Japanese fonts
  //!   - Shin Go,
  //!   - Heisei Kaku Gothic W5.
  //!
  //! Arabic fonts
  //!   - MS Tahoma.
  FONT_FAMILY_SANS_SERIF = 2,

  //! @brief "cursive" font.
  //!
  //! Glyphs in cursive fonts, as the term is used in CSS, generally have either
  //! joining strokes or other cursive characteristics beyond those of italic
  //! typefaces. The glyphs are partially or completely connected, and the
  //! result looks more like handwritten pen or brush writing than printed
  //! letterwork. Fonts for some scripts, such as Arabic, are almost always
  //! cursive. CSS uses the term 'cursive' to apply to a font for any script,
  //! although other names such as Chancery, Brush, Swing and Script are also
  //! used in font names.
  //!
  //! Latin fonts:
  //!   - Caflisch Script,
  //!   - Adobe Poetica,
  //!   - Sanvito,
  //!   - Ex Ponto,
  //!   - Snell Roundhand,
  //!   - Zapf-Chancery.
  //!
  //! Cyrillic fonts:
  //!   - ER Architekt.
  //!
  //! Hebrew fonts:
  //!   - Corsiva.
  //!
  //! Arabic fonts:
  //!   - DecoType Naskh,
  //!   - Monotype Urdu 507.
  FONT_FAMILY_CUSRIVE = 3,

  //! @brief "fantasy" font.
  //!
  //! Fantasy fonts, as used in CSS, are primarily decorative while still
  //! containing representations of characters (as opposed to Pi or Picture
  //! fonts, which do not represent characters).
  //!
  //! Latin fonts:
  //! - Alpha Geometrique,
  //! - Critter,
  //! - Cottonwood,
  //! - FB Reactor,
  //! - Studz.
  FONT_FAMILY_FANTASY = 4,

  //! @brief "monospace" font.
  //!
  //! The sole criterion of a monospace font is that all glyphs have the same
  //! fixed width. (This can make some scripts, such as Arabic, look most
  //! peculiar.) The effect is similar to a manual typewriter, and is often
  //! used to set samples of computer code.
  //!
  //! Latin fonts:
  //!   - Courier,
  //!   - Courier New,
  //!   - Lucida Console,
  //!   - Monaco.
  //!
  //! Greek fonts:
  //!   - MS Courier New,
  //!   - Everson Mono.
  //!
  //! Cyrillic fonts:
  //!   - ER Kurier, Everson Mono.
  //!
  //! Japanese fonts:
  //!   - Osaka Monospaced.
  FONT_FAMILY_MONOSPACE = 5,

  //! @brief Count of font-family IDs.
  FONT_FAMILY_COUNT = 6
};

// ============================================================================
// [Fog::FONT_FEATURE]
// ============================================================================

enum FONT_FEATURE
{
  //! @brief Font supports kerning.
  FONT_FEATURE_KERNING = 0x00000001
};

// ============================================================================
// [Fog::FONT_FLAG]
// ============================================================================

enum FONT_FLAG
{
  //! @brief Whether the font-size is aligned to integer (pixel).
  FONT_FLAG_IS_ALIGNED_SIZE = 0x01,
  //! @brief Font properties contains custom letter-spacing.
  FONT_FLAG_HAS_LETTER_SPACING = 0x02,
  //! @brief Font properties contains custom word-spacing.
  FONT_FLAG_HAS_WORD_SPACING = 0x04,
  //! @brief Whether the font transformation matrix is not identity.
  FONT_FLAG_HAS_MATRIX = 0x08
};

// ============================================================================
// [Fog::FONT_HINTING]
// ============================================================================

enum FONT_HINTING
{
  FONT_HINTING_DISABLED = 0x0,
  FONT_HINTING_VERTICAL = 0x1,
  FONT_HINTING_BOTH = 0x2,
  FONT_HINTING_AUTO = 0xF
};

// ============================================================================
// [Fog::FONT_KERNING]
// ============================================================================

//! @brief Font-kerning mode.
enum FONT_KERNING
{
  //! @brief Disable the use of kerning.
  FONT_KERNING_DISABLED = 0,
  //! @brief Enable the use of kerning (default).
  FONT_KERNING_ENABLED = 1,

  //! @brief Count of FONT_KERNING options.
  FONT_KERNING_COUNT = 2
};

// ============================================================================
// [Fog::FONT_NUMERIC_FIGURE]
// ============================================================================

enum FONT_NUMERIC_FIGURE
{
  FONT_NUMERIC_FIGURE_LINING = 0,
  FONT_NUMERIC_FIGURE_OLD_STYLE = 1,

  FONT_NUMERIC_FIGURE_COUNT = 2
};

// ============================================================================
// [Fog::FONT_NUMERIC_FRACTION]
// ============================================================================

enum FONT_NUMERIC_FRACTION
{
  FONT_NUMERIC_FRACTION_NONE = 0,
  FONT_NUMERIC_FRACTION_DIAGONAL = 1,
  FONT_NUMERIC_FRACTION_STACKED = 2,

  FONT_NUMERIC_FRACTION_COUNT = 3
};

// ============================================================================
// [Fog::FONT_NUMERIC_SPACING]
// ============================================================================

enum FONT_NUMERIC_SPACING
{
  FONT_NUMERIC_SPACING_PROPORTIONAL = 0,
  FONT_NUMERIC_SPACING_TABULAR = 1,

  FONT_NUMERIC_SPACING_COUNT = 2
};

// ============================================================================
// [Fog::FONT_PARAM]
// ============================================================================

enum FONT_PARAM
{
  FONT_PARAM_SIZE = 0,
  FONT_PARAM_WEIGHT,
  FONT_PARAM_STRETCH,
  FONT_PARAM_DECORATION,
  FONT_PARAM_STYLE,
  FONT_PARAM_KERNING,
  FONT_PARAM_COMMON_LIGATURES,
  FONT_PARAM_DISCRETIONARY_LIGATURES,
  FONT_PARAM_HISTORICAL_LIGATURES,
  FONT_PARAM_CAPS,
  FONT_PARAM_NUMERIC_FIGURE,
  FONT_PARAM_NUMERIC_SPACING,
  FONT_PARAM_NUMERIC_FRACTION,
  FONT_PARAM_NUMERIC_SLASHED_ZERO,
  FONT_PARAM_EAST_ASIAN_VARIANT,
  FONT_PARAM_EAST_ASIAN_WIDTH,
  FONT_PARAM_LETTER_SPACING,
  FONT_PARAM_WORD_SPACING,
  FONT_PARAM_SIZE_ADJUST,

  FONT_PARAM_FEATURES,
  FONT_PARAM_MATRIX,

  FONT_PARAM_COUNT
};

// ============================================================================
// [Fog::FONT_QUALITY]
// ============================================================================

//! @brief Font quality.
enum FONT_QUALITY
{
  //! @brief No antialiasing.
  FONT_QUALITY_ALIASED = 0,
  //! @brief Grey antialiasing.
  FONT_QUALITY_GREY = 1,
  //! @brief LCD subpixel antialiasing (only enabled for the LCD output device).
  FONT_QUALITY_LCD = 2,

  //! @brief Use default font quality.
  FONT_QUALITY_DETECT = 0xFF,
  //! @brief Count of font-quality options.
  FONT_QUALITY_COUNT = 3
};

// ============================================================================
// [Fog::FONT_SPACING]
// ============================================================================

//! @brief Font spacing mode.
enum FONT_SPACING
{
  //! @brief Spacing is absolute.
  FONT_SPACING_ABSOLUTE = 0,
  //! @brief Spacing is percentage (in Fog 0.0 to 1.0, inclusive) of the glyph
  //! spacing.
  FONT_SPACING_PERCENTAGE = 1,

  //! @brief Count of font spacing modes.
  FONT_SPACING_COUNT = 2
};

// ============================================================================
// [Fog::FONT_STRETCH]
// ============================================================================

enum FONT_STRETCH
{
  FONT_STRETCH_ULTRA_CONDENSED = 10,
  FONT_STRETCH_EXTRA_CONDENSED = 20,
  FONT_STRETCH_CONDENSED = 30,
  FONT_STRETCH_SEMI_CONDENSED = 40,
  FONT_STRETCH_NORMAL = 50,
  FONT_STRETCH_SEMI_EXPANDED = 60,
  FONT_STRETCH_EXPANDED = 70,
  FONT_STRETCH_EXTRA_EXPANDED = 80,
  FONT_STRETCH_ULTRA_EXPANDED = 90,

  FONT_STRETCH_MAX = 100
};

// ============================================================================
// [Fog::FONT_STYLE]
// ============================================================================

//! @brief Font style.
enum FONT_STYLE
{
  //! @brief Normal style.
  FONT_STYLE_NORMAL = 0,

  //! @brief Oblique (slanted, sloped) style.
  //!
  //! Form of type that slants slightly to the right, using shearing transform
  //! of original glyph-set.
  FONT_STYLE_OBLIQUE = 1,

  //! @brief Italic style.
  //!
  //! Form of type that slants slightly to the right, using different glyph-set.
  //! If glyph-set for italic is not available, the @c FONT_STYLE_OBLIQUE is
  //! used instead.
  FONT_STYLE_ITALIC = 2,

  //! @brief Count of font styles.
  FONT_STYLE_COUNT = 3
};

// ============================================================================
// [Fog::FONT_WEIGHT]
// ============================================================================

enum FONT_WEIGHT
{
  FONT_WEIGHT_100 = 10,
  FONT_WEIGHT_200 = 20,
  FONT_WEIGHT_300 = 30,
  FONT_WEIGHT_400 = 40,
  FONT_WEIGHT_500 = 50,
  FONT_WEIGHT_600 = 60,
  FONT_WEIGHT_700 = 70,
  FONT_WEIGHT_800 = 80,
  FONT_WEIGHT_900 = 90,

  FONT_WEIGHT_THIN       = FONT_WEIGHT_100,
  FONT_WEIGHT_EXTRALIGHT = FONT_WEIGHT_200,
  FONT_WEIGHT_LIGHT      = FONT_WEIGHT_300,
  FONT_WEIGHT_NORMAL     = FONT_WEIGHT_400,
  FONT_WEIGHT_MEDIUM     = FONT_WEIGHT_500,
  FONT_WEIGHT_SEMIBOLD   = FONT_WEIGHT_600,
  FONT_WEIGHT_BOLD       = FONT_WEIGHT_700,
  FONT_WEIGHT_EXTRABOLD  = FONT_WEIGHT_800,
  FONT_WEIGHT_BLACK      = FONT_WEIGHT_900,

  FONT_WEIGHT_MAX = 100
};

// ============================================================================
// [Fog::GEOMETRIC_PRECISION]
// ============================================================================

//! @brief The geometic precision hint.
enum GEOMETRIC_PRECISION
{
  //! @brief Standard geometric precision (default).
  //!
  //! Standard geometric precision uses single-precision or double-precision
  //! floating points. The usage of single-precision floating points is
  //! determined by input parameters. For example if shape that is being
  //! painted is given in single-precision floating point vectors, then
  //! single-precision floating point operations will be used to do the
  //! transformations, clipping and final painting.
  GEOMETRIC_PRECISION_NORMAL = 0,

  //! @brief High geometric precision.
  //!
  //! High geometric precision uses always double-precision floating points
  //! regardless of input arguments.
  //!
  //! @note High geometric precision may be very expensive when running on CPU
  //! without double-precision floating point unit.
  GEOMETRIC_PRECISION_HIGH = 1,

  GEOMETRIC_PRECISION_DEFAULT = GEOMETRIC_PRECISION_NORMAL,
  GEOMETRIC_PRECISION_COUNT = 2
};

// ============================================================================
// [Fog::GRADIENT_QUALITY]
// ============================================================================

//! @brief The color interpolation type, used by @c Painter.
enum GRADIENT_QUALITY
{
  //! @brief Normal interpolation quality (good for the most purposes).
  GRADIENT_QUALITY_NORMAL = 0,
  //! @brief High interpolation quality (slower, but more precise).
  GRADIENT_QUALITY_HIGH = 1,

  //! @brief Default interpolation quality, @c GRADIENT_QUALITY_NORMAL.
  GRADIENT_QUALITY_DEFAULT = GRADIENT_QUALITY_NORMAL,
  //! @brief Count of interpolation quality options.
  GRADIENT_QUALITY_COUNT = 2
};

// ============================================================================
// [Fog::GRADIENT_SPREAD]
// ============================================================================

//! @brief Spread type.
enum GRADIENT_SPREAD
{
  //! @brief Pad spread (area outside the shape continues using border color).
  GRADIENT_SPREAD_PAD = 0,
  //! @brief Releat spread (pattern is repeated).
  GRADIENT_SPREAD_REPEAT = 1,
  //! @brief Reflect spread (pattern is reflected and then repeated).
  GRADIENT_SPREAD_REFLECT = 2,

  //! @brief Default spread.
  GRADIENT_SPREAD_DEFAULT = GRADIENT_SPREAD_PAD,
  //! @brief Count of spread types.
  GRADIENT_SPREAD_COUNT = 3
};

// ============================================================================
// [Fog::GRADIENT_TYPE]
// ============================================================================

enum GRADIENT_TYPE
{
  //! @brief Linear gradient type.
  GRADIENT_TYPE_LINEAR = 0,
  //! @brief Radial gradient type.
  GRADIENT_TYPE_RADIAL = 1,
  //! @brief Rectangular gradient type.
  GRADIENT_TYPE_RECTANGULAR = 2,
  //! @brief Conical gradient type.
  GRADIENT_TYPE_CONICAL = 3,

  //! @brief Invalid gradient type, used internally.
  GRADIENT_TYPE_INVALID = 4,
  //! @brief Count of gradient types.
  GRADIENT_TYPE_COUNT = 4
};

// ============================================================================
// [Fog::IMAGE_ADOPT]
// ============================================================================

//! @brief Image adopt flags.
enum IMAGE_ADOPT
{
  //! @brief Standard adopt behavior
  IMAGE_ADOPT_DEFAULT = 0x00,
  //! @brief Adopted image will be read-only.
  IMAGE_ATOPT_READ_ONLY = 0x01,
  //! @brief Adopted image data are from bottom-to-top.
  //!
  //! Useful flag for Windows-DIB adoption.
  IMAGE_ADOPT_REVERSED = 0x02
};

// ============================================================================
// [Fog::IMAGE_COLOR_KEY]
// ============================================================================

enum IMAGE_COLOR_KEY
{
  IMAGE_COLOR_KEY_NONE = 0x100
};

// ============================================================================
// [Fog::IMAGE_TYPE]
// ============================================================================

//! @brief Image type.
enum IMAGE_TYPE
{
  //! @brief Image is a memory buffer (the default).
  //!
  //! @note This is the default, platform independent, image type.
  IMAGE_TYPE_BUFFER = 0,

  //! @brief Image is a Windows DIBSECTION.
  //!
  //! Fog-Framework can only create or adopt DIBSECTION which is compatible to
  //! the @c IMAGE_FORMAT. Different pixel formats can't be used inside of the 
  //! @c Image container.
  //!
  //! @note This is Windows-only image type.
  IMAGE_TYPE_WIN_DIB = 1,

  //! @brief Image is a Mac @c CGImageRef.
  //!
  //! Fog-Framework can only create or adopt CGImageRef which is compatible to
  //! the @c IMAGE_FORMAT. Different pixel formats can't be used inside of the 
  //! @c Image container.
  //!
  //! @note This is Mac-only image type.
  IMAGE_TYPE_MAC_CG = 2,

  //! @brief Count of image types.
  IMAGE_TYPE_COUNT = 3,

  //! @brief Ignore the image type (used by some functions inside @c Image).
  IMAGE_TYPE_IGNORE = 0xFF
};

// ============================================================================
// [Fog::IMAGE_COMPONENT]
// ============================================================================

//! @brief Image components.
enum IMAGE_COMPONENT
{
  //! @brief No components.
  IMAGE_COMPONENT_NONE = 0x00000000,

  //! @brief Component mask.
  IMAGE_COMPONENT_MASK = 0x00000003,

  //! @brief Alpha component available.
  //!
  //! Related pixel formats:
  //!   - @c IMAGE_FORMAT_A8.
  //!   - @c IMAGE_FORMAT_A16.
  IMAGE_COMPONENT_ALPHA = 0x00000001,

  //! @brief RGB components available.
  //!
  //! Related pixel formats:
  //!   - @c IMAGE_FORMAT_XRGB32.
  //!   - @c IMAGE_FORMAT_RGB24.
  //!   - @c IMAGE_FORMAT_RGB48.
  IMAGE_COMPONENT_RGB = 0x00000002,

  //! @brief Image contains ARGB entities (RGB and ALPHA).
  //!
  //! Related pixel formats:
  //!   - @c IMAGE_FORMAT_PRGB32.
  //!   - @c IMAGE_FORMAT_PRGB64.
  IMAGE_COMPONENT_ARGB = IMAGE_COMPONENT_ALPHA | IMAGE_COMPONENT_RGB
};

// ============================================================================
// [Fog::IMAGE_CODEC]
// ============================================================================

//! @brief Type of @c ImageCodec class.
enum IMAGE_CODEC
{
  //! @brief None, null codec or non-initialized (shouldn't be used in public code).
  IMAGE_CODEC_NONE = 0x0,
  //! @brief Image encoder.
  IMAGE_CODEC_ENCODER = 0x1,
  //! @brief Image decoder.
  IMAGE_CODEC_DECODER = 0x2,
  //! @brief Image decoder and encoder.
  IMAGE_CODEC_BOTH = 0x3
};

// ============================================================================
// [Fog::IMAGE_FD_FLAGS]
// ============================================================================

enum IMAGE_FD_FLAGS
{
  IMAGE_FD_NONE,

  IMAGE_FD_IS_PREMULTIPLIED = 0x01,
  IMAGE_FD_IS_BYTESWAPPED = 0x02,
  IMAGE_FD_FILL_UNUSED_BITS = 0x04
};

// ============================================================================
// [Fog::IMAGE_FORMAT]
// ============================================================================

//! @brief Pixel format.
//!
//! @note The memory representation of nearly all pixel formats depends on the
//! CPU endianness.
//!
//! @c IMAGE_FORMAT_PRGB32, @c IMAGE_FORMAT_XRGB32:
//! - Memory       : 00|01|02|03
//! - Little endian: BB|GG|RR|AA
//! - Big endian   : AA|RR|GG|BB
//!
//! @c IMAGE_FORMAT_RGB24:
//! - Memory       : 00|01|02
//! - Little endian: BB|GG|RR
//! - Big endian   : RR|GG|BB
//!
//! @c IMAGE_FORMAT_A8:
//! - No difference: AA (8-bit alpha value, indices to palette are also valid).
//!
//! @c IMAGE_FORMAT_I8:
//! - No difference: II (8-bit index value to palette)
//!
//! @c IMAGE_FORMAT_PRGB64
//! - Memory       : 00|01|02|03|04|05|06|07
//! - Little endian: B0|B1|G0|G1|R0|R1|A0|A1
//! - Big endian   : A1|A0|R1|R0|G1|G0|B1|B0
//!
//! @c IMAGE_FORMAT_RGB48
//! - Memory       : 00|01|02|03|04|05
//! - Little endian: B0|B1|G0|G1|R0|R1
//! - Big endian   : R1|R0|G1|G0|B1|B0
//!
//! @c IMAGE_FORMAT_A16
//! - Memory       : 00|01|
//! - Little endian: A0|A1|
//! - Big endian   : A1|A0|
//!
//! @note The terminology used for position is the same as in the @c Fog-Face
//! framework:
//!
//! @verbatim
//! - Byte0: (Value >>  0) & 0xFF
//! - Byte1: (Value >>  8) & 0xFF
//! - Byte2: (Value >> 16) & 0xFF
//! - Byte3: (Value >> 24) & 0xFF
//! @endverbatim
enum IMAGE_FORMAT
{
  // --------------------------------------------------------------------------
  // NOTE: When changing order, removing or adding image format please match
  // ${IMAGE_FORMAT:BEGIN} -> ${IMAGE_FORMAT:END} and fix the code that depends
  // on the image format order.
  // --------------------------------------------------------------------------

  // --------------------------------------------------------------------------
  // [8-bit Per Component or Less]
  // --------------------------------------------------------------------------

  //! @brief 32-bit ARGB (8 bits per component), premultiplied.
  //!
  //! Supported natively by these libraries:
  //!   - Cairo (CAIRO_FORMAT_ARGB32)
  //!   - CGImageRef (8bpc, 32bpp, Default byte-order, premultiplied alpha last).
  //!   - DIBSECTION (32bpp, RGB)
  //!   - Qt (QImage::Format_ARGB32_Premultiplied)
  IMAGE_FORMAT_PRGB32 = 0,

  //! @brief 32-bit RGB (8 bits per component).
  //!
  //! Supported natively by these libraries:
  //!   - Cairo (CAIRO_FORMAT_RGB24)
  //!   - CGImageRef (8bpc, 32bpp, Default byte-order, skip alpha last).
  //!   - DIBSECTION (32bpp, RGB)
  //!   - Qt (QImage::Format_RGB32)
  IMAGE_FORMAT_XRGB32 = 1,

  //! @brief 24-bit RGB (8 bits per component).
  //!
  //! Supported natively by these libraries:
  //!   - DIBSECTION (24bpp, RGB)
  IMAGE_FORMAT_RGB24 = 2,

  //! @brief 8-bit ALPHA.
  //!
  //! Supported natively by these libraries:
  //!   - Cairo (CAIRO_FORMAT_A8)
  //!   - CGImageRef (8bpc, 8bpp, alpha only).
  //!   - DIBSECTION (8bpp, palette)
  IMAGE_FORMAT_A8 = 3,

  //! @brief 8-bit INDEXED (RGB32 palette, without alpha).
  IMAGE_FORMAT_I8 = 4,

  // --------------------------------------------------------------------------
  // [16-bit Per Component]
  // --------------------------------------------------------------------------

  //! @brief 64-bit ARGB (16 bits per component), premultiplied.
  IMAGE_FORMAT_PRGB64 = 5,
  //! @brief 48-bit RGB (16 bits per component).
  IMAGE_FORMAT_RGB48 = 6,
  //! @brief 16-bit ALPHA.
  IMAGE_FORMAT_A16 = 7,

  // --------------------------------------------------------------------------
  // [...]
  // --------------------------------------------------------------------------

  //! @brief Count of image formats.
  IMAGE_FORMAT_COUNT = 8,

  //! @brief Null image format (used only by empty images).
  IMAGE_FORMAT_NULL = IMAGE_FORMAT_COUNT,

  //! @brief Ignore image format (used by some functions inside the @c Image).
  IMAGE_FORMAT_IGNORE = IMAGE_FORMAT_COUNT
};

// ============================================================================
// [Fog::IMAGE_LIMITS]
// ============================================================================

//! @brief Image limits.
enum IMAGE_LIMITS
{
  //! @brief Maximum image width (65536 in pixels).
  IMAGE_MAX_WIDTH = 0x10000,
  //! @brief Maximum image height (65536 in pixels).
  IMAGE_MAX_HEIGHT = 0x10000
};

// ============================================================================
// [Fog::IMAGE_MIRROR_MODE]
// ============================================================================

//! @brief Mirror modes used together with @c Image::mirror().
enum IMAGE_MIRROR_MODE
{
  IMAGE_MIRROR_NONE = 0,
  IMAGE_MIRROR_HORIZONTAL = 1,
  IMAGE_MIRROR_VERTICAL = 2,
  IMAGE_MIRROR_BOTH = 3,

  IMAGE_MIRROR_COUNT = 4
};

// ============================================================================
// [Fog::IMAGE_PRECISION]
// ============================================================================

enum IMAGE_PRECISION
{
  IMAGE_PRECISION_BYTE = 0,
  IMAGE_PRECISION_WORD = 1,

  IMAGE_PRECISION_COUNT = 2
};

// ============================================================================
// [Fog::IMAGE_RESIZE]
// ============================================================================

//! @brief Built-in resize methods available to Image::resize() function.
enum IMAGE_RESIZE
{
  //! @brief Nearest neighbor filter (radius 1.0).
  IMAGE_RESIZE_NEAREST = 0,

  //! @brief Bilinear filter (radius 1.0).
  IMAGE_RESIZE_BILINEAR = 1,

  //! @brief Bicubic filter (radius 2.0).
  IMAGE_RESIZE_BICUBIC = 2,

  //! @brief Bell filter (radius 1.5).
  IMAGE_RESIZE_BELL = 3,

  //! @brief Gauss filter (radius 2.0).
  IMAGE_RESIZE_GAUSS = 4,

  //! @brief Hermite filter (radius 1.0).
  IMAGE_RESIZE_HERMITE = 5,

  //! @brief Hanning filter (radius 1.0).
  IMAGE_RESIZE_HANNING = 6,

  //! @brief Catrom filter (radius 2.0).
  IMAGE_RESIZE_CATROM = 7,

  //! @brief Mitchell filter (radius 2.0).
  //!
  //! The Mitchell filter accepts the following parameters:
  //!   - @a "b" - b value used to calculate the final polynoms p and q, default 1/3.
  //!   - @a "c" - c value used to calculate the final polynoms p and q, default 1/3.
  IMAGE_RESIZE_MITCHELL = 8,

  //! @brief Bessel filter (radius 3.2383).
  IMAGE_RESIZE_BESSEL = 9,

  //! @brief Sinc filter (user defined radius, default 2.0).
  //!
  //! The Sinc filter accepts the following parameters:
  //!   - @a "radius" - filter radius, default 2.0.
  IMAGE_RESIZE_SINC = 10,

  //! @brief Lanczos filter (user defined radius, default 2.0).
  //!
  //! The Lanczos filter accepts the following parameters:
  //!   - @a "radius" - filter radius, default 2.0.
  IMAGE_RESIZE_LANCZOS = 11,

  //! @brief Blackman filter (user defined radius, default 2.0).
  //!
  //! The Blackman filter accepts the following parameters:
  //!   - @a "radius" - filter radius, default 2.0.
  IMAGE_RESIZE_BLACKMAN = 12,

  //! @brief Count of IMAGE_RESIZE filters.
  IMAGE_RESIZE_COUNT = 13
};

// ============================================================================
// [Fog::IMAGE_ROTATE_MODE]
// ============================================================================

//! @brief Rotate modes used together with @c Image::rotate() methods.
enum IMAGE_ROTATE_MODE
{
  IMAGE_ROTATE_0 = 0,
  IMAGE_ROTATE_90 = 1,
  IMAGE_ROTATE_180 = 2,
  IMAGE_ROTATE_270 = 3,

  IMAGE_ROTATE_COUNT = 4
};

// ============================================================================
// [Fog::IMAGE_STREAM]
// ============================================================================

//! @brief Image file type.
enum IMAGE_STREAM
{
  IMAGE_STREAM_NONE = 0,

  IMAGE_STREAM_ANI,
  IMAGE_STREAM_APNG,
  IMAGE_STREAM_BMP,
  IMAGE_STREAM_FLI,
  IMAGE_STREAM_FLC,
  IMAGE_STREAM_GIF,
  IMAGE_STREAM_ICO,
  IMAGE_STREAM_JPEG,
  IMAGE_STREAM_LBM,
  IMAGE_STREAM_MNG,
  IMAGE_STREAM_PCX,
  IMAGE_STREAM_PNG,
  IMAGE_STREAM_PNM,
  IMAGE_STREAM_TGA,
  IMAGE_STREAM_TIFF,
  IMAGE_STREAM_XBM,
  IMAGE_STREAM_XPM,

  IMAGE_STREAM_CUSTOM = 65536
};

// ============================================================================
// [Fog::IMAGE_QUALITY]
// ============================================================================

//! @brief Image interpolation type, used by @c Painter or @c Image::scale().
enum IMAGE_QUALITY
{
  IMAGE_QUALITY_NEAREST = 0,
  IMAGE_QUALITY_BILINEAR = 1,
  IMAGE_QUALITY_BICUBIC = 2,

  IMAGE_QUALITY_BILINEAR_HQ = 3,
  IMAGE_QUALITY_BICUBIC_HQ = 4,

  IMAGE_QUALITY_DEFAULT = IMAGE_QUALITY_BILINEAR,
  IMAGE_QUALITY_COUNT = 5
};

// ============================================================================
// [Fog::LCD_ORDER]
// ============================================================================

//! @brief Order of RGB components of LCD display.
enum LCD_ORDER
{
  //! @brief Order is unknown or the device is not the LCD display.
  LCD_ORDER_NONE = 0,
  //! @brief Horizontal R-G-B order.
  LCD_ORDER_HRGB = 1,
  //! @brief Horizontal B-G-R order.
  LCD_ORDER_HBGR = 2,
  //! @brief Vertical R-G-B order.
  LCD_ORDER_VRGB = 3,
  //! @brief Vertical B-G-R order.
  LCD_ORDER_VBGR = 4,

  //! @brief Count of LCD order options.
  LCD_ORDER_COUNT = 5
};

// ============================================================================
// [Fog::LINE_CAP]
// ============================================================================

//! @brief Line-cap.
enum LINE_CAP
{
  //! @brief Butt line-cap (default).
  LINE_CAP_BUTT = 0,
  //! @brief Square line-cap.
  LINE_CAP_SQUARE = 1,
  //! @brief Round line-cap.
  LINE_CAP_ROUND = 2,
  //! @brief Reversed round line-cap.
  LINE_CAP_ROUND_REVERSE = 3,
  //! @brief Triangle line-cap.
  LINE_CAP_TRIANGLE = 4,
  //! @brief Reversed triangle line-cap.
  LINE_CAP_TRIANGLE_REVERSE = 5,

  //! @brief Default line-cap type.
  LINE_CAP_DEFAULT = LINE_CAP_BUTT,
  //! @brief Used to catch invalid arguments.
  LINE_CAP_COUNT = 6
};

// ============================================================================
// [Fog::LINE_INTERSECTION]
// ============================================================================

//! @brief Line intersection result.
enum LINE_INTERSECTION
{
  LINE_INTERSECTION_NONE = 0,
  LINE_INTERSECTION_BOUNDED = 1,
  LINE_INTERSECTION_UNBOUNDED = 2
};

// ============================================================================
// [Fog::LINE_JOIN]
// ============================================================================

//! @brief Line join.
enum LINE_JOIN
{
  //! @brief Miter line-join (default).
  LINE_JOIN_MITER = 0,
  //! @brief Round line-join.
  LINE_JOIN_ROUND = 1,
  //! @brief Bevel line-join.
  LINE_JOIN_BEVEL = 2,
  LINE_JOIN_MITER_REVERT = 3,
  LINE_JOIN_MITER_ROUND = 4,

  //! @brief Default line-join type.
  LINE_JOIN_DEFAULT = LINE_JOIN_MITER,
  //! @brief Used to catch invalid arguments.
  LINE_JOIN_COUNT = 5
};

// ============================================================================
// [Fog::MASK_OP]
// ============================================================================

enum MASK_OP
{
  //! @brief Copy the source to the destination (SRC).
  MASK_OP_REPLACE = 0,
  //! @brief Intersect the source with the destination (MULTIPLY).
  MASK_OP_INTERSECT = 1,

  //! @brief Count of mask operators.
  MASK_OP_COUNT = 2
};

// ============================================================================
// [Fog::MATRIX_ORDER]
// ============================================================================

//! @brief Matrix multiply ordering.
enum MATRIX_ORDER
{
  //! @brief The second matrix which is multiplied with the primary matrix is
  //! on the left (default for all graphics / color matrix operations).
  MATRIX_ORDER_PREPEND = 0,

  //! @brief The second matrix which is multiplied with the primary matrix is
  //! on the right.
  MATRIX_ORDER_APPEND = 1
};

// ============================================================================
// [Fog::PAINTER_FLUSH]
// ============================================================================

//! @brief Painter flush flags.
enum PAINTER_FLUSH
{
  //! @brief Flush all painter commands and wait for completition. Use this
  //! command if you want to access image data after the @c flush() call.
  PAINTER_FLUSH_SYNC = 0x00000001
};

// ============================================================================
// [Fog::PAINTER_GROUP]
// ============================================================================

//! @brief Painter group flags, used by @ref Painter::beginGroup().
enum PAINTER_GROUP
{
  // --------------------------------------------------------------------------
  // [Format]
  // --------------------------------------------------------------------------

  //! @brief Default group format.
  PAINTER_GROUP_DEFAULT = 0x00000000,

  //! @brief Create Alpha group (can be combined with @ref PAINTER_GROUP_RGB).
  PAINTER_GROUP_ALPHA = 0x00000001,
  //! @brief Create RGB group (can be combined with @ref PAINTER_GROUP_ALPHA).
  PAINTER_GROUP_RGB = 0x00000002,
  //! @brief Create ARGB group (combination of @ref PAINTER_GROUP_ALPHA and 
  //! @ref PAINTER_GROUP_RGB).
  PAINTER_GROUP_ARGB = 0x00000003
};

// ============================================================================
// [Fog::PAINTER_INIT]
// ============================================================================

//! @brief Painter initialization flags.
enum PAINTER_INIT
{
  //! @brief Crear the content of the painter to transparent color if target
  //! buffer contains alpha channel or to black (if target buffer not contains
  //! alpha channel).
  //!
  //! Using this flag can lead to very optimized painting and it's generally
  //! faster than setting painter to @c COMPOSITE_SRC and clearing the content
  //! by using @c Painter::fillAll() or @c Painter::fillRect() methods. This
  //! method is efficient, because the painter can mark an image region as
  //! transparent and then use that hint to perform fast source-copy
  //! compositing on that region, instead of doing regular composition.
  PAINTER_INIT_CLEAR = 0x00000001,

  //! @brief Initialize multithreading if it makes sense.
  //!
  //! If this option is true, painter first check if image size is not too
  //! small (painting to small images are singlethreaded by default). Then
  //! CPU detection is used to check if machine contains more CPUs or cores.
  PAINTER_INIT_MT = 0x00000002
};

// ============================================================================
// [Fog::PAINTER_MAP]
// ============================================================================

//! @brief Painter point options.
enum PAINTER_MAP
{
  //! @brief Map user coordinates to device coordinates.
  PAINTER_MAP_USER_TO_DEVICE = 0,
  //! @brief Map device coordinates to user coordinates.
  PAINTER_MAP_DEVICE_TO_USER = 1,

  //! @brief Count of map operations.
  PAINTER_MAP_COUNT = 2
};

// ============================================================================
// [Fog::PAINT_DEVICE
// ============================================================================

//! @brief Type of the paint-device.
enum PAINT_DEVICE
{
  //! @brief Null paint-device (not initialized or invalid).
  PAINT_DEVICE_NULL = 0,
  //! @brief @ref Image paint-device (raster-based).
  PAINT_DEVICE_IMAGE = 1,

  //! @brief Count of paint-device IDs.
  PAINT_DEVICE_COUNT = 2
};

// ============================================================================
// [Fog::PAINTER_PARAMETER]
// ============================================================================

enum PAINTER_PARAMETER
{
  // --------------------------------------------------------------------------
  // [Engine]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_SIZE_I = 0,
  PAINTER_PARAMETER_SIZE_F = 1,
  PAINTER_PARAMETER_SIZE_D = 2,

  PAINTER_PARAMETER_FORMAT_I = 3,
  PAINTER_PARAMETER_DEVICE_I = 4,

  // --------------------------------------------------------------------------
  // [Multithreading]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_MULTITHREADED_I = 5,
  PAINTER_PARAMETER_MAX_THREADS_I = 6,

  // --------------------------------------------------------------------------
  // [Paint Params]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_PAINT_PARAMS_F = 7,
  PAINTER_PARAMETER_PAINT_PARAMS_D = 8,

  // --------------------------------------------------------------------------
  // [Paint Hints]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_PAINT_HINTS = 9,

  //! @brief Compositing operator.
  PAINTER_PARAMETER_COMPOSITING_OPERATOR_I = 10,

  //! @brief Render quality, see @c RENDER_QUALITY.
  PAINTER_PARAMETER_RENDER_QUALITY_I = 11,

  //! @brief Image interpolation quality, see @c IMAGE_QUALITY.
  PAINTER_PARAMETER_IMAGE_QUALITY_I = 12,

  //! @brief Gradient interpolation quality, see @c GRADIENT_QUALITY.
  PAINTER_PARAMETER_GRADIENT_QUALITY_I = 13,

  //! @brief Whether to render text using path-outlines only.
  PAINTER_PARAMETER_OUTLINED_TEXT_I = 14,

  //! @brief Whether to maximize the performance of rendering lines, possibly
  //! degrading their visual quality.
  //!
  //! @note The quality of rendered lines must be related to antialiasing quality.
  //! This means that using fast-lines flag never turns antialiasing off, only
  //! the mask computation can be simplified.
  PAINTER_PARAMETER_FAST_LINE_I = 15,

  //! @brief Whether to maximize the geometric precision of vector coordinates
  //! and transformations.
  PAINTER_PARAMETER_GEOMETRIC_PRECISION_I = 16,

  // --------------------------------------------------------------------------
  // [Paint Opacity]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_OPACITY_F = 17,
  PAINTER_PARAMETER_OPACITY_D = 18,

  // --------------------------------------------------------------------------
  // [Fill Params]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_FILL_RULE_I = 19,

  // --------------------------------------------------------------------------
  // [Stroke Params]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_STROKE_PARAMS_F = 20,
  PAINTER_PARAMETER_STROKE_PARAMS_D = 21,

  PAINTER_PARAMETER_LINE_WIDTH_F = 22,
  PAINTER_PARAMETER_LINE_WIDTH_D = 23,

  PAINTER_PARAMETER_LINE_JOIN_I = 24,
  PAINTER_PARAMETER_START_CAP_I = 25,
  PAINTER_PARAMETER_END_CAP_I = 26,
  PAINTER_PARAMETER_LINE_CAPS_I = 27,

  PAINTER_PARAMETER_MITER_LIMIT_F = 28,
  PAINTER_PARAMETER_MITER_LIMIT_D = 29,

  PAINTER_PARAMETER_DASH_OFFSET_F = 30,
  PAINTER_PARAMETER_DASH_OFFSET_D = 31,

  PAINTER_PARAMETER_DASH_LIST_F = 32,
  PAINTER_PARAMETER_DASH_LIST_D = 33,

  // --------------------------------------------------------------------------
  // [Filter Params]
  // --------------------------------------------------------------------------

  PAINTER_PARAMETER_FILTER_SCALE_F = 34,
  PAINTER_PARAMETER_FILTER_SCALE_D = 35,

  // --------------------------------------------------------------------------
  // [...]
  // --------------------------------------------------------------------------

  //! @brief Count of painter parameters.
  PAINTER_PARAMETER_COUNT = 36
};

// ============================================================================
// [Fog::PAINTER_SOURCE]
// ============================================================================

//! @brief Painter source parameter.
enum PAINTER_SOURCE
{
  //! @brief Parameter is @c TextureF.
  PAINTER_SOURCE_TEXTURE_F = 0,
  //! @brief Parameter is @c TextureD.
  PAINTER_SOURCE_TEXTURE_D = 1,
  //! @brief Parameter is @c GradientF.
  PAINTER_SOURCE_GRADIENT_F = 2,
  //! @brief Parameter is @c GradientD.
  PAINTER_SOURCE_GRADIENT_D = 3,
  //! @brief Count of painter source parameters.
  PAINTER_SOURCE_COUNT = 4
};

// ============================================================================
// [Fog::PATH_CLIPPER_MEASURE]
// ============================================================================

enum PATH_CLIPPER_MEASURE
{
  //! @brief Source path is bounded to the clip-box (not needed to clip).
  PATH_CLIPPER_MEASURE_BOUNDED = 0,
  //! @brief Source path is not bounded to the clip-box (needed to clip).
  PATH_CLIPPER_MEASURE_UNBOUNDED = 1,
  //! @brief Source path is invalid.
  PATH_CLIPPER_MEASURE_INVALID = 2
};

// ============================================================================
// [Fog::PATH_CMD]
// ============================================================================

//! @brief Path commands.
enum PATH_CMD
{
  //! @brief Move-to command (one vertex).
  PATH_CMD_MOVE_TO = 0,

  //! @brief Line-to command (one vertex).
  PATH_CMD_LINE_TO = 1,

  //! @brief Quad-to command (two vertices), followed by @c PATH_CMD_DATA
  //! command.
  PATH_CMD_QUAD_TO = 2,

  //! @brief Cubic-to command (three vertices), followed by two @c PATH_CMD_DATA
  //! commands.
  PATH_CMD_CUBIC_TO = 3,

  //! @brief Close command.
  PATH_CMD_CLOSE = 4,

  //! @brief Command used to distunguish between the additional data needed for
  //! @c PATH_CMD_QUAD_TO and @c PATH_CMD_CUBIC_TO.
  //!
  //! One @c PATH_CMD_DATA command follows the @c PATH_CMD_QUAD_TO, and two
  //! @c PATH_CMD_DATA commands follow the @c PATH_CMD_CUBIC_TO.
  PATH_CMD_DATA = 5,

  //! @internal
  //!
  //! @brief Internal flag never stored with PATH_CMD, used only when path is
  //! processed or iterated.
  _PATH_CMD_HAS_INITIAL = 0x8
};

// ============================================================================
// [Fog::PATH_DIRECTION]
// ============================================================================

//! @brief Sub-path direction.
enum PATH_DIRECTION
{
  //! @brief No direction specified.
  PATH_DIRECTION_NONE = 0,
  //! @brief Clockwise direction.
  PATH_DIRECTION_CW = 1,
  //! @brief Counter-clockwise direction.
  PATH_DIRECTION_CCW = 2
};

// ============================================================================
// [Fog::PATH_FLAG]
// ============================================================================

//! @brief @ref PathF and @ref PathF flags.
enum PATH_FLAG
{
  //! @brief Path boudning-box is dirty (needs to be updated).
  PATH_FLAG_DIRTY_BBOX  = VAR_FLAG_RESERVED_1,
  //! @brief Path command flags are dirty (needs to be updated).
  PATH_FLAG_DIRTY_CMD   = VAR_FLAG_RESERVED_2,
  //! @brief Path info structure is dirty (needs to be updated).
  PATH_FLAG_DIRTY_INFO  = VAR_FLAG_RESERVED_3,

  //! @brief Path contains bounding-box.
  PATH_FLAG_HAS_BBOX    = VAR_FLAG_RESERVED_4,
  //! @brief Path contains quadratic Bezier curves.
  PATH_FLAG_HAS_QBEZIER = VAR_FLAG_RESERVED_5,
  //! @brief Path contains cubic Bezier curves.
  PATH_FLAG_HAS_CBEZIER = VAR_FLAG_RESERVED_6,

  //! @brief Mask of all @ref PathF and @ref PathD specific flags.
  PATH_FLAG_MASK        = PATH_FLAG_DIRTY_BBOX  |
                          PATH_FLAG_DIRTY_CMD   |
                          PATH_FLAG_DIRTY_INFO  |
                          PATH_FLAG_HAS_BBOX    |
                          PATH_FLAG_HAS_QBEZIER |
                          PATH_FLAG_HAS_CBEZIER
};

// ============================================================================
// [Fog::PATH_FLATTEN]
// ============================================================================

//! @brief Path flattening option.
enum PATH_FLATTEN
{
  //! @brief Flattening is disabled.
  PATH_FLATTEN_DISABLED = 0,

  //! @brief Flatten all cubic Bezier curves to quadratic Bezier curves.
  //!
  //! Resulting path may contain lines or quadratic Bezier curves. This option
  //! is designed for backends which only support quadratic bezier curves, so
  //! all cubics are flattened by Stroker/Clipper.
  PATH_FLATTEN_QBEZIER = 1,

  //! @brief Flatten all cubic and quadratic Bezier curves to lines.
  //!
  //! Resulting path may contain only lines. This option is usable for
  //! backends which support only line-segments which can form a triangles,
  //! trapezoids, or polygons.
  PATH_FLATTEN_LINE = 2,

  //! @brief Count of path flattening options.
  PATH_FLATTEN_COUNT = 3
};

// ============================================================================
// [Fog::PATTERN_TYPE]
// ============================================================================

//! @brief Type of @c Pattern.
enum PATTERN_TYPE
{
  //! @brief Null pattern (nothing will be paint using this pattern).
  PATTERN_TYPE_NULL = 0,
  //! @brief Solid color pattern.
  PATTERN_TYPE_COLOR = 1,
  //! @brief Texture pattern (@c Texture).
  PATTERN_TYPE_TEXTURE = 2,
  //! @brief Gradient pattern (@c GradientF or @c GradientD).
  PATTERN_TYPE_GRADIENT = 3,

  PATTERN_TYPE_MASK = 0x0F,
};

// ============================================================================
// [Fog::PATTERN_PRECISION]
// ============================================================================

//! @brief Precision of @c Pattern.
enum PATTERN_PRECISION
{
  PATTERN_PRECISION_F = 0x10,
  PATTERN_PRECISION_D = 0x20,
  PATTERN_PRECISION_MASK = 0xF0
};

// ============================================================================
// [Fog::PIXEL_ARGB32_POS]
// ============================================================================

enum PIXEL_ARGB32_POS
{
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
  PIXEL_ARGB32_POS_A = 3,
  PIXEL_ARGB32_POS_R = 2,
  PIXEL_ARGB32_POS_G = 1,
  PIXEL_ARGB32_POS_B = 0
#else // FOG_BYTE_ORDER == FOG_BIG_ENDIAN
  PIXEL_ARGB32_POS_A = 0,
  PIXEL_ARGB32_POS_R = 1,
  PIXEL_ARGB32_POS_G = 2,
  PIXEL_ARGB32_POS_B = 3
#endif // FOG_BYTE_ORDER
};

// ============================================================================
// [Fog::PIXEL_ARGB32_MASK]
// ============================================================================

static const uint32_t PIXEL_ARGB32_MASK_A = 0xFF000000U;
static const uint32_t PIXEL_ARGB32_MASK_R = 0x00FF0000U;
static const uint32_t PIXEL_ARGB32_MASK_G = 0x0000FF00U;
static const uint32_t PIXEL_ARGB32_MASK_B = 0x000000FFU;

// ============================================================================
// [Fog::PIXEL_ARGB32_SHIFT]
// ============================================================================

enum PIXEL_ARGB32_SHIFT
{
  PIXEL_ARGB32_SHIFT_A = 24U,
  PIXEL_ARGB32_SHIFT_R = 16U,
  PIXEL_ARGB32_SHIFT_G =  8U,
  PIXEL_ARGB32_SHIFT_B =  0U
};

// ============================================================================
// [Fog::PIXEL_ARGB64_POS]
// ============================================================================

enum PIXEL_ARGB64_POS
{
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
  PIXEL_ARGB64_WORD_A = 3,
  PIXEL_ARGB64_WORD_R = 2,
  PIXEL_ARGB64_WORD_G = 1,
  PIXEL_ARGB64_WORD_B = 0,

  PIXEL_ARGB64_BYTE_A    = 6,
  PIXEL_ARGB64_BYTE_A_HI = 7,
  PIXEL_ARGB64_BYTE_A_LO = 6,

  PIXEL_ARGB64_BYTE_R    = 4,
  PIXEL_ARGB64_BYTE_R_HI = 5,
  PIXEL_ARGB64_BYTE_R_LO = 4,

  PIXEL_ARGB64_BYTE_G    = 2,
  PIXEL_ARGB64_BYTE_G_HI = 3,
  PIXEL_ARGB64_BYTE_G_LO = 2,

  PIXEL_ARGB64_BYTE_B    = 0,
  PIXEL_ARGB64_BYTE_B_HI = 1,
  PIXEL_ARGB64_BYTE_B_LO = 0
#else // FOG_BYTE_ORDER == FOG_BIG_ENDIAN
  PIXEL_ARGB64_WORD_A = 0,
  PIXEL_ARGB64_WORD_R = 1,
  PIXEL_ARGB64_WORD_G = 2,
  PIXEL_ARGB64_WORD_B = 3,

  PIXEL_ARGB64_BYTE_A = 0,
  PIXEL_ARGB64_BYTE_A_HI = 0,
  PIXEL_ARGB64_BYTE_A_LO = 1,

  PIXEL_ARGB64_BYTE_R = 2,
  PIXEL_ARGB64_BYTE_R_HI = 2,
  PIXEL_ARGB64_BYTE_R_LO = 3,

  PIXEL_ARGB64_BYTE_G = 4,
  PIXEL_ARGB64_BYTE_G_HI = 4,
  PIXEL_ARGB64_BYTE_G_LO = 5,

  PIXEL_ARGB64_BYTE_B = 6,
  PIXEL_ARGB64_BYTE_B_HI = 6,
  PIXEL_ARGB64_BYTE_B_LO = 7
#endif // FOG_BYTE_ORDER
};

// ============================================================================
// [Fog::PIXEL_ARGB64_MASK]
// ============================================================================

static const uint64_t PIXEL_ARGB64_MASK_A = FOG_UINT64_C(0xFFFF000000000000);
static const uint64_t PIXEL_ARGB64_MASK_R = FOG_UINT64_C(0x0000FFFF00000000);
static const uint64_t PIXEL_ARGB64_MASK_G = FOG_UINT64_C(0x00000000FFFF0000);
static const uint64_t PIXEL_ARGB64_MASK_B = FOG_UINT64_C(0x000000000000FFFF);

// ============================================================================
// [Fog::PIXEL_ARGB64_SHIFT]
// ============================================================================

enum PIXEL_ARGB64_SHIFT
{
  PIXEL_ARGB64_SHIFT_A = 48U,
  PIXEL_ARGB64_SHIFT_R = 32U,
  PIXEL_ARGB64_SHIFT_G = 16U,
  PIXEL_ARGB64_SHIFT_B =  0U
};

// ============================================================================
// [Fog::PIXEL_RGB48_POS]
// ============================================================================

enum PIXEL_RGB48_POS
{
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
  PIXEL_RGB48_WORD_R = 2,
  PIXEL_RGB48_WORD_G = 1,
  PIXEL_RGB48_WORD_B = 0,

  PIXEL_RGB48_BYTE_R    = 4,
  PIXEL_RGB48_BYTE_R_HI = 5,
  PIXEL_RGB48_BYTE_R_LO = 4,

  PIXEL_RGB48_BYTE_G    = 2,
  PIXEL_RGB48_BYTE_G_HI = 3,
  PIXEL_RGB48_BYTE_G_LO = 2,

  PIXEL_RGB48_BYTE_B    = 0,
  PIXEL_RGB48_BYTE_B_HI = 1,
  PIXEL_RGB48_BYTE_B_LO = 0
#else // FOG_BYTE_ORDER == FOG_BIG_ENDIAN
  PIXEL_RGB48_WORD_R = 0,
  PIXEL_RGB48_WORD_G = 1,
  PIXEL_RGB48_WORD_B = 2,

  PIXEL_RGB48_BYTE_R = 0,
  PIXEL_RGB48_BYTE_R_HI = 0,
  PIXEL_RGB48_BYTE_R_LO = 1,

  PIXEL_RGB48_BYTE_G = 2,
  PIXEL_RGB48_BYTE_G_HI = 2,
  PIXEL_RGB48_BYTE_G_LO = 3,

  PIXEL_RGB48_BYTE_B = 4,
  PIXEL_RGB48_BYTE_B_HI = 4,
  PIXEL_RGB48_BYTE_B_LO = 5
#endif // FOG_BYTE_ORDER
};

// ============================================================================
// [Fog::PIXEL_A16]
// ============================================================================

enum PIXEL_A16_POS
{
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
  PIXEL_A16_BYTE_HI = 1,
  PIXEL_A16_BYTE_LO = 0
#else // FOG_BYTE_ORDER == FOG_BIG_ENDIAN
  PIXEL_A16_BYTE_HI = 0,
  PIXEL_A16_BYTE_LO = 1
#endif // FOG_BYTE_ORDER
};

// ============================================================================
// [Fog::PIXEL_RGB24_POS]
// ============================================================================

enum PIXEL_RGB24_POS
{
#if FOG_BYTE_ORDER == FOG_LITTLE_ENDIAN
  PIXEL_RGB24_BYTE_R = 2,
  PIXEL_RGB24_BYTE_G = 1,
  PIXEL_RGB24_BYTE_B = 0
#else // FOG_BYTE_ORDER == FOG_BIG_ENDIAN
  PIXEL_RGB24_BYTE_R = 0,
  PIXEL_RGB24_BYTE_G = 1,
  PIXEL_RGB24_BYTE_B = 2
#endif // FOG_BYTE_ORDER
};

// ============================================================================
// [Fog::REGION_HIT_TEST]
// ============================================================================

//! @brief Region hit-test result.
enum REGION_HIT_TEST
{
  //! @brief Object isn't in region (point, rectangle or another region).
  REGION_HIT_OUT = 0,
  //! @brief Object is in region (point, rectangle or another region).
  REGION_HIT_IN = 1,
  //! @brief Object is partially in region (point, rectangle or another region).
  REGION_HIT_PART = 2
};

// ============================================================================
// [Fog::REGION_OP]
// ============================================================================

//! @brief Region combining operators.
enum REGION_OP
{
  //! @brief Replacement (B).
  REGION_OP_REPLACE = 0,
  //! @brief Intersection (A & B).
  REGION_OP_INTERSECT = 1,
  //! @brief Union (A | B).
  REGION_OP_UNION = 2,
  //! @brief Xor (A ^ B).
  REGION_OP_XOR = 3,
  //! @brief Subtraction (A - B).
  REGION_OP_SUBTRACT = 4,
  //! @brief Reverse subtraction (B - A).
  REGION_OP_SUBTRACT_REV = 5,

  //! @brief Count of region operators.
  REGION_OP_COUNT = 6
};

// ============================================================================
// [Fog::REGION_TYPE]
// ============================================================================

//! @brief Type of @c Region.
enum REGION_TYPE
{
  // NOTE: Never change value of @c REGION_TYPE_EMPTY and @c REGION_TYPE_RECT
  // constants - see Fog::Region::getType() method in Fog/G2d/Tools/Region.cpp.

  //! @brief Region is empty.
  REGION_TYPE_EMPTY = 0,
  //! @brief Region has only one rectangle (rectangular).
  REGION_TYPE_RECT = 1,
  //! @brief Region has more YX sorted rectangles.
  REGION_TYPE_COMPLEX = 2,
  //! @brief Region is infinite (special region type).
  REGION_TYPE_INFINITE = 3
};

// ============================================================================
// [Fog::RENDER_QUALITY]
// ============================================================================

//! @brief Render quality.
enum RENDER_QUALITY
{
  //! @brief Aliased (disabled antialiasing).
  RENDER_QUALITY_ALIASED = 0,

  //! @brief Use at least 4 shades of grey for antialiasing.
  //!
  //! This is only recommendation for paint-engine. It can use higher render
  //! quality if this option is not supported.
  RENDER_QUALITY_GREY_4 = 1,

  //! @brief Use at least 8 shades of grey for antialiasing.
  //!
  //! This is only recommendation for paint-engine. It can use higher or lower
  //! render quality if this option is not supported.
  RENDER_QUALITY_GREY_8 = 2,

  //! @brief Use at least 16 shades of grey for antialiasing.
  //!
  //! This is only recommendation for paint-engine. It can use higher or lower
  //! render quality if this option is not supported.
  RENDER_QUALITY_GREY_16 = 3,

  //! @brief Use at least 32 shades of grey for antialiasing.
  //!
  //! This is only recommendation for paint-engine. It can use higher or lower
  //! render quality if this option is not supported.
  RENDER_QUALITY_GREY_32 = 4,

  //! @brief Use at least 64 shades of grey for antialiasing.
  //!
  //! This is only recommendation for paint-engine. It can use higher or lower
  //! render quality if this option is not supported.
  RENDER_QUALITY_GREY_64 = 5,

  //! @brief Use at least 256 shades of grey for antialiasing when rendering
  //! for 8-bit target and 65536 shades of grey when rendering for 16-bit
  //! target.
  //!
  //! This is only recommendation for paint-engine. It can use higher or lower
  //! render quality if this option is not supported.
  RENDER_QUALITY_GREY_HQ = 6,

  //! @brief LCD subpixel antialiasing.
  //!
  //! This type of antialiasing is usually only implemented for font rendering.
  RENDER_QUALITY_LCD = 7,

  //! @brief Count of render quality settings (for error checking).
  RENDER_QUALITY_COUNT = 8,

  //! @brief Default rendering quality (synonym to @c RENDER_QUALITY_GREY_16).
  RENDER_QUALITY_DEFAULT = RENDER_QUALITY_GREY_16
};

// ============================================================================
// [Fog::SHAPE_TYPE]
// ============================================================================

enum SHAPE_TYPE
{
  SHAPE_TYPE_NONE = 0,

  // --------------------------------------------------------------------------
  // [Unclosed]
  // --------------------------------------------------------------------------

  SHAPE_TYPE_LINE = 1,
  SHAPE_TYPE_QBEZIER = 2,
  SHAPE_TYPE_CBEZIER = 3,
  SHAPE_TYPE_ARC = 4,

  // --------------------------------------------------------------------------
  // [Closed]
  // --------------------------------------------------------------------------

  SHAPE_TYPE_RECT = 5,
  SHAPE_TYPE_ROUND = 6,
  SHAPE_TYPE_CIRCLE = 7,
  SHAPE_TYPE_ELLIPSE = 8,
  SHAPE_TYPE_CHORD = 9,
  SHAPE_TYPE_PIE = 10,
  SHAPE_TYPE_TRIANGLE = 11,

  SHAPE_TYPE_POLYLINE = 12,
  SHAPE_TYPE_POLYGON = 13,
  SHAPE_TYPE_RECT_ARRAY = 14,

  SHAPE_TYPE_PATH = 15,

  // --------------------------------------------------------------------------
  // [Count]
  // --------------------------------------------------------------------------

  SHAPE_TYPE_COUNT = 16
};

// ============================================================================
// [Fog::SVG_ELEMENT]
// ============================================================================

enum SVG_ELEMENT
{
  SVG_ELEMENT_NONE = 0,

  SVG_ELEMENT_A,
  SVG_ELEMENT_CIRCLE,
  SVG_ELEMENT_CLIP_PATH,
  SVG_ELEMENT_DEFS,
  SVG_ELEMENT_DESC,
  SVG_ELEMENT_ELLIPSE,
  SVG_ELEMENT_G,
  SVG_ELEMENT_IMAGE,
  SVG_ELEMENT_LINE,
  SVG_ELEMENT_LINEAR_GRADIENT,
  SVG_ELEMENT_MARKER,
  SVG_ELEMENT_PATH,
  SVG_ELEMENT_PATTERN,
  SVG_ELEMENT_POLYGON,
  SVG_ELEMENT_POLYLINE,
  SVG_ELEMENT_RADIAL_GRADIENT,
  SVG_ELEMENT_RECT,
  SVG_ELEMENT_SOLID_COLOR,
  SVG_ELEMENT_STOP,
  SVG_ELEMENT_SVG,
  SVG_ELEMENT_SYMBOL,
  SVG_ELEMENT_TEXT,
  SVG_ELEMENT_TEXT_PATH,
  SVG_ELEMENT_TITLE,
  SVG_ELEMENT_TREF,
  SVG_ELEMENT_TSPAN,
  SVG_ELEMENT_USE,
  SVG_ELEMENT_VIEW
};

// ============================================================================
// [Fog::SVG_PAINT]
// ============================================================================

enum SVG_PAINT
{
  SVG_PAINT_FILL = 0,
  SVG_PAINT_STROKE = 1
};

// ============================================================================
// [Fog::SVG_STYLE]
// ============================================================================

enum SVG_STYLE
{
  //! @note Part of SVG and CSS2 - Font properties.
  SVG_STYLE_FONT = 0,
  //! @note Part of SVG and CSS2 - Font properties.
  SVG_STYLE_FONT_FAMILY,
  //! @note Part of SVG and CSS2 - Font properties.
  SVG_STYLE_FONT_SIZE,

  //! @note Part of SVG and CSS2 - Text properties.
  SVG_STYLE_DIRECTION,
  //! @note Part of SVG and CSS2 - Text properties.
  SVG_STYLE_LETTER_SPACING,
  //! @note Part of SVG and CSS2 - Text properties.
  SVG_STYLE_TEXT_DECORATION,
  //! @note Part of SVG and CSS2 - Text properties.
  SVG_STYLE_WORD_SPACING,

  //! @note Part of SVG and CSS2 - Other properties for visual media.
  SVG_STYLE_COLOR,

  //! @note Part of SVG - Clipping, Masking, and Compositing properties.
  SVG_STYLE_CLIP_PATH,
  //! @note Part of SVG - Clipping, Masking, and Compositing properties.
  SVG_STYLE_CLIP_RULE,
  //! @note Part of SVG - Clipping, Masking, and Compositing properties.
  SVG_STYLE_MASK,
  //! @note Part of SVG - Clipping, Masking, and Compositing properties.
  SVG_STYLE_OPACITY,

  //! @note Part of SVG - Filter Effects properties.
  SVG_STYLE_ENABLE_BACKGROUND,
  //! @note Part of SVG - Filter Effects properties.
  SVG_STYLE_FILTER,
  //! @note Part of SVG - Filter Effects properties.
  SVG_STYLE_FLOOD_COLOR,
  //! @note Part of SVG - Filter Effects properties.
  SVG_STYLE_FLOOD_OPACITY,
  //! @note Part of SVG - Filter Effects properties.
  SVG_STYLE_LIGHTING_COLOR,

  //! @note Part of SVG - Gradient properties.
  SVG_STYLE_STOP_COLOR,
  //! @note Part of SVG - Gradient properties.
  SVG_STYLE_STOP_OPACITY,

  //! @note Part of SVG - Color and Painting properties.
  //!
  //! @note Introduced in SVG 1.2 specification.
  SVG_STYLE_COMP_OP,

  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_FILL,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_FILL_OPACITY,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_FILL_RULE,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_IMAGE_RENDERING,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_MARKER,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_MARKER_END,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_MARKER_MID,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_MARKER_START,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_SHAPE_RENDERING,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_DASH_ARRAY,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_DASH_OFFSET,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_LINE_CAP,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_LINE_JOIN,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_MITER_LIMIT,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_OPACITY,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_STROKE_WIDTH,
  //! @note Part of SVG - Color and Painting properties.
  SVG_STYLE_TEXT_RENDERING,

  //! @brief Count of style properties.
  SVG_STYLE_COUNT
};

// ============================================================================
// [Fog::SVG_SOURCE]
// ============================================================================

enum SVG_SOURCE
{
  SVG_SOURCE_NONE = 0,
  SVG_SOURCE_COLOR = 1,
  SVG_SOURCE_URI = 2,

  // TODO: SVG - currentColor
  // SVG_SOURCE_CURRENT_COLOR = 3,

  SVG_SOURCE_INVALID
};

// ============================================================================
// [Fog::SVG_PATTERN_UNITS]
// ============================================================================

enum SVG_PATTERN_UNITS
{
  SVG_OBJECT_BOUNDING_BOX = 0,
  SVG_USER_SPACE_ON_USE = 1,

  SVG_PATTERN_UNITS_COUNT = 2
};

// ============================================================================
// [Fog::SVG_LENGTH_ADJUST]
// ============================================================================

enum SVG_LENGTH_ADJUST
{
  SVG_LENGTH_ADJUST_SPACING = 0,
  SVG_LENGTH_ADJUST_SPACING_AND_GLYPHS = 1
};

// ============================================================================
// [Fog::SVG_CONTEXT]
// ============================================================================

//! @brief Type of SVG context.
enum SVG_CONTEXT
{
  //! @brief None, private use only.
  SVG_CONTEXT_NONE = 0,

  //! @brief SVG context is @ref SvgRenderContext.
  SVG_CONTEXT_RENDER = 1,
  //! @brief SVG context is @ref SvgMeasureContext.
  SVG_CONTEXT_MEASURE = 2,
  //! @brief SVG context is @ref SvgHitTestContext.
  SVG_CONTEXT_HIT_TEST = 3
};

// ============================================================================
// [Fog::SVG_MISC]
// ============================================================================

enum SVG_MISC
{
  //! @brief Inherit.
  SVG_INHERIT = 0xFF
};

// ============================================================================
// [Fog::TEXT_ALIGN]
// ============================================================================

//! @brief Text alignment
enum TEXT_ALIGN
{
  TEXT_ALIGN_LEFT = 0x01,
  TEXT_ALIGN_RIGHT = 0x02,
  TEXT_ALIGN_HCENTER = 0x03,
  TEXT_ALIGN_HMASK = 0x03,

  TEXT_ALIGN_TOP = 0x10,
  TEXT_ALIGN_BOTTOM = 0x20,
  TEXT_ALIGN_VCENTER = 0x30,
  TEXT_ALIGN_VMASK = 0x30,

  TEXT_ALIGN_CENTER = TEXT_ALIGN_VCENTER | TEXT_ALIGN_HCENTER
};

// ============================================================================
// [Fog::TEXT_OBJECT]
// ============================================================================

//! @brief @ref TextObj type.
enum TEXT_OBJECT
{
  //! @brief Not used.
  TEXT_OBJECT_NONE = 0,
  //! @brief @ref TextSpan node.
  TEXT_OBJECT_SPAN = 1,
  //! @brief @ref TextDocument node.
  TEXT_OBJECT_DOCUMENT = 2,
  //! @brief @ref TextParagraph node.
  TEXT_OBJECT_PARAGRAPH = 3
};

// ============================================================================
// [Fog::TEXT_FLAG]
// ============================================================================

//! @brief Flags which applies to @ref TextObj.
enum TEXT_FLAG
{
  TEXT_FLAG_DIRTY = 0x0001,
  TEXT_FLAG_DIRTY_CHILDREN = 0x0002
};

// ============================================================================
// [Fog::TEXTURE_TILE]
// ============================================================================

//! @brief Texture tiling mode (see @c Texture).
enum TEXTURE_TILE
{
  TEXTURE_TILE_PAD = 0,
  TEXTURE_TILE_REPEAT = 1,
  TEXTURE_TILE_REFLECT = 2,
  TEXTURE_TILE_CLAMP = 3,

  TEXTURE_TILE_DEFAULT = TEXTURE_TILE_REPEAT,
  TEXTURE_TILE_COUNT = 4
};

// ============================================================================
// [Fog::TRANSFORM_CREATE]
// ============================================================================

//! @brief Type of matrix to create.
enum TRANSFORM_CREATE
{
  TRANSFORM_CREATE_IDENTITY = 0,
  TRANSFORM_CREATE_TRANSLATION = 1,
  TRANSFORM_CREATE_SCALING = 2,
  TRANSFORM_CREATE_ROTATION = 3,
  TRANSFORM_CREATE_SKEWING = 4,
  TRANSFORM_CREATE_LINE_SEGMENT = 5,
  TRANSFORM_CREATE_REFLECTION_U = 6,
  TRANSFORM_CREATE_REFLECTION_XY = 7,
  TRANSFORM_CREATE_REFLECTION_UNIT = 8,
  TRANSFORM_CREATE_PARALLELOGRAM = 9,
  TRANSFORM_CREATE_QUAD_TO_QUAD = 10,

  TRANSFORM_CREATE_COUNT = 11
};

// ============================================================================
// [Fog::TRANSFORM_OP]
// ============================================================================

//! @brief Type of transform operation.
enum TRANSFORM_OP
{
  //! @brief Translate matrix using @ref PointF.
  TRANSFORM_OP_TRANSLATEF = 0,
  //! @brief Translate matrix using @ref PointD.
  TRANSFORM_OP_TRANSLATED = 1,
  //! @brief Scale matrix using @ref PointF.
  TRANSFORM_OP_SCALEF = 2,
  //! @brief Scale matrix using @ref PointD.
  TRANSFORM_OP_SCALED = 3,
  //! @brief Skew matrix using @ref PointF.
  TRANSFORM_OP_SKEWF = 4,
  //! @brief Skew matrix using @ref PointD.
  TRANSFORM_OP_SKEWD = 5,
  //! @brief Rotate matrix using SP-FP angle.
  TRANSFORM_OP_ROTATEF = 6,
  //! @brief Rotate matrix using DP-FP angle.
  TRANSFORM_OP_ROTATED = 7,
  //! @brief Rotate matrix (about a @ref PointF).
  TRANSFORM_OP_ROTATE_POINTF = 8,
  //! @brief Rotate matrix (about a @ref PointD).
  TRANSFORM_OP_ROTATE_POINTD = 9,
  //! @brief Multiply with @ref TransformF.
  TRANSFORM_OP_MULTIPLYF = 10,
  //! @brief Multiply with @ref TransformD.
  TRANSFORM_OP_MULTIPLYD = 11,
  //! @brief Multiply with inverted @ref TransformF.
  TRANSFORM_OP_MULTIPLY_INVF = 12,
  //! @brief Multiply with inverted @ref TransformD.
  TRANSFORM_OP_MULTIPLY_INVD = 13,
  //! @brief Flip matrix.
  TRANSFORM_OP_FLIP = 14,
  //! @brief Reserved, currently implemented as NOP.
  TRANSFORM_OP_RESERVED = 15,

  //! @brief Count of transform operations.
  TRANSFORM_OP_COUNT = 16
};

// ============================================================================
// [Fog::TRANSFORM_TYPE]
// ============================================================================

//! @brief Type of transform.
enum TRANSFORM_TYPE
{
  //! @brief Transform type is identity.
  TRANSFORM_TYPE_IDENTITY = 0,
  //! @brief Transform type is translation (_20, _21 elements are used).
  TRANSFORM_TYPE_TRANSLATION = 1,
  //! @brief Transform type is scaling (_00, _11, _20, _21 elements are used).
  TRANSFORM_TYPE_SCALING = 2,
  //! @brief Transform type is swap (_01, _10, _20, _21 elements are used).
  TRANSFORM_TYPE_SWAP = 3,
  //! @brief Transform type is rotation (affine part is used).
  TRANSFORM_TYPE_ROTATION = 4,
  //! @brief Transform type is affine.
  TRANSFORM_TYPE_AFFINE = 5,
  //! @brief Transform type is projection.
  TRANSFORM_TYPE_PROJECTION = 6,
  //! @brief Transform type is degenerate (same as projection, but degenerated).
  TRANSFORM_TYPE_DEGENERATE = 7,

  //! @brief Count of transform types (for asserts, ...).
  TRANSFORM_TYPE_COUNT = 8,

  //! @brief Matrix is dirty.
  TRANSFORM_TYPE_DIRTY = 0x8
};

// ============================================================================
// [Fog::UNIT]
// ============================================================================

//! @brief Coordinate units that can be used by the @c Dpi and @c Font classes.
//!
//! Coordinate units can be used to create display independent graphics, keeping
//! the coordinates in device independent units and translating them into
//! device pixels by Fog/G2d engine.
enum UNIT
{
  //! @brief No unit (compatible with @c UNIT_PX).
  UNIT_NONE = 0,
  //! @brief Pixel coordinate(s).
  UNIT_PX,

  //! @brief Point coordinate(s), 1 [pt] == 1/72 [in].
  UNIT_PT,
  //! @brief Pica coordinate(s), 1 [pc] == 12 [pt].
  UNIT_PC,

  //! @brief Inch coordinate(s), 1 [in] == 2.54 [cm].
  UNIT_IN,

  //! @brief Millimeter coordinate(s).
  UNIT_MM,
  //! @brief Centimeter coordinate(s).
  UNIT_CM,

  //! @brief Used for coordinates which depend on the object bounding box.
  UNIT_PERCENTAGE,

  //! @brief The font-size of the relevant font (see @c Font).
  UNIT_EM,
  //! @brief The x-height of the relevant font (see @c Font).
  UNIT_EX,

  //! @brief Count of coord units.
  UNIT_COUNT
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_CORE_GLOBAL_ENUMG2D_H
