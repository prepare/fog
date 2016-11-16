// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_VALUE_H
#define _FOG_CORE_VALUE_H

// [Dependencies]
#include <Fog/Build/Build.h>
#include <Fog/Core/Atomic.h>
#include <Fog/Core/Memory.h>
#include <Fog/Core/String.h>
#include <Fog/Core/TypeInfo.h>

//! @addtogroup Fog_Core
//! @{

namespace Fog {

// ============================================================================
// [Fog::ValueData]
// ============================================================================

struct FOG_API ValueData
{
  // [Construction / Destruction]

  ValueData(uint32_t type);
  virtual ~ValueData();

  static void* allogetData();
  static void freeData(void* data);

  // [Methods]

  virtual err_t clone(void* dst) const = 0;
  virtual err_t getInt32(int32_t* dst) const = 0;
  virtual err_t getInt64(int64_t* dst) const = 0;
  virtual err_t getDouble(double* dst) const = 0;
  virtual err_t getString(String* dst) const = 0;

  virtual err_t setValue(void* val) = 0;

  ValueData* ref() const;
  void deref();

  // [Members]

  //! @brief Reference count.
  mutable Atomic<sysuint_t> refCount;

  //! @brief Type of Value.
  uint32_t type;
  //! @brief Value flags.
  uint32_t flags;

  //! @brief Inlined data of various types
  //!
  //! Internal implementation:
  //!   - TypeNull
  //!   - TypeInteger: i.i64
  //!   - TypeDouble: i.d
  //!   - TypeString: &raw[8] == Fog::String*
  union
  {
    uint8_t raw[sizeof(void*)];
    int64_t i64;
    double d;
  };
  void* ptr;

private:
  FOG_DISABLE_COPY(ValueData)
};

// ============================================================================
// [Fog::Value]
// ============================================================================

struct FOG_API Value
{
  enum TYPE
  {
    TYPE_NULL = 0,
    TYPE_INTEGER = 1,
    TYPE_DOUBLE = 2,
    TYPE_STRING = 3
  };

  // [Construction / Destruction]

  Value();
  Value(const Value& other);
  explicit FOG_INLINE Value(ValueData* d) : _d(d) {}
  ~Value();

  Value& operator=(const Value& other);

  static Value null();
  static Value fromBool(bool val);
  static Value fromInt32(int32_t val);
  static Value fromInt64(int64_t val);
  static Value fromDouble(double val);
  static Value fromString(const String& val);

  static Value fromErrno();
#if defined(FOG_OS_WINDOWS)
  static Value fromWinLastError();
#endif // FOG_OS_WINDOWS

  // [Implicit Sharing]

  //! @copydoc Doxygen::Implicit::refCount().
  FOG_INLINE sysuint_t refCount() const { return _d->refCount.get(); }
  //! @copydoc Doxygen::Implicit::isDetached().
  FOG_INLINE bool isDetached() const { return refCount() == 1; }
  //! @copydoc Doxygen::Implicit::detach().
  err_t detach();

  // [Methods]

  uint32_t getType() const { return _d->type; }

  FOG_INLINE bool isNull() const { return _d->type == TYPE_NULL; }
  FOG_INLINE bool isInteger() const { return _d->type == TYPE_INTEGER; }
  FOG_INLINE bool isDouble() const { return _d->type == TYPE_DOUBLE; }
  FOG_INLINE bool isString() const { return _d->type == TYPE_STRING; }

  FOG_INLINE err_t getInt32(int32_t* dst) const { return _d->getInt32(dst); }
  FOG_INLINE err_t getInt64(int64_t* dst) const { return _d->getInt64(dst); }
  FOG_INLINE err_t getDouble(double* dst) const { return _d->getDouble(dst); }
  FOG_INLINE err_t getString(String* dst) const { return _d->getString(dst); }

  err_t reset();
  err_t setInt32(int32_t val);
  err_t setInt64(int64_t val);
  err_t setDouble(double val);
  err_t setString(const String& val);

  // [Static Data]

  static ValueData* sharedNull;

  // [Members]

  FOG_DECLARE_D(ValueData)
};

} // Fog namespace

//! @}

// ============================================================================
// [Fog::TypeInfo<>]
// ============================================================================

FOG_DECLARE_TYPEINFO(Fog::Value, Fog::TYPEINFO_MOVABLE)

// [Guard]
#endif // _FOG_CORE_VALUE_H
