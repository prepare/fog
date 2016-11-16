// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Global/Init_p.h>
#include <Fog/Core/Kernel/Application.h>
#include <Fog/Core/Kernel/Event.h>
#include <Fog/Core/Kernel/Object.h>
#include <Fog/Core/OS/OSUtil.h>
#include <Fog/Core/Threading/Thread.h>
#include <Fog/Core/Threading/ThreadLocal.h>
#include <Fog/Core/Tools/Cpu.h>
#include <Fog/Core/Tools/Logger.h>
#include <Fog/Core/Tools/String.h>
#include <Fog/Core/Tools/TextCodec.h>

// [Dependencies - Windows]
#if defined(FOG_OS_WINDOWS)
# include <Fog/Core/OS/WinUtil.h>
# include <process.h>
# if !defined(STACK_SIZE_PARAM_IS_A_RESERVATION)
#  define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000
# endif // STACK_SIZE_PARAM_IS_A_RESERVATION
#endif // FOG_OS_WINDOWS

// [Dependencies - Posix]
#if defined(FOG_OS_POSIX)
# include <pthread.h>
# include <signal.h>
# include <errno.h>
# include <sched.h>
#endif // FOG_OS_POSIX

// [Dependencies - Mac]
#if defined(FOG_OS_MAC)
# include <mach/mach.h>
#endif // FOG_OS_MAC

// [Dependencies - Linux]
#if defined(FOG_OS_LINUX)
# include <sys/syscall.h>
# include <unistd.h>
#endif // FOG_OS_LINUX

