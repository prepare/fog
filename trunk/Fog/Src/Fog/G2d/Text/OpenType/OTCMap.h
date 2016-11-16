// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_TEXT_OPENTYPE_OTCMAP_H
#define _FOG_G2D_TEXT_OPENTYPE_OTCMAP_H

// [Dependencies]
#include <Fog/G2d/Text/OpenType/OTApi.h>
#include <Fog/G2d/Text/OpenType/OTTypes.h>

namespace Fog {

// [Byte-Pack]
#include <Fog/Core/C++/PackByte.h>

//! @addtogroup Fog_G2d_Text_OpenType
//! @{

// ============================================================================
// [Fog::OTCMap0]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 0.
// UInt16       | 2            | length               | Table length in bytes.
// UInt16       | 4            | macLanguageCode      | Mac language code.
// UInt8[256]   | 6            | glyphIdArray         | Glyph index array (256).

//! @brief CMap format 0 header.
struct FOG_NO_EXPORT OTCMapFormat0Header
{
  //! @brief Format number is set to 0.
  OTUInt16 format;
  //! @brief Table length in bytes.
  OTUInt16 length;
  //! @brief Mac language code.
  OTUInt16 macLanguageCode;
  //! @brief glyph index array.
  OTUInt8 glyphIdArray[1];
};

// ============================================================================
// [Fog::OTCMap2]
// ============================================================================

// ============================================================================
// [Fog::OTCMap4]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 4.
// UInt16       | 2            | length               | Table length in bytes.
// UInt16       | 4            | macLanguageCode      | Mac language code.
// UInt16       | 6            | numSegX2 (N * 2)     | Number of segments * 2.
// UInt16       | 8            | searchRange          | (2^floor(log2(N))) * 2   == 2*1(LogSegs).
// UInt16       | 10           | entrySelector        | log2(searchRange / 2)    == LogSegs.
// UInt16       | 12           | rangeShift           | N*2 - searchRange.
// UInt16[N]    | 14           | endChar              | End characterCode for each segment, last=0xFFFF.
// UInt16       | 14+N*2       | reservedPad          | Padding, set to 0.
// UInt16[N]    | 16+N*2       | startChar            | Start character code for each segment.
// UInt16[N]    | 16+N*4       | delta                | Delta for all character codes in segment.
// UInt16[N]    | 16+N*6       | offset               | Offsets into glyphIdArray or 0.
// UInt16[]     | 16+N*8       | glyphIdArray         | Glyph index array (arbitrary length).

//! @brief CMap format 4 header.
struct FOG_NO_EXPORT OTCMapFormat4Header
{
  OTUInt16 format;
  OTUInt16 length;
  OTUInt16 macLanguageCode;
  OTUInt16 numSegX2;
  OTUInt16 searchRange;
  OTUInt16 entrySelector;
  OTUInt16 rangeShift;
};

// ============================================================================
// [Fog::OTCMap6]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 6.
// UInt16       | 2            | length               | Table length in bytes.
// UInt16       | 4            | macLanguageCode      | Mac language code.
// UInt16       | 6            | first                | First segment code.
// UInt16       | 8            | count (N)            | Segment size in bytes.
// UInt16[N]    | 10           | glyphIdArray         | Glyph index array.

//! @brief CMap format 6 header.
struct FOG_NO_EXPORT OTCMapFormat6Header
{
  //! @brief  Format number is set to 6.
  OTUInt16 format;
  //! @brief Table length in bytes.
  OTUInt16 length;
  //! @brief Mac language code.
  OTUInt16 macLanguageCode;
  //! @brief First segment code.
  OTUInt16 first;
  //! @brief Segment size in bytes.
  OTUInt16 count;
  //! @brief Glyph index array.
  OTUInt16 glyphIdArray[1];
};

// ============================================================================
// [Fog::OTCMap8]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 8.
// UInt16       | 2            | reserved             | Reserved.
// UInt32       | 4            | length               | Table length in bytes.
// UInt32       | 8            | macLanguageCode      | Mac language code.
// UInt8[8192]  | 12           | is32                 | Tightly packed array of bits indicating
//              |              |                      | whether the particular 16-bit (index) value
//              |              |                      | is the start of a 32-bit character code.
// UInt32       | 8204         | count (N)            | Number of groupings which follow.
//
// The header is followed by N groups in the following format:
//
// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt32       | 0            | startChar            | Start character code.
// UInt32       | 4            | lastChar             | Last character code.
// UInt32       | 8            | startGlyph           | Start glyph ID for the group.

struct FOG_NO_EXPORT OTCMapFormat8Group
{
  //! @brief Start character code.
  OTUInt32 startChar;
  //! @brief Last character code.
  OTUInt32 lastChar;
  //! @brief Start glyph ID for the group.
  OTUInt32 startGlyph;
};

//! @brief CMap format 8 header.
struct FOG_NO_EXPORT OTCMapFormat8Header
{
  //! @brief  Format number is set to 8.
  OTUInt16 format;
  //! @brief  Reserved.
  OTUInt16 reserved;
  //! @brief Table length in bytes.
  OTUInt32 length;
  //! @brief Mac language code.
  OTUInt32 macLanguageCode;
  //! @brief Tightly packed array of bits indicating whether the particular 
  //! 16-bit (index) value is the start of a 32-bit character code.
  OTUInt8 is32[8192];
  //! @brief Number of groupings which follow..
  OTUInt32 count;
  //! @brief Groups.
  OTCMapFormat8Group groups[1];
};

// ============================================================================
// [Fog::OTCMap10]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 10.
// UInt16       | 2            | reserved             | Reserved.
// UInt32       | 4            | length               | Table length in bytes.
// UInt32       | 8            | macLanguageCode      | Mac language code.
// UInt32       | 12           | startChar            | First character in range.
// UInt32       | 16           | count (N)            | Number of characters in range.
// UInt16[N]    | 20           | glyphIdArray         | Glyph index array.

//! @brief CMap format 10 header.
struct FOG_NO_EXPORT OTCMapFormat10Header
{
  //! @brief  Format number is set to 10.
  OTUInt16 format;
  //! @brief  Reserved.
  OTUInt16 reserved;
  //! @brief Table length in bytes.
  OTUInt32 length;
  //! @brief Mac language code.
  OTUInt32 macLanguageCode;
  //! @brief First character in range.
  OTUInt32 startChar;
  //! @brief Count of characters in range.
  OTUInt32 count;
  //! @brief Glyph index array.
  OTUInt16 glyphIdArray[1];
};

// ============================================================================
// [Fog::OTCMap12]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 12.
// UInt16       | 2            | reserved             | Reserved.
// UInt32       | 4            | length               | Table length in bytes.
// UInt32       | 8            | macLanguageCode      | Mac language code.
// UInt32       | 12           | count (N)            | Number of groups.
//
// The header is followed by N groups in the following format:
//
// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt32       | 0            | startChar            | First character code.
// UInt32       | 4            | lastChar             | Last character code.
// UInt32       | 8            | startGlyph           | Start glyph ID for the group.

//! @brief CMap format 12 group.
struct FOG_NO_EXPORT OTCMapFormat12Group
{
  //! @brief First character code.
  OTUInt32 startChar;
  //! @brief Last character code.
  OTUInt32 lastChar;
  //! @brief Start glyph ID for the group.
  OTUInt32 startGlyph;
};

//! @brief CMap format 12 header.
struct FOG_NO_EXPORT OTCMapFormat12Header
{
  //! @brief  Format number is set to 12.
  OTUInt16 format;
  //! @brief  Reserved.
  OTUInt16 reserved;
  //! @brief Table length in bytes.
  OTUInt32 length;
  //! @brief Mac language code.
  OTUInt32 macLanguageCode;
  //! @brief Number of groups.
  OTUInt32 count;
  //! @brief Glyph index array.
  OTCMapFormat12Group groups[1];
};

// ============================================================================
// [Fog::OTCMap13]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 13.
// UInt16       | 2            | reserved             | Reserved.
// UInt32       | 4            | length               | Table length in bytes.
// UInt32       | 8            | macLanguageCode      | Mac language code.
// UInt32       | 12           | count (N)            | Number of groups.
//
// The header is followed by N groups in the following format:
//
// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt32       | 0            | start                | First character code.
// UInt32       | 4            | end                  | Last character code.
// UInt32       | 8            | glyphId              | Glyph ID for the whole group.

// ============================================================================
// [Fog::OTCMap14]
// ============================================================================

// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt16       | 0            | format               | Format number is set to 14.
// UInt32       | 2            | length               | Table length in bytes.
// UInt32       | 8            | numSelector (N)      | Number of variation selection records.
//
// The header is followed by N selection records in the following format:
//
// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt24       | 0            | varSelector          | Unicode code-point of selection.
// UInt32       | 3            | defaultOffset        | Offset to a default UVS table.
// UInt32       | 7            | nonDefaultOffset     | Offset to a non-default UVS table.
//
// Selectors are sorted by code-point.
//
// A default Unicode Variation Selector (UVS) subtable is just a list of ranges
// of code-points which are to be found in the standard cmap. No glyph IDs here:
//
// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt32       | 0            | numRanges            | Number of ranges following.
// -------------+--------------+----------------------+------------------------
// UInt24       | 0            | uniStart             | First code-point in range.
// UInt8        | 3            | additionalCount      | Count of additional characters (can be zero).
//
// Ranges are sorted by uniStart:
//
// Type         | Offset       | Name                 | Description
// -------------+--------------+----------------------+------------------------
// UInt32       | 0            | numMappings          | Number of mappings.
// -------------+--------------+----------------------+------------------------
// UInt24       | 0            | uniStart             | First code-point in range.
// UInt16       | 3            | glyphID              | Glyph ID of the first character.

// ============================================================================
// [Fog::OTCMapHeader]
// ============================================================================

//! @brief TrueType/OpenType 'cmap' - Character to glyph mapping header.
struct FOG_NO_EXPORT OTCMapHeader
{
  //! @brief Version of 'cmap' header, must be 0.
  OTUInt16 version;
  //! @brief Count of 'cmap' (sub)tables.
  OTUInt16 count;
};

// ============================================================================
// [Fog::OTCMapEncoding]
// ============================================================================

//! @brief TrueType/OpenType 'cmap' - Character to glyph mapping encoding.
struct FOG_NO_EXPORT OTCMapEncoding
{
  //! @brief Platform identifier.
  OTUInt16 platformId;
  //! @brief Plaform-specific encoding identifier.
  OTUInt16 specificId;
  //! @brief Offset of the mapping table.
  OTUInt32 offset;
};

// ============================================================================
// [Fog::OTCMapItem]
// ============================================================================

//! @brief TrueType/OpenType 'cmap' - Character to glyph mapping parsed item,
//! internally by Fog-Framework.
struct FOG_NO_EXPORT OTCMapItem
{
  uint32_t encodingId;
  uint32_t priority;

