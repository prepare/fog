// [Fog-G2d]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_G2D_IMAGING_FILTERS_FECOLORMATRIX_H
#define _FOG_G2D_IMAGING_FILTERS_FECOLORMATRIX_H

// [Dependencies]
#include <Fog/Core/Global/Global.h>
#include <Fog/G2d/Imaging/Filters/FeBase.h>

namespace Fog {

//! @addtogroup Fog_G2d_Imaging
//! @{

// ============================================================================
// [Fog::FeColorMatrix]
// ============================================================================

//! @brief Color matrix.
//!
//! The color matrix is a 5x5 matrix that can be used to do a linear color
//! transformation.
struct FOG_NO_EXPORT FeColorMatrix : public FeBase
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  FOG_INLINE FeColorMatrix()
  {
    fog_api.fecolormatrix_ctor(this);
  }

  FOG_INLINE FeColorMatrix(const float data[25])
  {
    _feType = FE_TYPE_COLOR_MATRIX;
    fog_api.fecolormatrix_copy(m, data);
  }

  FOG_INLINE FeColorMatrix(const FeColorMatrix& other)
  {
    _feType = FE_TYPE_COLOR_MATRIX;
    fog_api.fecolormatrix_copy(m, other.m);
  }

  FOG_INLINE FeColorMatrix(
    float m00, float m01, float m02, float m03, float m04,
    float m10, float m11, float m12, float m13, float m14,
    float m20, float m21, float m22, float m23, float m24,
    float m30, float m31, float m32, float m33, float m34,
    float m40, float m41, float m42, float m43, float m44)
  {
    m[ 0] = m00; m[ 1] = m01; m[ 2] = m02; m[ 3] = m03; m[ 4] = m04;
    m[ 5] = m10; m[ 6] = m11; m[ 7] = m12; m[ 8] = m13; m[ 9] = m14;
    m[10] = m20; m[11] = m21; m[12] = m22; m[13] = m23; m[14] = m24;
    m[15] = m30; m[16] = m31; m[17] = m32; m[18] = m33; m[19] = m34;
    m[20] = m40; m[21] = m41; m[22] = m42; m[23] = m43; m[24] = m44;
  }

  explicit FOG_INLINE FeColorMatrix(_Uninitialized)
  {
    _feType = FE_TYPE_COLOR_MATRIX;
  }

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE float* getData()
  {
    return reinterpret_cast<float*>(m);
  }

  FOG_INLINE const float* getData() const
  {
    return reinterpret_cast<const float*>(m);
  }

  //! @brief Get the type of the color matrix.
  //!
  //! Type of color matrix is important part of optimization that can be done
  //! by blitters. The main advantage of color matrix class is that many color
  //! operations can be defined by it. But these operations usually do not use
  //! all matrix cells, so the getType() method checks for common values and
  //! returns type which can be optimized by image-filter.
  //!
  //! @see @c Type for type possibilities and its descriptions.
  FOG_INLINE uint32_t getType() const
  {
    return fog_api.fecolormatrix_getType(this);
  }

  // --------------------------------------------------------------------------
  // [Set]
  // --------------------------------------------------------------------------

  FOG_INLINE FeColorMatrix& setMatrix(const float data[25])
  {
    fog_api.fecolormatrix_copy(m, data);
    return *this;
  }

  FOG_INLINE FeColorMatrix& setMatrix(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_copy(m, other.m);
    return *this;
  }

  FOG_INLINE FeColorMatrix& setMatrix(
    float m00, float m01, float m02, float m03, float m04,
    float m10, float m11, float m12, float m13, float m14,
    float m20, float m21, float m22, float m23, float m24,
    float m30, float m31, float m32, float m33, float m34,
    float m40, float m41, float m42, float m43, float m44)
  {
    m[ 0] = m00; m[ 1] = m01; m[ 2] = m02; m[ 3] = m03; m[ 4] = m04;
    m[ 5] = m10; m[ 6] = m11; m[ 7] = m12; m[ 8] = m13; m[ 9] = m14;
    m[10] = m20; m[11] = m21; m[12] = m22; m[13] = m23; m[14] = m24;
    m[15] = m30; m[16] = m31; m[17] = m32; m[18] = m33; m[19] = m34;
    m[20] = m40; m[21] = m41; m[22] = m42; m[23] = m43; m[24] = m44;

    return *this;
  }

  // --------------------------------------------------------------------------
  // [Add]
  // --------------------------------------------------------------------------

  //! @brief Add other matrix into this matrix.
  FOG_INLINE FeColorMatrix& add(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_addMatrix(this, this, &other);
    return *this;
  }

  FOG_INLINE FeColorMatrix& add(float scalar)
  {
    fog_api.fecolormatrix_addScalar(this, this, NULL, scalar);
    return *this;
  }

  FOG_INLINE FeColorMatrix& add(const RectI& rect, float scalar)
  {
    fog_api.fecolormatrix_addScalar(this, this, &rect, scalar);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Subtract]
  // --------------------------------------------------------------------------

