// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Dependencies]
#include <Fog/Core/C++/Base.h>

// [Guard]
#if !defined(FOG_IDE) && !defined(_FOG_CORE_THREADING_ATOMIC_H)
#error "Fog::Threading::Atomic::GCC_Intrin - Only Fog/Core/Threading/Atomic.h can include this file."
#else

namespace Fog {

// ============================================================================
// [Fog::AtomicInt32]
// ============================================================================

//! @internal
struct AtomicInt32
{
  typedef int32_t Type;

  static FOG_INLINE void set(int32_t* atomic, int32_t value)
  {
    __sync_lock_test_and_set(atomic, value);
  }

  static FOG_INLINE int32_t setXchg(int32_t* atomic, int32_t value)
  {
    return __sync_lock_test_and_set(atomic, value);
  }

  static FOG_INLINE bool cmpXchg(int32_t* atomic, int32_t compar, int32_t value)
  {
    return __sync_bool_compare_and_swap(atomic, compar, value);
  }

  static FOG_INLINE int32_t get(const int32_t* atomic)
  {
#if defined(FOG_ARCH_X86) || defined(FOG_ARCH_X86_64)
    // There's no __sync_* method which could be used here.
    int32_t result;
    __asm__ __volatile__
    (
      "movl %1, %0\n"
        : "=r" (result)
        : "m" (*atomic)
    );
    return result;
#else
    return *(const volatile int32_t*)atomic;
#endif
  }

  static FOG_INLINE void inc(int32_t* atomic)
  {
    __sync_fetch_and_add(atomic, 1);
  }

  static FOG_INLINE void dec(int32_t* atomic)
  {
    __sync_fetch_and_sub(atomic, 1);
  }

  static FOG_INLINE void add(int32_t* atomic, int32_t value)
  {
    __sync_fetch_and_add(atomic, value);
  }

  static FOG_INLINE void sub(int32_t* atomic, int32_t value)
  {
    __sync_fetch_and_sub(atomic, value);
  }

  static FOG_INLINE int32_t addXchg(int32_t* atomic, int32_t value)
  {
    return __sync_fetch_and_add(atomic, value);
  }

  static FOG_INLINE int32_t subXchg(int32_t* atomic, int32_t value)
  {
    return __sync_fetch_and_sub(atomic, value);
  }

  static FOG_INLINE bool deref(int32_t* atomic)
  {
#if defined(FOG_ARCH_X86) || defined(FOG_ARCH_X86_64)
    // There's no __sync_* method, so we leave the x86(_64) version.
    char result;

    __asm__ __volatile__
    (
      "lock; decl %0\n"
      "setne %1\n"
        : "=m" (*atomic), "=qm" (result)
        : "m" (*atomic)
    );
    return !result;
#else
    return __sync_fetch_and_sub(atomic, 1) == 1;
#endif
  }
};

// Specialize.
template<>
struct AtomicImplementation<4> : public AtomicInt32 {};

// ============================================================================
// [Fog::AtomicInt64]
// ============================================================================

#if FOG_ARCH_BITS == 64

//! @internal
struct AtomicInt64
{
  typedef int64_t Type;

  static FOG_INLINE void set(int64_t* atomic, int64_t value)
  {
    __sync_lock_test_and_set(atomic, value);
  }

  static FOG_INLINE int64_t setXchg(int64_t* atomic, int64_t value)
  {
    return __sync_lock_test_and_set(atomic, value);
  }

  static FOG_INLINE bool cmpXchg(int64_t* atomic, int64_t compar, int64_t value)
  {
    return __sync_bool_compare_and_swap(atomic, compar, value);
  }

  static FOG_INLINE int64_t get(const int64_t* atomic)
  {
#if defined(FOG_ARCH_X86) || defined(FOG_ARCH_X86_64)
    // There's no __sync_* method which could be used here.
    int64_t result;
    __asm__ __volatile__
    (
      "movq %1, %0\n"
        : "=r" (result)
        : "m" (*atomic)
    );
    return result;
#else
    return *(const volatile int64_t*)atomic;
#endif
  }

  static FOG_INLINE void inc(int64_t* atomic)
  {
    __sync_fetch_and_add(atomic, 1);
  }

  static FOG_INLINE void dec(int64_t* atomic)
  {
    __sync_fetch_and_sub(atomic, 1);
  }

  static FOG_INLINE void add(int64_t* atomic, int64_t value)
  {
    __sync_fetch_and_add(atomic, value);
  }

  static FOG_INLINE void sub(int64_t* atomic, int64_t value)
  {
    __sync_fetch_and_sub(atomic, value);
  }

  static FOG_INLINE int64_t addXchg(int64_t* atomic, int64_t value)
  {
    return __sync_fetch_and_add(atomic, value);
  }

  static FOG_INLINE int64_t subXchg(int64_t* atomic, int64_t value)
  {
    return __sync_fetch_and_sub(atomic, value);
  }

  static FOG_INLINE bool deref(int64_t* atomic)
  {
#if defined(FOG_ARCH_X86) || defined(FOG_ARCH_X86_64)
    char result;

    __asm__ __volatile__
    (
      "lock; decq %0\n"
      "setne %1\n"
        : "=m" (*atomic), "=qm" (result)
        : "m" (*atomic)
    );
    return !result;
#else
    return __sync_fetch_and_sub(atomic, (int64_t)1) == (int64_t)1;
#endif
  }
};

// Specialize.
template<>
struct AtomicImplementation<8> : public AtomicInt64 {};

#endif // FOG_ARCH_BITS == 64

} // Fog namespace

// [Guard]
#endif // _FOG_CORE_THREADING_ATOMIC_H