  uint32_t specificData;
  err_t status;
};

// ============================================================================
// [Fog::OTCMap]
// ============================================================================

//! @brief TrueType/OpenType 'cmap' - Character to glyph mapping table.
//!
//! Specification:
//!   - http://www.microsoft.com/typography/otspec/cmap.htm
//!   - https://developer.apple.com/fonts/ttrefman/RM06/Chap6cmap.html
struct FOG_NO_EXPORT OTCMap : public OTTable
{
  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE const OTCMapHeader* getHeader() const { return reinterpret_cast<OTCMapHeader*>(_data); }

  FOG_INLINE uint32_t getUnicodeEncodingIndex() const { return _unicodeEncodingIndex; }

  FOG_INLINE uint32_t getCount() const { return _count; }
  FOG_INLINE OTCMapItem* getItems() const { return _items; }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  uint32_t _unicodeEncodingIndex;
  uint32_t _count;
  OTCMapItem* _items;

  //! @brief Get character placement.
  OTCMapInitContextFunc _initContext;
};

// ============================================================================
// [Fog::OTCMapContext]
// ============================================================================

struct FOG_NO_EXPORT OTCMapContext
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE OTCMapContext()
  {
    _getGlyphPlacementFunc = NULL;
  }

  // --------------------------------------------------------------------------
  // [Methods]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t init(OTCMap* cmap, uint32_t encodingId)
  {
    FOG_ASSERT_X(cmap != NULL,
      "Fog::OTCMapContext::init() - Called on NULL cmap.");
    FOG_ASSERT_X(cmap->_initContext != NULL,
      "Fog::OTCMapContext::init() - Called on unsupported cmap.");

    return cmap->_initContext(this, cmap, encodingId);
  }

