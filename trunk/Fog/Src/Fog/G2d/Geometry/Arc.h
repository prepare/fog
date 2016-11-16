// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_GEOMETRY_ARC_H
#define _FOG_G2D_GEOMETRY_ARC_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Math/Fuzzy.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/G2d/Geometry/Box.h>
#include <Fog/G2d/Geometry/Ellipse.h>
#include <Fog/G2d/Geometry/Point.h>
#include <Fog/G2d/Geometry/Rect.h>

namespace Fog {

//! @addtogroup Fog_G2d_Geometry
//! @{

// ============================================================================
// [Fog::ArcF]
// ============================================================================

//! @brief Arc (float).
struct FOG_NO_EXPORT ArcF
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE ArcF() { reset(); }
  FOG_INLINE ArcF(const ArcF& other) { setArc(other); }

  FOG_INLINE ArcF(const PointF& cp, float rad, float start_, float sweep_)
  {
    setEllipse(cp, rad);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE ArcF(const PointF& cp, const PointF& rp, float start_, float sweep_)
  {
    setEllipse(cp, rp);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE ArcF(const RectF& r, float start_, float sweep_)
  {
    setEllipse(r);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE ArcF(const BoxF& r, float start_, float sweep_)
  {
    setEllipse(r);
    start = start_;
    sweep = sweep_;
  }

  explicit FOG_INLINE ArcF(_Uninitialized) {}
  explicit FOG_INLINE ArcF(const ArcD& other) { setArc(other); }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE const PointF& getCenter() const { return center; }
  FOG_INLINE const PointF& getRadius() const { return radius; }
  FOG_INLINE float getStart() const { return start; }
  FOG_INLINE float getSweep() const { return sweep; }

  FOG_INLINE void setCenter(const PointF& cp) { center = cp; }
  FOG_INLINE void setRadius(const PointF& rp) { radius = rp; }
  FOG_INLINE void setRadius(float r) { radius.set(r, r); }
  FOG_INLINE void setRadius(float rx, float ry) { radius.set(rx, ry); }

  FOG_INLINE void setEllipse(const CircleF& circle) { center = circle.center; radius.set(circle.radius, circle.radius); }
  FOG_INLINE void setEllipse(const EllipseF& ellipse) { center = ellipse.center; radius = ellipse.radius; }

  FOG_INLINE void setEllipse(const PointF& cp, float rad) { center = cp; radius.set(rad, rad); }
  FOG_INLINE void setEllipse(const PointF& cp, const PointF& rp) { center = cp; radius = rp; }

  FOG_INLINE void setEllipse(const RectF& r) { radius.set(r.w * 0.5f, r.h * 0.5f); center.set(r.x + radius.x, r.y + radius.y); }
  FOG_INLINE void setEllipse(const BoxF& r) { radius.set(r.getWidth() * 0.5f, r.getHeight() * 0.5f); center.set(r.x0 + radius.x, r.y0 + radius.y); }

  FOG_INLINE void setStart(float start_) { start = start_; }
  FOG_INLINE void setSweep(float sweep_) { sweep = sweep_; }

  FOG_INLINE void setArc(const ArcF& other)
  {
    center = other.center;
    radius = other.radius;
    start = other.start;
    sweep = other.sweep;
  }

  FOG_INLINE void setArc(const ArcD& other);

  FOG_INLINE void setArc(const PointF& cp, float rad, float start_, float sweep_)
  {
    center = cp;
    radius.set(rad, rad);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE void setArc(const PointF& cp, const PointF& rad, float start_, float sweep_)
  {
    center = cp;
    radius = rad;
    start = start_;
    sweep = sweep_;
  }

  // --------------------------------------------------------------------------
  // [Reset]
  // --------------------------------------------------------------------------

  FOG_INLINE void reset()
  {
    center.reset();
    radius.reset();
    start = 0.0f;
    sweep = 0.0f;
  }

  // --------------------------------------------------------------------------
  // [BoundingBox / BoundingRect]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t getBoundingBox(BoxF& dst) const
  {
    return _getBoundingBox(dst, NULL);
  }

  FOG_INLINE err_t getBoundingBox(BoxF& dst, const TransformF& tr) const
  {
    return _getBoundingBox(dst, &tr);
  }

  FOG_INLINE err_t getBoundingRect(RectF& dst) const
  {
    return _getBoundingRect(dst, NULL);
  }

  FOG_INLINE err_t getBoundingRect(RectF& dst, const TransformF& tr) const
  {
    return _getBoundingRect(dst, &tr);
  }

  FOG_INLINE err_t _getBoundingBox(BoxF& dst, const TransformF* tr) const
  {
    return fog_api.arcf_getBoundingBox(this, &dst, tr, false);
  }

  FOG_INLINE err_t _getBoundingRect(RectF& dst, const TransformF* tr) const
  {
    err_t err = fog_api.arcf_getBoundingBox(this, reinterpret_cast<BoxF*>(&dst), tr, false);
    dst.w -= dst.x;
    dst.h -= dst.y;
    return err;
  }

  // --------------------------------------------------------------------------
  // [Transform]
  // --------------------------------------------------------------------------

  FOG_INLINE void translate(const PointF& pt)
  {
    center += pt;
  }

  // --------------------------------------------------------------------------
  // [ToCSpline]
  // --------------------------------------------------------------------------

  FOG_INLINE uint toCSpline(PointF* dst) const
  {
    return fog_api.arcf_toCSpline(this, dst);
  }

  // --------------------------------------------------------------------------
  // [Operator Overload]
  // --------------------------------------------------------------------------

  FOG_INLINE ArcF& operator=(const ArcF& other) { setArc(other); return *this; }
  FOG_INLINE ArcF& operator=(const ArcD& other) { setArc(other); return *this; }

  FOG_INLINE bool operator==(const ArcF& other) const { return  MemOps::eq_t<ArcF>(this, &other); }
  FOG_INLINE bool operator!=(const ArcF& other) const { return !MemOps::eq_t<ArcF>(this, &other); }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  PointF center;
  PointF radius;
  float start;
  float sweep;
};

// ============================================================================
// [Fog::ArcD]
// ============================================================================

//! @brief Arc base (double).
struct FOG_NO_EXPORT ArcD
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE ArcD() { reset(); }
  FOG_INLINE ArcD(const ArcD& other) { setArc(other); }

  FOG_INLINE ArcD(const PointD& cp, double rad, double start_, double sweep_)
  {
    setEllipse(cp, rad);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE ArcD(const PointD& cp, const PointD& rad, double start_, double sweep_)
  {
    setEllipse(cp, rad);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE ArcD(const RectD& r, double start_, double sweep_)
  {
    setEllipse(r);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE ArcD(const BoxD& r, double start_, double sweep_)
  {
    setEllipse(r);
    start = start_;
    sweep = sweep_;
  }

  explicit FOG_INLINE ArcD(_Uninitialized) {}
  explicit FOG_INLINE ArcD(const ArcF& other) { setArc(other); }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE const PointD& getCenter() const { return center; }
  FOG_INLINE const PointD& getRadius() const { return radius; }
  FOG_INLINE double getStart() const { return start; }
  FOG_INLINE double getSweep() const { return sweep; }

  FOG_INLINE void setCenter(const PointD& cp) { center = cp; }
  FOG_INLINE void setRadius(const PointD& rp) { radius = rp; }
  FOG_INLINE void setRadius(double r) { radius.set(r, r); }
  FOG_INLINE void setRadius(double rx, double ry) { radius.set(rx, ry); }

  FOG_INLINE void setEllipse(const CircleD& circle) { center = circle.center; radius.set(circle.radius, circle.radius); }
  FOG_INLINE void setEllipse(const EllipseD& ellipse) { center = ellipse.center; radius = ellipse.radius; }

  FOG_INLINE void setEllipse(const PointD& cp, double rad) { center = cp; radius.set(rad, rad); }
  FOG_INLINE void setEllipse(const PointD& cp, const PointD& rp) { center = cp; radius = rp; }

  FOG_INLINE void setEllipse(const RectD& r) { radius.set(r.w * 0.5, r.h * 0.5); center.set(r.x + radius.x, r.y + radius.y); }
  FOG_INLINE void setEllipse(const BoxD& r) { radius.set(r.getWidth() * 0.5, r.getHeight() * 0.5); center.set(r.x0 + radius.x, r.y0 + radius.y); }

  FOG_INLINE void setStart(double start_) { start = start_; }
  FOG_INLINE void setSweep(double sweep_) { sweep = sweep_; }

  FOG_INLINE void setArc(const ArcD& other)
  {
    center = other.center;
    radius = other.radius;
    start = other.start;
    sweep = other.sweep;
  }

  FOG_INLINE void setArc(const ArcF& other)
  {
    center = other.center;
    radius = other.radius;
    start = other.start;
    sweep = other.sweep;
  }

  FOG_INLINE void setArc(const PointD& cp, double rad, double start_, double sweep_)
  {
    center = cp;
    radius.set(rad, rad);
    start = start_;
    sweep = sweep_;
  }

  FOG_INLINE void setArc(const PointD& cp, const PointD& rad, double start_, double sweep_)
  {
    center = cp;
    radius = rad;
    start = start_;
    sweep = sweep_;
  }

  // --------------------------------------------------------------------------
  // [Reset]
  // --------------------------------------------------------------------------

  FOG_INLINE void reset()
  {
    center.reset();
    radius.reset();
    start = 0.0;
    sweep = 0.0;
  }

  // --------------------------------------------------------------------------
  // [BoundingBox / BoundingRect]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t getBoundingBox(BoxD& dst) const
  {
    return _getBoundingBox(dst, NULL);
  }

  FOG_INLINE err_t getBoundingBox(BoxD& dst, const TransformD& tr) const
  {
    return _getBoundingBox(dst, &tr);
  }

  FOG_INLINE err_t getBoundingRect(RectD& dst) const
  {
    return _getBoundingRect(dst, NULL);
  }

  FOG_INLINE err_t getBoundingRect(RectD& dst, const TransformD& tr) const
  {
    return _getBoundingRect(dst, &tr);
  }

  FOG_INLINE err_t _getBoundingBox(BoxD& dst, const TransformD* tr) const
  {
    return fog_api.arcd_getBoundingBox(this, &dst, tr, false);
  }

  FOG_INLINE err_t _getBoundingRect(RectD& dst, const TransformD* tr) const
  {
    err_t err = fog_api.arcd_getBoundingBox(this, reinterpret_cast<BoxD*>(&dst), tr, false);
    dst.w -= dst.x;
    dst.h -= dst.y;
    return err;
  }

  // --------------------------------------------------------------------------
  // [Transform]
  // --------------------------------------------------------------------------

  FOG_INLINE void translate(const PointD& pt)
  {
    center += pt;
  }

  // --------------------------------------------------------------------------
  // [ToCSpline]
  // --------------------------------------------------------------------------

  FOG_INLINE uint toCSpline(PointD* dst) const
  {
    return fog_api.arcd_toCSpline(this, dst);
  }

  // --------------------------------------------------------------------------
  // [Operator Overload]
  // --------------------------------------------------------------------------

  FOG_INLINE ArcD& operator=(const ArcD& other) { setArc(other); return *this; }
  FOG_INLINE ArcD& operator=(const ArcF& other) { setArc(other); return *this; }

  FOG_INLINE bool operator==(const ArcD& other) const { return  MemOps::eq_t<ArcD>(this, &other); }
  FOG_INLINE bool operator!=(const ArcD& other) const { return !MemOps::eq_t<ArcD>(this, &other); }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  PointD center;
  PointD radius;
  double start;
  double sweep;
};

// ============================================================================
// [Implemented-Later]
// ============================================================================

FOG_INLINE void ArcF::setArc(const ArcD& other)
{
  center = other.center;
  radius = other.radius;
  start = float(other.start);
  sweep = float(other.sweep);
}

// ============================================================================
// [Fog::ArcT<>]
// ============================================================================

_FOG_NUM_T(Arc)
_FOG_NUM_F(Arc)
_FOG_NUM_D(Arc)

//! @}

} // Fog namespace

// ============================================================================
// [Fog::Fuzzy<>]
// ============================================================================

FOG_FUZZY_DECLARE_F_VEC(Fog::ArcF, 6)
FOG_FUZZY_DECLARE_D_VEC(Fog::ArcD, 6)

// [Guard]
#endif // _FOG_G2D_GEOMETRY_ARC_H
