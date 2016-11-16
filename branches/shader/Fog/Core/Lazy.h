// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_LAZY_H
#define _FOG_CORE_LAZY_H

// [Dependencies]
#include <Fog/Core/Atomic.h>
#include <Fog/Core/Build.h>
#include <Fog/Core/Constants.h>

namespace Fog {

//! @addtogroup Fog_Core_Threading
//! @{

// ============================================================================
// [Fog::LazyBase]
// ============================================================================

//! @internal
//!
//! @brief Base class for @c Lazy template (exported in the library).
struct FOG_API LazyBase
{
  //! @brief Current state, stored in the @c _ptr member.
  enum STATE
  {
    //! @brief Lazy object isn't initialized yet (same as @c NULL).
    STATE_NULL = 0x0,
    //! @brief The instance is being created. This happens in rare conditions
    //! when two threads tries to create the instance at the same time.
    STATE_CREATING_NOW = 0x1
  };

  LazyBase();
  FOG_INLINE explicit LazyBase(_DONT_INITIALIZE x) { FOG_UNUSED(x); }
  virtual ~LazyBase();

protected:
  //! @brief Create the instance, called once by @c _get() method.
  virtual void* _create() = 0;

  //! @brief Get the instance or create it.
  virtual void* _get();

  //! @brief Instance pointer.
  void* _ptr;

private:
  FOG_DISABLE_COPY(LazyBase)
};

// ============================================================================
// [Fog::Lazy<T>]
// ============================================================================

//! @brief Template that can be used to create class instance on demand (lazy
//! instantiation).
template<typename T>
struct Lazy : public LazyBase
{
  //! @brief Constructor, will create class without creating the instance, you
  //! must call @c get() to create it.
  FOG_INLINE Lazy() {}

  //! @brief Constructor used by the library (internal).
  explicit FOG_INLINE Lazy(_DONT_INITIALIZE x) : LazyBase(DONT_INITIALIZE) {}

  //! @brief Destructor, will destroy the lazy instance.
  virtual ~Lazy()
  {
    // Delete and clear pointer, because LazyBase checks for it
    // and can assert if it's not NULL
    if (_ptr) { delete reinterpret_cast<T*>(_ptr); _ptr = NULL; }
  }

  //! @brief Get or create the lazy instance.
  FOG_INLINE T* get() { return reinterpret_cast<T*>(LazyBase::_get()); }

  //! @brief Get or create the lazy instance (overloaded operator for convenience).
  FOG_INLINE T* operator->() { return get(); }

protected:
  virtual void* _create() { return new(std::nothrow) T(); }
};

// @}

} // Fog namespace

// [Guard]
#endif // _FOG_CORE_LAZY_H