  //! @brief Subtract other matrix from this matrix.
  FOG_INLINE FeColorMatrix& subtract(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_subtractMatrix(this, this, &other);
    return *this;
  }

  FOG_INLINE FeColorMatrix& subtract(float scalar)
  {
    fog_api.fecolormatrix_subtractScalar(this, this, NULL, scalar);
    return *this;
  }

  FOG_INLINE FeColorMatrix& subtract(const RectI& rect, float scalar)
  {
    fog_api.fecolormatrix_subtractScalar(this, this, &rect, scalar);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Multiply]
  // --------------------------------------------------------------------------

  //! @brief Multiply this matrix with @a other matrix.
  FOG_INLINE FeColorMatrix& multiply(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_multiplyMatrix(this, &other, this);
    return *this;
  }

  //! @brief Multiply this matrix with @a other matrix.
  FOG_INLINE FeColorMatrix& multiply(const FeColorMatrix& other, uint32_t order)
  {
    fog_api.fecolormatrix_multiplyOther(this, &other, order);
    return *this;
  }

  //! @brief Multiply this matrix with scalar value.
  FOG_INLINE FeColorMatrix& multiply(float scalar)
  {
    fog_api.fecolormatrix_multiplyScalar(this, this, NULL, scalar);
    return *this;
  }

  //! @overload
  FOG_INLINE FeColorMatrix& multiply(const RectI& rect, float scalar)
  {
    fog_api.fecolormatrix_multiplyScalar(this, this, &rect, scalar);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Translate]
  // --------------------------------------------------------------------------