namespace Fog {

// ============================================================================
// [Fog::Thread - Global]
// ============================================================================

static Thread* Thread_mainThread;
static uintptr_t Thread_mainThreadId;

#if defined(FOG_OS_WINDOWS)
static inline uintptr_t Thread_getCurrentThreadId() { return (uintptr_t)::GetCurrentThreadId(); }
#elif defined(FOG_OS_MAC)
static inline uintptr_t Thread_getCurrentThreadId() { return (uintptr_t)mach_thread_self(); }
#else
static inline uintptr_t Thread_getCurrentThreadId() { return (uintptr_t)syscall(__NR_gettid); }
#endif // FOG_OS_...

// ============================================================================
// [Fog::Thread - Windows]
// ============================================================================

#if defined(FOG_OS_WINDOWS)

// ============================================================================
// [Fog::Thread - Windows - Thread TLS]
// ============================================================================

static DWORD _Thread_tls;

// ============================================================================
// [Fog::Thread - Windows - Yield / Sleep]
// ============================================================================

void Thread::yield()
{
  ::Sleep(0);
}

void Thread::sleep(uint32_t ms)
{
  ::Sleep(ms);
}

// ============================================================================
// [Fog::Thread - Windows - Current Thread]
// ============================================================================

Thread* Thread::getCurrentThread()
{
  return reinterpret_cast<Thread*>(TlsGetValue(_Thread_tls));
}

uintptr_t Thread::getCurrentThreadId()
{
  return Thread_getCurrentThreadId();
}

Thread* Thread::getMainThread()
{
  return Thread_mainThread;
}

uintptr_t Thread::getMainThreadId()
{
  return Thread_mainThreadId;
}

bool Thread::isMainThread()
{
  return Thread_getCurrentThreadId() == Thread_mainThreadId;
}

// ============================================================================
// [Fog::Thread - Windows - Entry]
// ============================================================================

static unsigned __stdcall _Thread_entry(void* closure)
{
  Thread* thread = static_cast<Thread*>(closure);
  TlsSetValue(_Thread_tls, thread);

  thread->main();
  thread->finished();

  return 0;
}

// ============================================================================
// [Fog::Thread - Windows - Create / Join]
// ============================================================================

static bool _Thread_create(size_t stackSize, Thread* thread)
{
  uint flags = 0;

  if (stackSize > 0 && WinUtil::getWinVersion() >= WIN_VERSION_XP)
    flags = STACK_SIZE_PARAM_IS_A_RESERVATION;
  else
    stackSize = 0;

  thread->_handle = reinterpret_cast<Thread::Handle>(_beginthreadex(NULL, (uint)stackSize, _Thread_entry, thread, flags, NULL));
  return thread->_handle != NULL;
}

static void _Thread_join(Thread* thread)
{
  FOG_ASSERT(thread->_handle);

  // Wait for the thread to exit.
  DWORD result = WaitForSingleObject(thread->_handle, INFINITE);
  FOG_ASSERT(result == WAIT_OBJECT_0);

  CloseHandle(thread->_handle);
  thread->_handle = NULL;
}

// ============================================================================
// [Fog::Thread - Windows - Affinity]
// ============================================================================

err_t Thread::setAffinity(int mask)
{
  if (mask <= 0) return resetAffinity();
  DWORD_PTR result = SetThreadAffinityMask(_handle, (DWORD_PTR)mask);

  if (result != 0)
    return ERR_OK;
  else
    return ERR_RT_INVALID_ARGUMENT;
}

err_t Thread::resetAffinity()
{
  size_t affinityMask = (1 << Cpu::get()->getNumberOfProcessors()) - 1;
  DWORD_PTR result = SetThreadAffinityMask(_handle, (DWORD_PTR)(affinityMask));

  if (result != 0)
    return ERR_OK;
  else
    return ERR_RT_INVALID_ARGUMENT;
}

#endif

// ============================================================================
// [Fog::Thread - Posix]
// ============================================================================

#if defined(FOG_OS_POSIX)

// ============================================================================
// [Fog::Thread - Posix - Thread TLS]
// ============================================================================

static pthread_key_t _Thread_tls;

// ============================================================================
// [Fog::Thread - Posix - Yield / Sleep]
// ============================================================================

void Thread::yield()
{
  sched_yield();
}

void Thread::sleep(uint32_t ms)
{
  struct timespec sleep_time, remaining;

  // Contains the portion of ms >= 1 sec.
  sleep_time.tv_sec = ms / 1000;
  ms -= sleep_time.tv_sec * 1000;

  // Contains the portion of ms < 1 sec.
  sleep_time.tv_nsec = ms * 1000 * 1000; // nanoseconds.

  while (nanosleep(&sleep_time, &remaining) == -1 && errno == EINTR)
    sleep_time = remaining;
}

// ============================================================================
// [Fog::Thread - Posix - Current Thread]
// ============================================================================

Thread* Thread::getCurrentThread()
{
  return reinterpret_cast<Thread*>(pthread_getspecific(_Thread_tls));
}

uintptr_t Thread::getCurrentThreadId()
{
  return Thread_getCurrentThreadId();
}

Thread* Thread::getMainThread()
{
  return Thread_mainThread;
}

uintptr_t Thread::getMainThreadId()
{
  return Thread_mainThreadId;
}

bool Thread::isMainThread()
{
  return Thread_getCurrentThreadId() == Thread_mainThreadId;
}

// ============================================================================
// [Fog::Thread - Posix - Entry]
// ============================================================================

static void* _Thread_entry(void* closure)
{
  Thread* thread = static_cast<Thread*>(closure);
  pthread_setspecific(_Thread_tls, thread);

  thread->main();
  thread->finished();
  return NULL;
}

// ============================================================================
// [Fog::Thread - Posix - Create / Join]
// ============================================================================

static bool _Thread_create(size_t stackSize, Thread* thread)
{
  bool success = false;

  pthread_attr_t attributes;
  pthread_attr_init(&attributes);

  // Pthreads are joinable by default, so we don't need to specify any special
  // attributes to be able to call pthread_join later.
  if (stackSize > 0)
    pthread_attr_setstacksize(&attributes, stackSize);

  success = !pthread_create(&thread->_handle, &attributes, _Thread_entry, thread);

  pthread_attr_destroy(&attributes);
  return success;
}

static void _Thread_join(Thread* thread)
{
  pthread_join(thread->_handle, NULL);
}

// ============================================================================
// [Fog::Thread - Posix - Affinity]
// ============================================================================

err_t Thread::setAffinity(int mask)
{
  if (mask <= 0) return resetAffinity();

#if defined(FOG_OS_LINUX)
  size_t affinityMask = mask;
  return pthread_setaffinity_np(_handle, sizeof(affinityMask), (const cpu_set_t*)&affinityMask);
#else
  return ERR_RT_NOT_IMPLEMENTED;
#endif
}

err_t Thread::resetAffinity()
{
#if defined(FOG_OS_LINUX)
  size_t affinityMask = (1 << Cpu::get()->getNumberOfProcessors()) - 1;
  return pthread_setaffinity_np(_handle, sizeof(affinityMask), (const cpu_set_t*)&affinityMask);
#else
  return ERR_RT_NOT_IMPLEMENTED;
#endif
}

#endif

// ============================================================================
// [Fog::Thread]
// ============================================================================

Thread::Thread() :
  _handle(),
  _id(0),
  _stackSize(0),
  _startupData(NULL),
  _eventLoop()
{
}

Thread::~Thread()
{
  if (this == Thread_mainThread)
  {
    Thread_mainThread = NULL;
    return;
  }

  stop();
}

err_t Thread::setStackSize(uint32_t stackSize)
{
  if (isStarted())
    return ERR_RT_INVALID_STATE;

  _stackSize = stackSize;
  return ERR_OK;
}

bool Thread::start(const StringW& eventLoopType)
{
  FOG_ASSERT(!_eventLoop.isCreated());

  StartupData startupData(eventLoopType);
  _startupData = &startupData;

  if (!_Thread_create(_stackSize, this))
  {
    Logger::fatal("Fog::Thread", "start",
      "failed to create a thread.");

    _startupData = NULL; // Record that we failed to start.
    return false;
  }

  // Wait for the thread to start and initialize the _eventLoop.
  startupData.event.wait();
  return true;
}

void Thread::stop()
{
  // We should only be called on the same thread that started us.
  FOG_ASSERT(_id == getCurrentThreadId());

  if (!isStarted())
    return;

  // The stopSoon() method may have already been called.
  if (_eventLoop.isCreated())
    _eventLoop.postTask(fog_new QuitTask());

  // Wait for the thread to exit. It should already have terminated but make
  // sure this assumption is valid.
  _Thread_join(this);

  // The thread can't receive events anymore.
  _eventLoop.reset();

  // The thread no longer needs to be joined.
  _startupData = NULL;
}

void Thread::stopSoon()
{
  // We should only be called on the same thread that started us.
  FOG_ASSERT(_id == getCurrentThreadId());

  if (!_eventLoop.isCreated())
    return;

  _eventLoop.postTask(fog_new QuitTask());

  // The thread can't receive events anymore.
  _eventLoop.reset();
}

void Thread::init()
{
}

void Thread::cleanUp()
{
}

void Thread::main()
{
  // Complete the initialization of our Thread object.
  _id = getCurrentThreadId();

  // The event loop for this thread.
  if (!_startupData->eventLoopType.isEmpty())
    _eventLoop.adopt(Application::createEventLoop(_startupData->eventLoopType));

  if (_eventLoop.isCreated())
  {
    // Event loop
    _startupData->event.signal();

    // _startupData can't be touched anymore since the starting thread is now
    // unlocked.

    // Let the thread do extra initialization.
    init();

    _eventLoop.run();

    // Let the thread do extra cleanup.
    cleanUp();

    // We can't receive messages anymore.
    _eventLoop.reset();
  }
  else
  {
    // Thread is waiting for it.
    _startupData->event.signal();
  }
}

void Thread::finished()
{
}

// ============================================================================
// [Fog::Thread - Main]
// ============================================================================

struct FOG_NO_EXPORT MainThread : public Thread
{
  MainThread();
  virtual void main();
};

MainThread::MainThread()
{
}

void MainThread::main()
{
  FOG_ASSERT_NOT_REACHED();
}

static Static<MainThread> _Thread_mainThread;

// ============================================================================
// [Init / Fini]
// ============================================================================

FOG_NO_EXPORT void Thread_init(void)
{
  Thread* thread = _Thread_mainThread.init();

#if defined(FOG_OS_WINDOWS)
  thread->_handle = GetCurrentThread();
  thread->_id = Thread::getCurrentThreadId();

  _Thread_tls = ::TlsAlloc();
  ::TlsSetValue(_Thread_tls, thread);
#endif // FOG_OS_WINDOWS

#if defined(FOG_OS_POSIX)
  thread->_handle = pthread_self();
  thread->_id = Thread::getCurrentThreadId();

  int error = ::pthread_key_create(&_Thread_tls, NULL);

  if (error != 0)
  {
    Logger::fatal("Fog::ThreadLocal", "$init", "pthread_key_create() failed (error=%d).", error);
    Application::terminate(-1);
  }

  pthread_setspecific(_Thread_tls, thread);
#endif // FOG_OS_POSIX

  Thread_mainThread = thread;
  Thread_mainThreadId = thread->getId();
}

FOG_NO_EXPORT void Thread_fini(void)
{
  _Thread_mainThread.destroy();

#if defined(FOG_OS_WINDOWS)
  ::TlsFree(_Thread_tls);
#endif // FOG_OS_WINDOWS

#if defined(FOG_OS_POSIX)
  ::pthread_key_delete(_Thread_tls);
#endif // FOG_OS_POSIX
}

} // Fog namespace
