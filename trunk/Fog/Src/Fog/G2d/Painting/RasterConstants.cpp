// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/G2d/Painting/RasterConstants_p.h>

namespace Fog {

// ============================================================================
// [Fog::Raster - Data - CombineProperties]
// ============================================================================

// ${COMPOSITE_OP:BEGIN}
uint32_t _raster_combineProperties[COMPOSITE_COUNT] =
{
  /* 00: SRC         -> */ RASTER_COMBINE_OP_SRC,
  /* 01: SRC_OVER    -> */ RASTER_COMBINE_OP_SRC_OVER,
  /* 02: SRC_IN      -> */ RASTER_COMBINE_OP_SRC_IN,
  /* 03: SRC_OUT     -> */ RASTER_COMBINE_OP_SRC_OUT,
  /* 04: SRC_ATOP    -> */ RASTER_COMBINE_OP_SRC_ATOP,
  /* 05: DST         -> */ RASTER_COMBINE_OP_DST,
  /* 06: DST_OVER    -> */ RASTER_COMBINE_OP_DST_OVER,
  /* 07: DST_IN      -> */ RASTER_COMBINE_OP_DST_IN,
  /* 08: DST_OUT     -> */ RASTER_COMBINE_OP_DST_OUT,
  /* 09: DST_ATOP    -> */ RASTER_COMBINE_OP_DST_ATOP,
  /* 10: XOR         -> */ RASTER_COMBINE_OP_XOR,
  /* 11: CLEAR       -> */ RASTER_COMBINE_OP_CLEAR,
  /* 12: PLUS        -> */ RASTER_COMBINE_OP_PLUS,
  /* 13: MINUS       -> */ RASTER_COMBINE_OP_MINUS,
  /* 14: MULTIPLY    -> */ RASTER_COMBINE_OP_MULTIPLY,
  /* 15: SCREEN      -> */ RASTER_COMBINE_OP_SCREEN,
  /* 16: OVERLAY     -> */ RASTER_COMBINE_OP_OVERLAY,
  /* 17: DARKEN      -> */ RASTER_COMBINE_OP_DARKEN,
  /* 18: LIGHTEN     -> */ RASTER_COMBINE_OP_LIGHTEN,
  /* 19: COLOR_DODGE -> */ RASTER_COMBINE_OP_COLOR_DODGE,
  /* 20: COLOR_BURN  -> */ RASTER_COMBINE_OP_COLOR_BURN,
  /* 21: HARD_LIGHT  -> */ RASTER_COMBINE_OP_HARD_LIGHT,
  /* 22: SOFT_LIGHT  -> */ RASTER_COMBINE_OP_SOFT_LIGHT,
  /* 23: DIFFERENCE  -> */ RASTER_COMBINE_OP_DIFFERENCE,
  /* 24: EXCLUSION   -> */ RASTER_COMBINE_OP_EXCLUSION
};
// ${COMPOSITE_OP:END}

// ============================================================================
// [Fog::Raster - Data - LinearBlur8]
// ============================================================================

const uint16_t _raster_blur_stack_8_mul[256] =
{
  512, 512, 456, 512, 328, 456, 335, 512, 405, 328, 271, 456, 388, 335, 292, 512,
  454, 405, 364, 328, 298, 271, 496, 456, 420, 388, 360, 335, 312, 292, 273, 512,
  482, 454, 428, 405, 383, 364, 345, 328, 312, 298, 284, 271, 259, 496, 475, 456,
  437, 420, 404, 388, 374, 360, 347, 335, 323, 312, 302, 292, 282, 273, 265, 512,
  497, 482, 468, 454, 441, 428, 417, 405, 394, 383, 373, 364, 354, 345, 337, 328,
  320, 312, 305, 298, 291, 284, 278, 271, 265, 259, 507, 496, 485, 475, 465, 456,
  446, 437, 428, 420, 412, 404, 396, 388, 381, 374, 367, 360, 354, 347, 341, 335,
  329, 323, 318, 312, 307, 302, 297, 292, 287, 282, 278, 273, 269, 265, 261, 512,
  505, 497, 489, 482, 475, 468, 461, 454, 447, 441, 435, 428, 422, 417, 411, 405,
  399, 394, 389, 383, 378, 373, 368, 364, 359, 354, 350, 345, 341, 337, 332, 328,
  324, 320, 316, 312, 309, 305, 301, 298, 294, 291, 287, 284, 281, 278, 274, 271,
  268, 265, 262, 259, 257, 507, 501, 496, 491, 485, 480, 475, 470, 465, 460, 456,
  451, 446, 442, 437, 433, 428, 424, 420, 416, 412, 408, 404, 400, 396, 392, 388,
  385, 381, 377, 374, 370, 367, 363, 360, 357, 354, 350, 347, 344, 341, 338, 335,
  332, 329, 326, 323, 320, 318, 315, 312, 310, 307, 304, 302, 299, 297, 294, 292,
  289, 287, 285, 282, 280, 278, 275, 273, 271, 269, 267, 265, 263, 261, 259,   0
};

const uint8_t _raster_blur_stack_8_shr[256] =
{
  9 , 11, 12, 13, 13, 14, 14, 15, 15, 15, 15, 16, 16, 16, 16, 17,
  17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 19,
  19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 20, 20, 20,
  20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21,
  21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
  21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22,
  22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
  22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23,
  23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
  23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
  23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
  23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
  24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 32
};

// ============================================================================
// [Fog::Raster - Data - CompatibleFormat]
// ============================================================================

// ${IMAGE_FORMAT:BEGIN}
// ${COMPOSITE:BEGIN}
const RasterCompatibleFormat _raster_compatibleFormat[IMAGE_FORMAT_COUNT][IMAGE_FORMAT_COUNT] =
{
  // --------------------------------------------------------------------------
  // [PRGB32]
  // --------------------------------------------------------------------------

  {
    /* PRGB32    <- PRGB32    */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* PRGB32    <- XRGB32    */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_XRGB32   , RASTER_VBLIT_PRGB32_AND_XRGB32, 0 },
    /* PRGB32    <- RGB24     */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_RGB24    , RASTER_VBLIT_PRGB32_AND_RGB24 , 0 },
    /* PRGB32    <- A8        */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_A8       , RASTER_VBLIT_PRGB32_AND_A8    , 0 },
    /* PRGB32    <- I8        */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* PRGB32    <- PRGB64    */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* PRGB32    <- RGB48     */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_XRGB32   , RASTER_VBLIT_PRGB32_AND_XRGB32, 0 },
    /* PRGB32    <- A16       */ { IMAGE_FORMAT_PRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 }
  },

  // --------------------------------------------------------------------------
  // [XRGB32]
  // --------------------------------------------------------------------------

  {
    /* XRGB32    <- PRGB32    */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* XRGB32    <- XRGB32    */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_XRGB32   , RASTER_VBLIT_PRGB32_AND_XRGB32, 0 },
    /* XRGB32    <- RGB24     */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_RGB24    , RASTER_VBLIT_PRGB32_AND_RGB24 , 0 },
    /* XRGB32    <- A8        */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* XRGB32    <- I8        */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* XRGB32    <- PRGB64    */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* XRGB32    <- RGB48     */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_XRGB32   , RASTER_VBLIT_PRGB32_AND_XRGB32, 0 },
    /* XRGB32    <- A16       */ { IMAGE_FORMAT_XRGB32   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 }
  },

  // --------------------------------------------------------------------------
  // [RGB24]
  // --------------------------------------------------------------------------

  {
    /* RGB24     <- PRGB32    */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* RGB24     <- XRGB32    */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_XRGB32   , RASTER_VBLIT_PRGB32_AND_XRGB32, 0 },
    /* RGB24     <- RGB24     */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_RGB24    , RASTER_VBLIT_PRGB32_AND_RGB24 , 0 },
    /* RGB24     <- A8        */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* RGB24     <- I8        */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* RGB24     <- PRGB64    */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 },
    /* RGB24     <- RGB48     */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_XRGB32   , RASTER_VBLIT_PRGB32_AND_XRGB32, 0 },
    /* RGB24     <- A16       */ { IMAGE_FORMAT_RGB24    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB32_AND_PRGB32, 0 }
  },

  // --------------------------------------------------------------------------
  // [A8]
  // --------------------------------------------------------------------------

  {
    /* A8        <- PRGB32    */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_A8_AND_PRGB32    , 0 },
    /* A8        <- XRGB32    */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 },
    /* A8        <- RGB24     */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 },
    /* A8        <- A8        */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 },
    /* A8        <- I8        */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 },
    /* A8        <- PRGB64    */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 },
    /* A8        <- RGB48     */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 },
    /* A8        <- A16       */ { IMAGE_FORMAT_A8       , IMAGE_FORMAT_A8       , RASTER_VBLIT_A8_AND_A8        , 0 }
  },

  // --------------------------------------------------------------------------
  // [I8]
  // --------------------------------------------------------------------------

  {
    /* I8        <- PRGB32    */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- XRGB32    */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- RGB24     */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- A8        */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- I8        */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- PRGB64    */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- RGB48     */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 },
    /* I8        <- A16       */ { IMAGE_FORMAT_NULL     , IMAGE_FORMAT_NULL     , RASTER_VBLIT_INVALID         , 0 }
  },

  // --------------------------------------------------------------------------
  // [PRGB64]
  // --------------------------------------------------------------------------

  {
    /* PRGB64    <- PRGB32    */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB64_AND_PRGB32, 0 },
    /* PRGB64    <- XRGB32    */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_RGB48    , RASTER_VBLIT_PRGB64_AND_RGB48 , 0 },
    /* PRGB64    <- RGB24     */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_RGB48    , RASTER_VBLIT_PRGB64_AND_RGB48 , 0 },
    /* PRGB64    <- A8        */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB64_AND_PRGB32, 0 },
    /* PRGB64    <- I8        */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB64_AND_PRGB32, 0 },
    /* PRGB64    <- PRGB64    */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_PRGB64   , RASTER_VBLIT_PRGB64_AND_PRGB64, 0 },
    /* PRGB64    <- RGB48     */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_RGB48    , RASTER_VBLIT_PRGB64_AND_RGB48 , 0 },
    /* PRGB64    <- A16       */ { IMAGE_FORMAT_PRGB64   , IMAGE_FORMAT_PRGB64   , RASTER_VBLIT_PRGB64_AND_PRGB64, 0 }
  },

  // --------------------------------------------------------------------------
  // [RGB48]
  // --------------------------------------------------------------------------

  {
    /* RGB48     <- PRGB32    */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB64_AND_PRGB32 , 0 },
    /* RGB48     <- XRGB32    */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_RGB48    , RASTER_VBLIT_PRGB64_AND_RGB48  , 0 },
    /* RGB48     <- RGB24     */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_RGB48    , RASTER_VBLIT_PRGB64_AND_RGB48  , 0 },
    /* RGB48     <- A8        */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB64_AND_PRGB32 , 0 },
    /* RGB48     <- I8        */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_PRGB32   , RASTER_VBLIT_PRGB64_AND_PRGB32 , 0 },
    /* RGB48     <- PRGB64    */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_PRGB64   , RASTER_VBLIT_PRGB64_AND_PRGB64 , 0 },
    /* RGB48     <- RGB48     */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_RGB48    , RASTER_VBLIT_PRGB64_AND_RGB48  , 0 },
    /* RGB48     <- A16       */ { IMAGE_FORMAT_RGB48    , IMAGE_FORMAT_PRGB64   , RASTER_VBLIT_PRGB64_AND_PRGB64 , 0 }
  },

  // --------------------------------------------------------------------------
  // [A16]
  // --------------------------------------------------------------------------

  {
    /* A16       <- PRGB32    */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A8       , RASTER_VBLIT_A16_AND_A8        , 0 },
    /* A16       <- XRGB32    */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A8       , RASTER_VBLIT_A16_AND_A8        , 0 },
    /* A16       <- RGB24     */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A8       , RASTER_VBLIT_A16_AND_A8        , 0 },
    /* A16       <- A8        */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A8       , RASTER_VBLIT_A16_AND_A8        , 0 },
    /* A16       <- I8        */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A8       , RASTER_VBLIT_A16_AND_A8        , 0 },
    /* A16       <- PRGB64    */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_PRGB64   , RASTER_VBLIT_A16_AND_PRGB64    , 0 },
    /* A16       <- RGB48     */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A16      , RASTER_VBLIT_A16_AND_A16       , 0 },
    /* A16       <- A16       */ { IMAGE_FORMAT_A16      , IMAGE_FORMAT_A16      , RASTER_VBLIT_A16_AND_A16       , 0 }
  }
};
// ${COMPOSITE:END}
// ${IMAGE_FORMAT:END}