  FOG_INLINE size_t getGlyphPlacement(uint32_t* glyphId, size_t glyphAdvance, const uint16_t* sData, size_t sLength)
  {
    FOG_ASSERT_X(_getGlyphPlacementFunc != NULL,
      "Fog::OTCmapContext::getGlyphPlacement() - Called on uninitialized context");

    return _getGlyphPlacementFunc(this, glyphId, glyphAdvance, sData, sLength);
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! @brief Pointer to 'getGlyphPlacement' implementation.
  OTCMapGetGlyphPlacementFunc _getGlyphPlacementFunc;

  //! @brief Pointer to data used by 'getGlyphPlacement' implementation.
  //!
  //! The data pointer is extracted and offseted from the table data. It points
  //! to the first byte of a selected 'cmap' subtable.
  const uint8_t* _data;

  //! @brief Link to the parsed 'cmap' item (subtable).
  //!
  //! The pointer to 'cmap' item is here by design. In future we can extend
  //! the item without breaking binary compatibility in 'OTCMapContext'.
  const OTCMapItem* _item;
};

//! @}

// [Byte-Pack]
#include <Fog/Core/C++/PackRestore.h>

} // Fog namespace

// [Guard]
#endif // _FOG_G2D_TEXT_OPENTYPE_OTCMAP_H
