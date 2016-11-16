// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_PAINTING_RASTEROPS_C_GRADIENTLINEAR_P_H
#define _FOG_G2D_PAINTING_RASTEROPS_C_GRADIENTLINEAR_P_H

// [Dependencies]
#include <Fog/G2d/Painting/RasterOps_C/GradientBase_p.h>

namespace Fog {
namespace RasterOps_C {

// ============================================================================
// [Fog::RasterOps_C - PGradientLinear]
// ============================================================================

struct FOG_NO_EXPORT PGradientLinear
{
  // ==========================================================================
  // [Create]
  // ==========================================================================

  // TODO: 16-bit image processing.
  static err_t FOG_FASTCALL create(
    RasterPattern* ctx, uint32_t dstFormat, const BoxI* clipBox,
    const GradientD* gradient,
    const TransformD* tr,
    uint32_t gradientQuality)
  {
    const ColorStopList& stops = gradient->getStops();
    uint32_t spread = gradient->getGradientSpread();

    FOG_ASSERT(gradient->getGradientType() == GRADIENT_TYPE_LINEAR);
    FOG_ASSERT(spread < GRADIENT_SPREAD_COUNT);

    TransformD inv(UNINITIALIZED);
    bool isInverted = TransformD::invert(inv, *tr);

    // ------------------------------------------------------------------------
    // [Solid]
    // ------------------------------------------------------------------------

    if (stops.isEmpty())
    {
      Color color(Argb32(0x00000000));
      return Helpers::p_solid_create_color(ctx, dstFormat, &color);
    }

    if (stops.getLength() < 2 || !isInverted)
    {
      const ColorStop& stop = stops.getAt(stops.getLength() - 1);
      return Helpers::p_solid_create_color(ctx, dstFormat, &stop._color);
    }

    // ------------------------------------------------------------------------
    // [Prepare]
    // ------------------------------------------------------------------------

    PointD origin;
    PointD pd = gradient->_pts[1] - gradient->_pts[0];

    double pd_x2y2 = pd.x * pd.x + pd.y * pd.y;
    double pd_dist = Math::sqrt(pd_x2y2);

    // If there is no such distance between the start and end point then the
    // color of the last color-stop is used. This is used for compatibility
    // with SVG.
    if (pd_dist < MATH_EPSILON_F)
    {
      const ColorStop& stop = stops.getAt(stops.getLength() - 1);
      return Helpers::p_solid_create_color(ctx, dstFormat, &stop._color);
    }

    FOG_RETURN_ON_ERROR(PGradientBase::create(ctx, dstFormat, clipBox, spread, &stops));
    int tableLength = ctx->_d.gradient.base.len;

    // TODO:
    uint32_t srcFormat = IMAGE_FORMAT_PRGB32;

    // ------------------------------------------------------------------------
    // [Simple]
    // ------------------------------------------------------------------------

    if (tr->getType() <= TRANSFORM_TYPE_AFFINE)
    {
      // Negate and adjust the origin by 0.5 to move it to the center of
      // the pixel.
      tr->mapPoint(origin, gradient->_pts[0]);
      origin.x = -origin.x + 0.5;
      origin.y = -origin.y + 0.5;

      double ax = (double)(pd.x);
      double ay = (double)(pd.y);

      double dx = ax * inv._00 + ay * inv._01;
      double dy = ax * inv._10 + ay * inv._11;

      dx = dx * (double)tableLength / pd_x2y2;
      dy = dy * (double)tableLength / pd_x2y2;

      ctx->_d.gradient.linear.simple.offset = origin.x * dx + origin.y * dy;
      ctx->_d.gradient.linear.simple.xx = dx;
      ctx->_d.gradient.linear.simple.xy = dy;
      ctx->_d.gradient.linear.simple.xx16x16 = Math::fixed16x16FromFloat(dx);

      ctx->_prepare = prepare_simple;
      ctx->_destroy = PGradientBase::destroy;
      ctx->_fetch = _api_raster.gradient.linear.fetch_simple_nearest[srcFormat][spread];
      ctx->_skip = skip_simple;
    }

    // ------------------------------------------------------------------------
    // [Projection]
    // ------------------------------------------------------------------------

    else
    {
      double ax = (double)(pd.x);
      double ay = (double)(pd.y);

      double dx = ax * (double)tableLength / pd_x2y2;
      double dy = ay * (double)tableLength / pd_x2y2;

      ctx->_d.gradient.linear.proj.offset = -gradient->_pts[0].x * dx + -gradient->_pts[0].y * dy;

      ctx->_d.gradient.linear.proj.xx = dx * inv._00 + dy * inv._01;
      ctx->_d.gradient.linear.proj.xy = dx * inv._10 + dy * inv._11;
      ctx->_d.gradient.linear.proj.xz = dx * inv._20 + dy * inv._21;

      ctx->_d.gradient.linear.proj.zx = inv._02;
      ctx->_d.gradient.linear.proj.zy = inv._12;
      ctx->_d.gradient.linear.proj.zz = inv._22;

      ctx->_prepare = prepare_proj;
      ctx->_destroy = PGradientBase::destroy;
      ctx->_fetch = _api_raster.gradient.linear.fetch_proj_nearest[srcFormat][spread];
      ctx->_skip = skip_proj;
    }

    return ERR_OK;
  }