// ============================================================================
// [Fog::Raster - Data - ModifiedOperator]
// ============================================================================

// ${IMAGE_FORMAT:BEGIN}
// ${COMPOSITE:BEGIN}
const uint8_t _raster_modifiedOperator[IMAGE_FORMAT_COUNT][COMPOSITE_COUNT][2] =
{
  // --------------------------------------------------------------------------
  // [PRGB32]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC_IN      , COMPOSITE_SRC_IN      },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_SRC_OUT     , COMPOSITE_SRC_OUT     },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_SRC_ATOP    , COMPOSITE_SRC_ATOP    },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_DST_OVER    , COMPOSITE_DST_OVER    },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_DST_IN      , COMPOSITE_DST_IN      },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_DST_OUT     },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_DST_ATOP    , COMPOSITE_DST_ATOP    },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_XOR         , COMPOSITE_XOR         },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_MINUS       , COMPOSITE_MINUS       },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_MULTIPLY    , COMPOSITE_MULTIPLY    },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SCREEN      , COMPOSITE_SCREEN      },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_OVERLAY     , COMPOSITE_OVERLAY     },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_DARKEN      , COMPOSITE_DARKEN      },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_LIGHTEN     , COMPOSITE_LIGHTEN     },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_COLOR_DODGE , COMPOSITE_COLOR_DODGE },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_COLOR_BURN  , COMPOSITE_COLOR_BURN  },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_HARD_LIGHT  , COMPOSITE_HARD_LIGHT  },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SOFT_LIGHT  , COMPOSITE_SOFT_LIGHT  },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_DIFFERENCE  , COMPOSITE_DIFFERENCE  },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_EXCLUSION   , COMPOSITE_EXCLUSION   }
  },
  
  // --------------------------------------------------------------------------
  // [XRGB32]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_DST_IN      , COMPOSITE_DST_IN      },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_DST_OUT     },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_DST_IN      , COMPOSITE_DST_ATOP    },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_DST_OUT     , COMPOSITE_XOR         },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_MINUS       , COMPOSITE_MINUS       },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_MULTIPLY    , COMPOSITE_MULTIPLY    },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SCREEN      , COMPOSITE_SCREEN      },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_OVERLAY     , COMPOSITE_OVERLAY     },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_DARKEN      , COMPOSITE_DARKEN      },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_LIGHTEN     , COMPOSITE_LIGHTEN     },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_COLOR_DODGE , COMPOSITE_COLOR_DODGE },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_COLOR_BURN  , COMPOSITE_COLOR_BURN  },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_HARD_LIGHT  , COMPOSITE_HARD_LIGHT  },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SOFT_LIGHT  , COMPOSITE_SOFT_LIGHT  },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_DIFFERENCE  , COMPOSITE_DIFFERENCE  },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_EXCLUSION   , COMPOSITE_EXCLUSION   }
  },

  // --------------------------------------------------------------------------
  // [RGB24]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_DST_IN      , COMPOSITE_DST_IN      },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_DST_OUT     },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_DST_IN      , COMPOSITE_DST_ATOP    },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_DST_OUT     , COMPOSITE_XOR         },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_MINUS       , COMPOSITE_MINUS       },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_MULTIPLY    , COMPOSITE_MULTIPLY    },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SCREEN      , COMPOSITE_SCREEN      },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_OVERLAY     , COMPOSITE_OVERLAY     },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_DARKEN      , COMPOSITE_DARKEN      },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_LIGHTEN     , COMPOSITE_LIGHTEN     },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_COLOR_DODGE , COMPOSITE_COLOR_DODGE },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_COLOR_BURN  , COMPOSITE_COLOR_BURN  },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_HARD_LIGHT  , COMPOSITE_HARD_LIGHT  },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SOFT_LIGHT  , COMPOSITE_SOFT_LIGHT  },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_DIFFERENCE  , COMPOSITE_DIFFERENCE  },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_EXCLUSION   , COMPOSITE_EXCLUSION   }
  },

  // --------------------------------------------------------------------------
  // [A8]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC_IN      , COMPOSITE_DST         },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_SRC_OUT     , COMPOSITE_SRC_OUT     },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC_OVER    },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_SRC_IN      , COMPOSITE_DST         },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_CLEAR       },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_XOR         , COMPOSITE_SRC_OUT     },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         }
  },

  // --------------------------------------------------------------------------
  // [I8]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_DST_IN      , COMPOSITE_DST_IN      },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_DST_OUT     },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_DST_IN      , COMPOSITE_DST_ATOP    },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_DST_OUT     , COMPOSITE_XOR         },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_MINUS       , COMPOSITE_MINUS       },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_MULTIPLY    , COMPOSITE_MULTIPLY    },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SCREEN      , COMPOSITE_SCREEN      },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_OVERLAY     , COMPOSITE_OVERLAY     },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_DARKEN      , COMPOSITE_DARKEN      },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_LIGHTEN     , COMPOSITE_LIGHTEN     },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_COLOR_DODGE , COMPOSITE_COLOR_DODGE },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_COLOR_BURN  , COMPOSITE_COLOR_BURN  },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_HARD_LIGHT  , COMPOSITE_HARD_LIGHT  },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SOFT_LIGHT  , COMPOSITE_SOFT_LIGHT  },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_DIFFERENCE  , COMPOSITE_DIFFERENCE  },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_EXCLUSION   , COMPOSITE_EXCLUSION   }
  },

  // --------------------------------------------------------------------------
  // [PRGB64]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC_IN      , COMPOSITE_SRC_IN      },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_SRC_OUT     , COMPOSITE_SRC_OUT     },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_SRC_ATOP    , COMPOSITE_SRC_ATOP    },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_DST_OVER    , COMPOSITE_DST_OVER    },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_DST_IN      , COMPOSITE_DST_IN      },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_DST_OUT     },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_DST_ATOP    , COMPOSITE_DST_ATOP    },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_XOR         , COMPOSITE_XOR         },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_MINUS       , COMPOSITE_MINUS       },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_MULTIPLY    , COMPOSITE_MULTIPLY    },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SCREEN      , COMPOSITE_SCREEN      },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_OVERLAY     , COMPOSITE_OVERLAY     },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_DARKEN      , COMPOSITE_DARKEN      },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_LIGHTEN     , COMPOSITE_LIGHTEN     },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_COLOR_DODGE , COMPOSITE_COLOR_DODGE },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_COLOR_BURN  , COMPOSITE_COLOR_BURN  },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_HARD_LIGHT  , COMPOSITE_HARD_LIGHT  },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SOFT_LIGHT  , COMPOSITE_SOFT_LIGHT  },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_DIFFERENCE  , COMPOSITE_DIFFERENCE  },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_EXCLUSION   , COMPOSITE_EXCLUSION   }
  },

  // --------------------------------------------------------------------------
  // [RGB48]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_DST_IN      , COMPOSITE_DST_IN      },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_DST_OUT     },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_DST_IN      , COMPOSITE_DST_ATOP    },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_DST_OUT     , COMPOSITE_XOR         },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_MINUS       , COMPOSITE_MINUS       },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_MULTIPLY    , COMPOSITE_MULTIPLY    },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SCREEN      , COMPOSITE_SCREEN      },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_OVERLAY     , COMPOSITE_OVERLAY     },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_DARKEN      , COMPOSITE_DARKEN      },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_LIGHTEN     , COMPOSITE_LIGHTEN     },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_COLOR_DODGE , COMPOSITE_COLOR_DODGE },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_COLOR_BURN  , COMPOSITE_COLOR_BURN  },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_HARD_LIGHT  , COMPOSITE_HARD_LIGHT  },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SOFT_LIGHT  , COMPOSITE_SOFT_LIGHT  },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_DIFFERENCE  , COMPOSITE_DIFFERENCE  },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_EXCLUSION   , COMPOSITE_EXCLUSION   }
  },

  // --------------------------------------------------------------------------
  // [A16]
  // --------------------------------------------------------------------------

  {
    // -----------------------------+-----------------------+-----------------
    // XX: Operator name            | Alpha+RGB             | RGB only
    // -----------------------------+-----------------------+-----------------

    /* 00: COMPOSITE_SRC         */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 01: COMPOSITE_SRC_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 02: COMPOSITE_SRC_IN      */ { COMPOSITE_SRC_IN      , COMPOSITE_DST         },
    /* 03: COMPOSITE_SRC_OUT     */ { COMPOSITE_SRC_OUT     , COMPOSITE_SRC_OUT     },
    /* 04: COMPOSITE_SRC_ATOP    */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 05: COMPOSITE_DST         */ { COMPOSITE_DST         , COMPOSITE_DST         },
    /* 06: COMPOSITE_DST_OVER    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC_OVER    },
    /* 07: COMPOSITE_DST_IN      */ { COMPOSITE_SRC_IN      , COMPOSITE_DST         },
    /* 08: COMPOSITE_DST_OUT     */ { COMPOSITE_DST_OUT     , COMPOSITE_CLEAR       },
    /* 09: COMPOSITE_DST_ATOP    */ { COMPOSITE_SRC         , COMPOSITE_SRC         },
    /* 10: COMPOSITE_XOR         */ { COMPOSITE_XOR         , COMPOSITE_SRC_OUT     },
    /* 11: COMPOSITE_CLEAR       */ { COMPOSITE_CLEAR       , COMPOSITE_CLEAR       },
    /* 12: COMPOSITE_PLUS        */ { COMPOSITE_PLUS        , COMPOSITE_PLUS        },
    /* 13: COMPOSITE_MINUS       */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 14: COMPOSITE_MULTIPLY    */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 15: COMPOSITE_SCREEN      */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 16: COMPOSITE_OVERLAY     */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 17: COMPOSITE_DARKEN      */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 18: COMPOSITE_LIGHTEN     */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 19: COMPOSITE_COLOR_DODGE */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 20: COMPOSITE_COLOR_BURN  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 21: COMPOSITE_HARD_LIGHT  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 22: COMPOSITE_SOFT_LIGHT  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 23: COMPOSITE_DIFFERENCE  */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         },
    /* 24: COMPOSITE_EXCLUSION   */ { COMPOSITE_SRC_OVER    , COMPOSITE_SRC         }
  }
};
// ${COMPOSITE:END}
// ${IMAGE_FORMAT:END}

} // Fog namespace
