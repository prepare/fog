// [Fog-Graphics Library - Private API]
//
// [License]
// MIT, See COPYING file in package

// For some IDEs to enable code-assist.
#include <Fog/Core/Build.h>

#if defined(FOG_IDE)
#include <Fog/Graphics/RasterEngine/Defs_C_p.h>
#endif // FOG_IDE

namespace Fog {
namespace RasterEngine {

// ============================================================================
// [Fog::RasterEngine::C - Composite - Add]
// ============================================================================

//! @internal
struct FOG_HIDDEN CompositeAddC : public CompositeBaseFuncsC32<CompositeAddC>
{
  enum { CHARACTERISTICS = OPERATOR_CHAR_ADD };

  static FOG_INLINE void prgb32_op_prgb32_32b(
    ByteUtil::byte1x2& dst0, ByteUtil::byte1x2 a0, ByteUtil::byte1x2 b0,
    ByteUtil::byte1x2& dst1, ByteUtil::byte1x2 a1, ByteUtil::byte1x2 b1)
  {
    ByteUtil::byte2x2_addus_byte2x2(dst0, a0, b0, dst1, a1, b1);
  }

  static FOG_INLINE void prgb32_op_xrgb32_32b(
    ByteUtil::byte1x2& dst0, ByteUtil::byte1x2 a0, ByteUtil::byte1x2 b0,
    ByteUtil::byte1x2& dst1, ByteUtil::byte1x2 a1, ByteUtil::byte1x2 b1)
  {
    ByteUtil::byte2x2_addus_byte2x2(dst0, a0, b0, dst1, a1, b1);
    ByteUtil::byte1x2_fill_hi(dst1, dst1);
  }

  static FOG_INLINE void xrgb32_op_prgb32_32b(
    ByteUtil::byte1x2& dst0, ByteUtil::byte1x2 a0, ByteUtil::byte1x2 b0,
    ByteUtil::byte1x2& dst1, ByteUtil::byte1x2 a1, ByteUtil::byte1x2 b1)
  {
    ByteUtil::byte2x2_addus_byte2x2(dst0, a0, b0, dst1, a1, b1);
  }

  static FOG_INLINE void xrgb32_op_xrgb32_32b(
    ByteUtil::byte1x2& dst0, ByteUtil::byte1x2 a0, ByteUtil::byte1x2 b0,
    ByteUtil::byte1x2& dst1, ByteUtil::byte1x2 a1, ByteUtil::byte1x2 b1)
  {
    ByteUtil::byte2x2_addus_byte2x2(dst0, a0, b0, dst1, a1, b1);
  }
};

} // RasterEngine namespace
} // Fog namespace