  // ==========================================================================
  // [Prepare]
  // ==========================================================================

  static void FOG_FASTCALL prepare_simple(
    const RasterPattern* ctx, RasterPatternFetcher* fetcher, int _y, int _delta, uint32_t mode)
  {
    double y = (double)_y;
    double d = (double)_delta;

    fetcher->_ctx = ctx;
    fetcher->_fetch = ctx->_fetch;
    fetcher->_skip = ctx->_skip;
    fetcher->_mode = mode;

    fetcher->_d.gradient.linear.simple.pt = y * ctx->_d.gradient.linear.simple.xy + ctx->_d.gradient.linear.simple.offset;
    fetcher->_d.gradient.linear.simple.dt = d * ctx->_d.gradient.linear.simple.xy;
  }

  static void FOG_FASTCALL prepare_proj(
    const RasterPattern* ctx, RasterPatternFetcher* fetcher, int _y, int _delta, uint32_t mode)
  {
    double y = (double)_y;
    double d = (double)_delta;

    fetcher->_ctx = ctx;
    fetcher->_fetch = ctx->_fetch;
    fetcher->_skip = ctx->_skip;
    fetcher->_mode = mode;

    fetcher->_d.gradient.linear.proj.pt = y * ctx->_d.gradient.linear.proj.xy + ctx->_d.gradient.linear.proj.xz;
    fetcher->_d.gradient.linear.proj.dt = d * ctx->_d.gradient.linear.proj.xy;
    fetcher->_d.gradient.linear.proj.pz = y * ctx->_d.gradient.linear.proj.zy + ctx->_d.gradient.linear.proj.zz;
    fetcher->_d.gradient.linear.proj.dz = d * ctx->_d.gradient.linear.proj.zy;
  }

  // ==========================================================================
  // [Fetch - Simple - Pad]
  // ==========================================================================

  template<typename Accessor>
  static void FOG_FASTCALL fetch_simple_nearest_pad(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    const RasterPattern* ctx = fetcher->getContext();
    Accessor accessor(ctx);

    P_FETCH_SPAN8_INIT()

    int xx = ctx->_d.gradient.linear.simple.xx16x16;
    int len = ctx->_d.gradient.base.len16x16;

    int pos = Math::fixed16x16FromFloat(fetcher->_d.gradient.linear.simple.pt) + x * xx;

    typename Accessor::Pixel c0;
    typename Accessor::Pixel c1;

    accessor.fetchRaw(c0, 0);
    accessor.fetchRaw(c1, ctx->_d.gradient.base.len);

    // ------------------------------------------------------------------------
    // [Forward Direction]
    // ------------------------------------------------------------------------

    if (xx > 0)
    {
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()

        while (pos <= 0)
        {
          accessor.storePix(dst, c0);
          pos += xx;
          dst += Accessor::DST_BPP;
          if (--w == 0) goto _FetchForwardSkip;
        }

        while (pos < len)
        {
          accessor.storeRaw(dst, pos >> 16);
          pos += xx;
          dst += Accessor::DST_BPP;
          if (--w == 0) goto _FetchForwardSkip;
        }

        c0 = c1;
        goto _FetchSolidLoop;

_FetchForwardSkip:
        P_FETCH_SPAN8_HOLE(
        {
          pos += hole * xx;
        })
      P_FETCH_SPAN8_END()

      goto _End;
    }

    // ------------------------------------------------------------------------
    // [Backward Direction]
    // ------------------------------------------------------------------------

    else if (xx < 0)
    {
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()

        while (pos >= len)
        {
          accessor.storePix(dst, c1);
          pos += xx;
          dst += Accessor::DST_BPP;
          if (--w == 0) goto _FetchBackwardSkip;
        }

        while (pos >= 0)
        {
          accessor.storeRaw(dst, pos >> 16);
          pos += xx;
          dst += Accessor::DST_BPP;
          if (--w == 0) goto _FetchBackwardSkip;
        }
        goto _FetchSolidLoop;

_FetchBackwardSkip:
        P_FETCH_SPAN8_HOLE(
        {
          pos += hole * xx;
        })
      P_FETCH_SPAN8_END()

      goto _End;
    }

    // ------------------------------------------------------------------------
    // [Solid]
    // ------------------------------------------------------------------------

    else
    {
      if (pos > len)
        c0 = c1;
      else if (pos >= 0)
        accessor.fetchRaw(c0, pos >> 16);
    }

    P_FETCH_SPAN8_BEGIN()
      P_FETCH_SPAN8_SET_CURRENT()
      do {
_FetchSolidLoop:
        accessor.storePix(dst, c0);
        dst += Accessor::DST_BPP;
      } while (--w);
      P_FETCH_SPAN8_NEXT()
    P_FETCH_SPAN8_END()

    // ------------------------------------------------------------------------
    // [Advance]
    // ------------------------------------------------------------------------

_End:
    fetcher->_d.gradient.linear.simple.pt += fetcher->_d.gradient.linear.simple.dt;
  }

