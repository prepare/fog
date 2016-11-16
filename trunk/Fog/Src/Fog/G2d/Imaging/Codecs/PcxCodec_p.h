// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_IMAGING_CODECS_PCXCODEC_P_H
#define _FOG_G2D_IMAGING_CODECS_PCXCODEC_P_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Tools/String.h>
#include <Fog/G2d/Imaging/Image.h>
#include <Fog/G2d/Imaging/ImageCodec.h>
#include <Fog/G2d/Imaging/ImageCodecProvider.h>
#include <Fog/G2d/Imaging/ImageDecoder.h>
#include <Fog/G2d/Imaging/ImageEncoder.h>

namespace Fog {

//! @addtogroup Fog_G2d_Imaging
//! @{

// ============================================================================
// [Fog::PcxHeader]
// ============================================================================

#include <Fog/Core/C++/PackByte.h>

//! @internal
struct PcxHeader
{
  uint8_t manufacturer;
  uint8_t version;
  uint8_t encoding;
  uint8_t bitsPerPixel;
  int16_t xMin;
  int16_t yMin;
  int16_t xMax;
  int16_t yMax;
  int16_t horizontalDPI;
  int16_t verticalDPI;
  uint8_t colorMap[48];
  uint8_t reserved;
  uint8_t nPlanes;
  int16_t bytesPerLine;
  int16_t paletteInfo;
  int16_t hScreenSize;
  int16_t vScreenSize;
  uint8_t unused[54];
};

#include <Fog/Core/C++/PackRestore.h>

// ============================================================================
// [Fog::PcxCodecProvider]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT PcxCodecProvider : public ImageCodecProvider
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  PcxCodecProvider();
  virtual ~PcxCodecProvider();

  // --------------------------------------------------------------------------
  // [Implementation]
  // --------------------------------------------------------------------------

  virtual uint32_t checkSignature(const void* mem, size_t length) const;
  virtual err_t createCodec(uint32_t codecType, ImageCodec** codec) const;
};

// ============================================================================
// [Fog::PcxDecoder]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT PcxDecoder : public ImageDecoder
{
  FOG_DECLARE_OBJECT(PcxDecoder, ImageDecoder)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  PcxDecoder(ImageCodecProvider* provider);
  virtual ~PcxDecoder();

  // --------------------------------------------------------------------------
  // [Implementation]
  // --------------------------------------------------------------------------

  virtual void reset();
  virtual err_t readHeader();
  virtual err_t readImage(Image& image);

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE const PcxHeader& pcxFileHeader() const { return _pcxFileHeader; }

  // --------------------------------------------------------------------------
  // [Helpers]
  // --------------------------------------------------------------------------

  // Clear everything.
  FOG_INLINE void zeroall() { MemOps::zero(&_pcxFileHeader, sizeof(_pcxFileHeader)); }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  PcxHeader _pcxFileHeader;
};

// ============================================================================
// [Fog::PcxEncoder]
// ============================================================================

//! @internal
struct FOG_NO_EXPORT PcxEncoder : public ImageEncoder
{
  FOG_DECLARE_OBJECT(PcxEncoder, ImageEncoder)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  PcxEncoder(ImageCodecProvider* provider);
  virtual ~PcxEncoder();

  // --------------------------------------------------------------------------
  // [Implementation]
  // --------------------------------------------------------------------------

  virtual err_t writeImage(const Image& image);
};

//! @}

} // Fog namespace

// ============================================================================
// [Fog::TypeInfo<>]
// ============================================================================

_FOG_TYPE_DECLARE(Fog::PcxHeader, Fog::TYPE_CATEGORY_SIMPLE)

// [Guard]
#endif // _FOG_G2D_IMAGING_CODECS_PCXCODEC_P_H
