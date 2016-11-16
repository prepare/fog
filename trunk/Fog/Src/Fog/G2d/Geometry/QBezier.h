// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_GEOMETRY_QBEZIER_H
#define _FOG_G2D_GEOMETRY_QBEZIER_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/Core/Math/Fuzzy.h>
#include <Fog/Core/Math/Math.h>
#include <Fog/Core/Memory/MemOps.h>
#include <Fog/G2d/Geometry/Box.h>
#include <Fog/G2d/Geometry/Math2d.h>
#include <Fog/G2d/Geometry/Point.h>
#include <Fog/G2d/Geometry/Rect.h>

namespace Fog {

//! @addtogroup Fog_G2d_Geometry
//! @{

// ============================================================================
// [Fog::QBezierF]
// ============================================================================

struct FOG_NO_EXPORT QBezierF
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE QBezierF() {}

  FOG_INLINE QBezierF(const PointF& p0, const PointF& p1, const PointF& p2)
  {
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
  }

  FOG_INLINE QBezierF(float x0, float y0, float x1, float y1, float x2, float y2)
  {
    p[0].set(x0, y0);
    p[1].set(x1, y1);
    p[2].set(x2, y2);
  }

  explicit FOG_INLINE QBezierF(_Uninitialized) {}

  explicit FOG_INLINE QBezierF(const PointF* pts)
  {
    p[0] = pts[0];
    p[1] = pts[1];
    p[2] = pts[2];
  }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE float getX0() const { return p[0].x; }
  FOG_INLINE float getY0() const { return p[0].y; }
  FOG_INLINE float getX1() const { return p[1].x; }
  FOG_INLINE float getY1() const { return p[1].y; }
  FOG_INLINE float getX2() const { return p[2].x; }
  FOG_INLINE float getY2() const { return p[2].y; }

  FOG_INLINE void setX0(float v) { p[0].x = v; }
  FOG_INLINE void setY0(float v) { p[0].y = v; }
  FOG_INLINE void setX1(float v) { p[1].x = v; }
  FOG_INLINE void setY1(float v) { p[1].y = v; }
  FOG_INLINE void setX2(float v) { p[2].x = v; }
  FOG_INLINE void setY2(float v) { p[2].y = v; }

  // --------------------------------------------------------------------------
  // [GetLength]
  // --------------------------------------------------------------------------

  FOG_INLINE float getLength() const
  {
    float length;
    fog_api.qbezierf_getLength(p, &length);
    return length;
  }

  // --------------------------------------------------------------------------
  // [GetClosestPoint]
  // --------------------------------------------------------------------------

  FOG_INLINE float getClosestPoint(PointF& dst, const PointF& pt) const
  {
    return fog_api.qbezierf_getClosestPoint(p, &dst, &pt);
  }

  // --------------------------------------------------------------------------
  // [Reset]
  // --------------------------------------------------------------------------

  FOG_INLINE void reset()
  {
    p[0].reset();
    p[1].reset();
    p[2].reset();
  }

  // --------------------------------------------------------------------------
  // [Evaluate]
  // --------------------------------------------------------------------------

  FOG_INLINE void evaluate(PointF& pt, float t) const
  {
    evaluate(p, &pt, t);
  }

  // --------------------------------------------------------------------------
  // [BoundingBox / BoundingRect]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t getBoundingBox(BoxF& dst) const
  {
    return fog_api.qbezierf_getBoundingBox(p, &dst);
  }

  FOG_INLINE err_t getBoundingRect(RectF& dst) const
  {
    err_t err = fog_api.qbezierf_getBoundingBox(p, reinterpret_cast<BoxF*>(&dst));
    dst.w -= dst.x;
    dst.h -= dst.y;
    return err;
  }

  // --------------------------------------------------------------------------
  // [Transform]
  // --------------------------------------------------------------------------

  FOG_INLINE void translate(const PointF& pt)
  {
    p[0] += pt;
    p[1] += pt;
    p[2] += pt;
  }

  // --------------------------------------------------------------------------
  // [Flatten]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t flatten(PathF& dst, uint8_t initialCommand, float flatness) const
  {
    return fog_api.qbezierf_flatten(p, &dst, initialCommand, flatness);
  }