  // ==========================================================================
  // [Fetch - Simple - Repeat]
  // ==========================================================================

  template<typename Accessor>
  static void FOG_FASTCALL fetch_simple_nearest_repeat(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    const RasterPattern* ctx = fetcher->getContext();
    Accessor accessor(ctx);

    P_FETCH_SPAN8_INIT()

    int xx = ctx->_d.gradient.linear.simple.xx16x16;
    int len = ctx->_d.gradient.base.len16x16;

    int pos = Helpers::p_repeat_integer(
      Math::fixed16x16FromFloat(fetcher->_d.gradient.linear.simple.pt) + x * xx, len);

    // ------------------------------------------------------------------------
    // [Forward Direction]
    // ------------------------------------------------------------------------

    if (xx > 0)
    {
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()

        do {
          accessor.storeRaw(dst, pos >> 16);
          if ((pos += xx) >= len) pos -= len;
          dst += Accessor::DST_BPP;
        } while (--w);

        P_FETCH_SPAN8_HOLE(
        {
          pos += xx * hole;
          if (pos > len) pos %= len;
        })
      P_FETCH_SPAN8_END()
    }

    // ------------------------------------------------------------------------
    // [Backward Direction]
    // ------------------------------------------------------------------------

    else if (xx < 0)
    {
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()

        do {
          accessor.storeRaw(dst, pos >> 16);
          if ((pos += xx) < 0) pos += len;
          dst += Accessor::DST_BPP;
        } while (--w);

        P_FETCH_SPAN8_HOLE(
        {
          pos = Helpers::p_repeat_integer(pos + xx * hole, len);
        })
      P_FETCH_SPAN8_END()
    }

    // ------------------------------------------------------------------------
    // [Solid]
    // ------------------------------------------------------------------------

    else
    {
      typename Accessor::Pixel c0;
      accessor.fetchRaw(c0, pos >> 16);

      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()
        do {
          accessor.storePix(dst, c0);
          dst += Accessor::DST_BPP;
        } while (--w);
        P_FETCH_SPAN8_NEXT()
      P_FETCH_SPAN8_END()
    }

    // ------------------------------------------------------------------------
    // [Advance]
    // ------------------------------------------------------------------------

    fetcher->_d.gradient.linear.simple.pt += fetcher->_d.gradient.linear.simple.dt;
  }

  // ==========================================================================
  // [Fetch - Simple - Reflect]
  // ==========================================================================