  //! @brief Translate the color components of the matrix.
  FOG_INLINE FeColorMatrix& translateArgb(float a, float r, float g, float b, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_translateArgb(this, a, r, g, b, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& translateRgb(float c, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_translateArgb(this, 1.0f, c, c, c, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& translateAlpha(float a, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_translateArgb(this, a, 1.0f, 1.0f, 1.0f, order);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Scale]
  // --------------------------------------------------------------------------

  //! @brief Scale the color components of the matrix.
  FOG_INLINE FeColorMatrix& scaleArgb(float a, float r, float g, float b, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_scaleArgb(this, a, r, g, b, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& scaleRgb(float c, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_scaleArgb(this, 1.0f, c, c, c, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& scaleAlpha(float a, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_scaleArgb(this, a, 1.0f, 1.0f, 1.0f, order);
    return *this;
  }

  //! @brief Scale the tint.
  //!
  //! @param phi [-PI to PI, 0 = blue]
  //! @param amount [-1.0 to 1.0, 0 = neutral]
  FOG_INLINE FeColorMatrix& scaleTint(float phi, float amount)
  {
    fog_api.fecolormatrix_scaleTint(this, phi, amount);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Rotate]
  // --------------------------------------------------------------------------

  FOG_INLINE FeColorMatrix& rotateRed(float phi, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_rotateColor(this, 2, 1, phi, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& rotateGreen(float phi, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_rotateColor(this, 0, 2, phi, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& rotateBlue(float phi, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_rotateColor(this, 1, 0, phi, order);
    return *this;
  }

  //! @brief Rotate the hue about the gray axis.
  //!
  //! Luminance is fixed, other colors are rotated.
  FOG_INLINE FeColorMatrix& rotateHue(float phi)
  {
    fog_api.fecolormatrix_rotateHue(this, phi);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Shear]
  // --------------------------------------------------------------------------

  FOG_INLINE FeColorMatrix& shearRed(float g, float b, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_shearColor(this, 0, 1, g, 2, b, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& shearGreen( float r, float b, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_shearColor(this, 1, 0, r, 2, b, order);
    return *this;
  }

  FOG_INLINE FeColorMatrix& shearBlue(float r, float g, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_shearColor(this, 2, 0, r, 1, g, order);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Saturate]
  // --------------------------------------------------------------------------

  //! @brief Saturate.
  //!
  //! Saturation of 0.0 yields black & white, 1.0 is neutral.
  FOG_INLINE FeColorMatrix& saturate(float s, uint32_t order = MATRIX_ORDER_PREPEND)
  {
    fog_api.fecolormatrix_saturate(this, s, order);
    return *this;
  }

  // --------------------------------------------------------------------------
  // [Equality]
  // --------------------------------------------------------------------------

  FOG_INLINE bool eq(const FeColorMatrix& other) const
  {
    return fog_api.fecolormatrix_eq(this, &other);
  }

  // --------------------------------------------------------------------------
  // [Map]
  // --------------------------------------------------------------------------

  //! @brief Map @a argb color.
  FOG_INLINE void mapArgb(Argb32& argb) const
  {
    return fog_api.fecolormatrix_mapArgb32(this, &argb, &argb);
  }

  //! @overload.
  FOG_INLINE void mapArgb(Argb32& dst, const Argb32& src) const
  {
    return fog_api.fecolormatrix_mapArgb32(this, &dst, &src);
  }

  //! @brief Map @a argb color.
  FOG_INLINE void mapArgb(Argb64& argb) const
  {
    return fog_api.fecolormatrix_mapArgb64(this, &argb, &argb);
  }

  //! @overload.
  FOG_INLINE void mapArgb(Argb64& dst, const Argb64& src) const
  {
    return fog_api.fecolormatrix_mapArgb64(this, &dst, &src);
  }

  //! @brief Map @a argb color.
  FOG_INLINE void mapArgb(ArgbF& argb) const
  {
    return fog_api.fecolormatrix_mapArgbF(this, &argb, &argb);
  }

  //! @overload.
  FOG_INLINE void mapArgb(ArgbF& dst, const ArgbF& src) const
  {
    return fog_api.fecolormatrix_mapArgbF(this, &dst, &src);
  }

  // --------------------------------------------------------------------------
  // [Operator Overload]
  // --------------------------------------------------------------------------

  //! @brief Assignment operator.
  FOG_INLINE FeColorMatrix& operator=(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_copy(m, other.m);
    return *this;
  }

  FOG_INLINE FeColorMatrix& operator+=(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_addMatrix(this, this, &other);
    return *this;
  }

  FOG_INLINE FeColorMatrix& operator+=(float scalar)
  {
    fog_api.fecolormatrix_addScalar(this, this, NULL, scalar);
    return *this;
  }

  FOG_INLINE FeColorMatrix& operator-=(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_subtractMatrix(this, this, &other);
    return *this;
  }

  FOG_INLINE FeColorMatrix& operator-=(float scalar)
  {
    fog_api.fecolormatrix_subtractScalar(this, this, NULL, scalar);
    return *this;
  }

  FOG_INLINE FeColorMatrix& operator*=(const FeColorMatrix& other)
  {
    fog_api.fecolormatrix_multiplyMatrix(this, this, &other);
    return *this;
  }

  FOG_INLINE FeColorMatrix& operator*=(float scalar)
  {
    fog_api.fecolormatrix_multiplyScalar(this, this, NULL, scalar);
    return *this;
  }

  FOG_INLINE FeColorMatrix operator+(const FeColorMatrix& other) const
  {
    FeColorMatrix t(UNINITIALIZED);
    fog_api.fecolormatrix_addMatrix(&t, this, &other);
    return t;
  }

  FOG_INLINE FeColorMatrix operator-(const FeColorMatrix& other) const
  {
    FeColorMatrix t(UNINITIALIZED);
    fog_api.fecolormatrix_subtractMatrix(&t, this, &other);
    return t;
  }

  FOG_INLINE FeColorMatrix operator*(const FeColorMatrix& other) const
  {
    FeColorMatrix t(UNINITIALIZED);
    fog_api.fecolormatrix_multiplyMatrix(&t, this, &other);
    return t;
  }

  FOG_INLINE bool operator==(const FeColorMatrix& other) const { return  eq(other); }
  FOG_INLINE bool operator!=(const FeColorMatrix& other) const { return !eq(other); }

  //! @brief Overload the [] operator for access to a row. This will enable
  //! access to the elements by using [y][x].
  FOG_INLINE float* operator[](size_t y)
  {
    FOG_ASSERT_X(y < 5, "Fog::FeColorMatrix::operator[] - Index out of bounds.");
    return &m[y * 5];
  }

  //! @overload.
  FOG_INLINE const float* const operator[](size_t y) const
  {
    FOG_ASSERT_X(y < 5, "Fog::FeColorMatrix::operator[] - Index out of bounds.");
    return &m[y * 5];
  }

  // --------------------------------------------------------------------------
  // [Statics - Instance]
  // --------------------------------------------------------------------------

  //! @brief Get global instance of identity matrix.
  static FOG_INLINE const FeColorMatrix& getIdentityInstance()
  {
    return *fog_api.fecolormatrix_oIdentity;
  }

  //! @brief Get global instance of zero matrix.
  static FOG_INLINE const FeColorMatrix& getZeroInstance()
  {
    return *fog_api.fecolormatrix_oZero;
  }

  //! @brief Get global instance of grayscale matrix.
  static FOG_INLINE const FeColorMatrix& getGreyscaleInstance()
  {
    return *fog_api.fecolormatrix_oGreyscale;
  }

  // --------------------------------------------------------------------------
  // [Statics - Equality]
  // --------------------------------------------------------------------------

  static FOG_INLINE bool eq(const FeColorMatrix* a, const FeColorMatrix* b)
  {
    return fog_api.fecolormatrix_eq(a, b);
  }

  static FOG_INLINE EqFunc getEqFunc()
  {
    return (EqFunc)fog_api.fecolormatrix_eq;
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  float m[25];
};

//! @}

} // Fog namespace

// [Guard]
#endif // _FOG_G2D_IMAGING_FILTERS_FECOLORMATRIX_H