  // --------------------------------------------------------------------------
  // [Operator Overload]
  // --------------------------------------------------------------------------

  FOG_INLINE QBezierF& operator=(const QBezierF& other)
  {
    p[0] = other.p[0];
    p[1] = other.p[1];
    p[2] = other.p[2];
    return *this;
  }

  FOG_INLINE bool operator==(const QBezierF& other) const { return  MemOps::eq_t<QBezierF>(this, &other); }
  FOG_INLINE bool operator!=(const QBezierF& other) const { return !MemOps::eq_t<QBezierF>(this, &other); }

  FOG_INLINE const PointF& operator[](size_t i) const
  {
    FOG_ASSERT_X(i < 3,
                 "Fog::QBezierF::operator[] - Index out of range");
    
    return p[i];
  }
  
  FOG_INLINE PointF& operator[](size_t i)
  {
    FOG_ASSERT_X(i < 3,
                 "Fog::QBezierF::operator[] - Index out of range");
    
    return p[i];
  }

  // --------------------------------------------------------------------------
  // [Statics]
  // --------------------------------------------------------------------------

  static FOG_INLINE void evaluate(const QBezierF* self, PointF* dst, float t)
  {
    evaluate(self->p, dst, t);
  }

  static FOG_INLINE void evaluate(const PointF* self, PointF* dst, float t)
  {
    float tInv = 1.0f - t;

    float a = tInv * tInv;
    float b = t * tInv * 2.0f;
    float c = t * t;

    dst->x = a * self[0].x + b * self[1].x + c * self[2].x;
    dst->y = a * self[0].y + b * self[1].y + c * self[2].y;
  }

  static FOG_INLINE err_t getBoundingBox(const QBezierF* self, BoxF* dst)
  {
    return fog_api.qbezierf_getBoundingBox(self->p, dst);
  }

  static FOG_INLINE err_t getBoundingBox(const PointF* self, BoxF* dst)
  {
    return fog_api.qbezierf_getBoundingBox(self, dst);
  }

  static FOG_INLINE err_t getSplineBBox(const PointF* self, size_t length, BoxF* dst)
  {
    return fog_api.qbezierf_getSplineBBox(self, length, dst);
  }

  static FOG_INLINE void splitHalf(const PointF* self, PointF* left, PointF* rght)
  {
    PointF p01 = Math2d::half(self[0], self[1]);
    PointF p12 = Math2d::half(self[1], self[2]);

    left[0] = self[0];
    left[1] = p01;
    rght[1] = p12;
    rght[2] = self[2];

    left[2] = Math2d::half(p01, p12);
    rght[0] = left[2];
  }

  static FOG_INLINE void splitHalf(const QBezierF* self, QBezierF* left, QBezierF* rght)
  {
    splitHalf(self->p, left->p, rght->p);
  }

  static FOG_INLINE void splitAt(const PointF* self, PointF* left, PointF* rght, float t)
  {
    float inv_t = (1.0f - t);

    PointF p01 = Math2d::lerp(self[0], self[1], t, inv_t);
    PointF p12 = Math2d::lerp(self[1], self[2], t, inv_t);

    left[0] = self[0];
    left[1] = p01;
    rght[1] = p12;
    rght[2] = self[2];

    left[2] = Math2d::lerp(p01, p12, t, inv_t);
    rght[0] = left[2];
  }

  static FOG_INLINE void splitAt(const QBezierF* self, QBezierF* left, QBezierF* rght, float t)
  {
    splitAt(self->p, left->p, rght->p, t);
  }

  static FOG_INLINE err_t flatten(const QBezierF* self, PathF& dst, uint8_t initialCommand, float flatness)
  {
    return fog_api.qbezierf_flatten(self->p, &dst, initialCommand, flatness);
  }

  static FOG_INLINE err_t flatten(const PointF* self, PathF& dst, uint8_t initialCommand, float flatness)
  {
    return fog_api.qbezierf_flatten(self, &dst, initialCommand, flatness);
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  PointF p[3];
};

// ============================================================================
// [Fog::QBezierD]
// ============================================================================

struct FOG_NO_EXPORT QBezierD
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE QBezierD() {}