  template<typename Accessor>
  static void FOG_FASTCALL fetch_simple_nearest_reflect(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    const RasterPattern* ctx = fetcher->getContext();
    Accessor accessor(ctx);

    P_FETCH_SPAN8_INIT()

    int xx = ctx->_d.gradient.linear.simple.xx16x16;
    int len = ctx->_d.gradient.base.len16x16;
    int len2 = len * 2;

    int pos = Helpers::p_repeat_integer(
      Math::fixed16x16FromFloat(fetcher->_d.gradient.linear.simple.pt) + x * xx, len2);

    if (pos > len)
    {
      pos = len2 - pos;
      xx = -xx;
    }

    // ------------------------------------------------------------------------
    // [Forward / Backward Direction]
    // ------------------------------------------------------------------------

    if (xx != 0)
    {
      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()

        if (xx > 0)
        {
          do {
            accessor.storeRaw(dst, pos >> 16);
            if ((pos += xx) >= len) {
              pos = len2 - pos;
              xx = -xx;
              goto _Backward;
            }
_Forward:
            dst += Accessor::DST_BPP;
          } while (--w);
        }
        else
        {
          do {
            accessor.storeRaw(dst, pos >> 16);
            if ((pos += xx) < 0)
            {
              pos = -pos;
              xx = -xx;
              goto _Forward;
            }
_Backward:
            dst += Accessor::DST_BPP;
          } while (--w);
        }

        P_FETCH_SPAN8_HOLE(
        {
          pos = Helpers::p_repeat_integer(pos + xx * hole, len2);
          if (pos > len)
          {
            pos = len2 - pos;
            xx = -xx;
          }
        })
      P_FETCH_SPAN8_END()
    }

    // ------------------------------------------------------------------------
    // [Solid]
    // ------------------------------------------------------------------------

    else
    {
      typename Accessor::Pixel c0;
      accessor.fetchRaw(c0, pos >> 16);

      P_FETCH_SPAN8_BEGIN()
        P_FETCH_SPAN8_SET_CURRENT()
        do {
          accessor.storePix(dst, c0);
          dst += Accessor::DST_BPP;
        } while (--w);
        P_FETCH_SPAN8_NEXT()
      P_FETCH_SPAN8_END()
    }

    // ------------------------------------------------------------------------
    // [Advance]
    // ------------------------------------------------------------------------

    fetcher->_d.gradient.linear.simple.pt += fetcher->_d.gradient.linear.simple.dt;
  }

  // ==========================================================================
  // [Fetch - Projection]
  // ==========================================================================

  template<typename Accessor>
  static void FOG_FASTCALL fetch_proj_nearest(
    RasterPatternFetcher* fetcher, RasterSpan* span, uint8_t* buffer)
  {
    const RasterPattern* ctx = fetcher->getContext();
    Accessor accessor(ctx);

    P_FETCH_SPAN8_INIT()

    double off = ctx->_d.gradient.linear.proj.offset;
    double xx = ctx->_d.gradient.linear.proj.xx;
    double zx = ctx->_d.gradient.linear.proj.zx;

    P_FETCH_SPAN8_BEGIN()
      P_FETCH_SPAN8_SET_CURRENT()

      double _x = (double)x;
      double px = fetcher->_d.gradient.linear.proj.pt + _x * xx;
      double pz = fetcher->_d.gradient.linear.proj.pz + _x * zx;

      do {
        typename Accessor::Pixel c0;
        accessor.fetchAtD(c0, off + px / pz);
        accessor.storePix(dst, c0);

        dst += Accessor::DST_BPP;
        px += xx;
        pz += zx;
      } while (--w);

      P_FETCH_SPAN8_NEXT()
    P_FETCH_SPAN8_END()

    // ------------------------------------------------------------------------
    // [Advance]
    // ------------------------------------------------------------------------

    fetcher->_d.gradient.linear.proj.pt += fetcher->_d.gradient.linear.proj.dt;
    fetcher->_d.gradient.linear.proj.pz += fetcher->_d.gradient.linear.proj.dz;
  }

  // ==========================================================================
  // [Skip]
  // ==========================================================================

  static void FOG_FASTCALL skip_simple(
    RasterPatternFetcher* fetcher, int step)
  {
    double s = (double)step;

    fetcher->_d.gradient.linear.simple.pt += fetcher->_d.gradient.linear.simple.dt * s;
  }

  static void FOG_FASTCALL skip_proj(
    RasterPatternFetcher* fetcher, int step)
  {
    double s = (double)step;

    fetcher->_d.gradient.linear.proj.pt += fetcher->_d.gradient.linear.proj.dt * s;
    fetcher->_d.gradient.linear.proj.pz += fetcher->_d.gradient.linear.proj.dz * s;
  }
};

} // RasterOps_C namespace
} // Fog namespace

// [Guard]
#endif // _FOG_G2D_PAINTING_RASTEROPS_C_GRADIENTLINEAR_P_H