  FOG_INLINE QBezierD(const PointD& p0, const PointD& p1, const PointD& p2)
  {
    p[0] = p0;
    p[1] = p1;
    p[2] = p2;
  }

  FOG_INLINE QBezierD(double x0, double y0, double x1, double y1, double x2, double y2)
  {
    p[0].set(x0, y0);
    p[1].set(x1, y1);
    p[2].set(x2, y2);
  }

  explicit FOG_INLINE QBezierD(_Uninitialized) {}

  explicit FOG_INLINE QBezierD(const PointD* pts)
  {
    p[0] = pts[0];
    p[1] = pts[1];
    p[2] = pts[2];
  }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE double getX0() const { return p[0].x; }
  FOG_INLINE double getY0() const { return p[0].y; }
  FOG_INLINE double getX1() const { return p[1].x; }
  FOG_INLINE double getY1() const { return p[1].y; }
  FOG_INLINE double getX2() const { return p[2].x; }
  FOG_INLINE double getY2() const { return p[2].y; }

  FOG_INLINE void setX0(double v) { p[0].x = v; }
  FOG_INLINE void setY0(double v) { p[0].y = v; }
  FOG_INLINE void setX1(double v) { p[1].x = v; }
  FOG_INLINE void setY1(double v) { p[1].y = v; }
  FOG_INLINE void setX2(double v) { p[2].x = v; }
  FOG_INLINE void setY2(double v) { p[2].y = v; }

  // --------------------------------------------------------------------------
  // [GetLength]
  // --------------------------------------------------------------------------

  FOG_INLINE double getLength() const
  {
    double length;
    fog_api.qbezierd_getLength(p, &length);
    return length;
  }

  // --------------------------------------------------------------------------
  // [GetClosestPoint]
  // --------------------------------------------------------------------------

  FOG_INLINE double getClosestPoint(PointD& dst, const PointD& pt) const
  {
    return fog_api.qbezierd_getClosestPoint(p, &dst, &pt);
  }

  // --------------------------------------------------------------------------
  // [Reset]
  // --------------------------------------------------------------------------

  FOG_INLINE void reset()
  {
    p[0].reset();
    p[1].reset();
    p[2].reset();
  }

  // --------------------------------------------------------------------------
  // [Evaluate]
  // --------------------------------------------------------------------------

  FOG_INLINE void evaluate(PointD& pt, double t) const
  {
    evaluate(p, &pt, t);
  }

  // --------------------------------------------------------------------------
  // [BoundingBox / BoundingRect]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t getBoundingBox(BoxD& dst) const
  {
    return fog_api.qbezierd_getBoundingBox(p, &dst);
  }

  FOG_INLINE err_t getBoundingRect(RectD& dst) const
  {
    err_t err = fog_api.qbezierd_getBoundingBox(p, reinterpret_cast<BoxD*>(&dst));
    dst.w -= dst.x;
    dst.h -= dst.y;
    return err;
  }

  // --------------------------------------------------------------------------
  // [Transform]
  // --------------------------------------------------------------------------

  FOG_INLINE void translate(const PointD& pt)
  {
    p[0] += pt;
    p[1] += pt;
    p[2] += pt;
  }

  // --------------------------------------------------------------------------
  // [Flatten]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t flatten(PathD& dst, uint8_t initialCommand, double flatness) const
  {
    return fog_api.qbezierd_flatten(p, &dst, initialCommand, flatness);
  }

  // --------------------------------------------------------------------------
  // [Operator Overload]
  // --------------------------------------------------------------------------

  FOG_INLINE QBezierD& operator=(const QBezierD& other)
  {
    p[0] = other.p[0];
    p[1] = other.p[1];
    p[2] = other.p[2];
    return *this;
  }

  FOG_INLINE bool operator==(const QBezierD& other) const { return  MemOps::eq_t<QBezierD>(this, &other); }
  FOG_INLINE bool operator!=(const QBezierD& other) const { return !MemOps::eq_t<QBezierD>(this, &other); }

  FOG_INLINE const PointD& operator[](size_t i) const
  {
    FOG_ASSERT_X(i < 3,
                 "Fog::QBezierD::operator[] - Index out of range");
    
    return p[i];
  }
  
  FOG_INLINE PointD& operator[](size_t i)
  {
    FOG_ASSERT_X(i < 3,
                 "Fog::QBezierD::operator[] - Index out of range");
    
    return p[i];
  }

  // --------------------------------------------------------------------------
  // [Statics]
  // --------------------------------------------------------------------------

  static FOG_INLINE void evaluate(const QBezierD* self, PointD* dst, double t)
  {
    evaluate(self->p, dst, t);
  }

  static FOG_INLINE void evaluate(const PointD* self, PointD* dst, double t)
  {
    double tInv = 1.0 - t;

    double a = tInv * tInv;
    double b = t * tInv * 2.0;
    double c = t * t;

    dst->x = a * self[0].x + b * self[1].x + c * self[2].x;
    dst->y = a * self[0].y + b * self[1].y + c * self[2].y;
  }

  static FOG_INLINE err_t getBoundingBox(const QBezierD* self, BoxD* dst)
  {
    return fog_api.qbezierd_getBoundingBox(self->p, dst);
  }

  static FOG_INLINE err_t getBoundingBox(const PointD* self, BoxD* dst)
  {
    return fog_api.qbezierd_getBoundingBox(self, dst);
  }

  static FOG_INLINE err_t getSplineBBox(const PointD* self, size_t length, BoxD* dst)
  {
    return fog_api.qbezierd_getSplineBBox(self, length, dst);
  }

  static FOG_INLINE void splitHalf(const PointD* self, PointD* left, PointD* rght)
  {
    PointD p01 = Math2d::half(self[0], self[1]);
    PointD p12 = Math2d::half(self[1], self[2]);

    left[0] = self[0];
    left[1] = p01;
    rght[1] = p12;
    rght[2] = self[2];

    left[2] = Math2d::half(p01, p12);
    rght[0] = left[2];
  }

  static FOG_INLINE void splitHalf(const QBezierD* self, QBezierD* left, QBezierD* rght)
  {
    splitHalf(self->p, left->p, rght->p);
  }

  static FOG_INLINE void splitAt(const PointD* self, PointD* left, PointD* rght, double t)
  {
    double inv_t = (1.0 - t);

    PointD p01 = Math2d::lerp(self[0], self[1], t, inv_t);
    PointD p12 = Math2d::lerp(self[1], self[2], t, inv_t);

    left[0] = self[0];
    left[1] = p01;
    rght[1] = p12;
    rght[2] = self[2];

    left[2] = Math2d::lerp(p01, p12, t, inv_t);
    rght[0] = left[2];
  }

  static FOG_INLINE void splitAt(const QBezierD* self, QBezierD* left, QBezierD* rght, double t)
  {
    splitAt(self->p, left->p, rght->p, t);
  }

  static FOG_INLINE err_t flatten(const QBezierD* self, PathD& dst, uint8_t initialCommand, double flatness)
  {
    return fog_api.qbezierd_flatten(self->p, &dst, initialCommand, flatness);
  }

  static FOG_INLINE err_t flatten(const PointD* self, PathD& dst, uint8_t initialCommand, double flatness)
  {
    return fog_api.qbezierd_flatten(self, &dst, initialCommand, flatness);
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  PointD p[3];
};

// ============================================================================
// [Fog::QBezierT<>]
// ============================================================================

_FOG_NUM_T(QBezier)
_FOG_NUM_F(QBezier)
_FOG_NUM_D(QBezier)

//! @}

} // Fog namespace

// ============================================================================
// [Fog::Fuzzy<>]
// ============================================================================

FOG_FUZZY_DECLARE_F_VEC(Fog::QBezierF, 6)
FOG_FUZZY_DECLARE_D_VEC(Fog::QBezierD, 6)

// [Guard]
#endif // _FOG_G2D_GEOMETRY_QBEZIER_H
