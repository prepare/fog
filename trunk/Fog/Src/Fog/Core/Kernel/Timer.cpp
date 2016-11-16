// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// [Precompiled Headers]
#if defined(FOG_PRECOMP)
#include FOG_PRECOMP
#endif // FOG_PRECOMP

// [Dependencies]
#include <Fog/Core/Kernel/EventLoop.h>
#include <Fog/Core/Kernel/Timer.h>

FOG_IMPLEMENT_OBJECT(Fog::Timer)

namespace Fog {

// ============================================================================
// [Fog::TimerTask]
// ============================================================================

struct FOG_NO_EXPORT TimerTask : public Task
{
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  TimerTask(Timer* timer) :
    timer(timer)
  {
  }

  // --------------------------------------------------------------------------
  // [Interface]
  // --------------------------------------------------------------------------

  virtual void run();

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  Timer* timer;
};

void TimerTask::run()
{
  if (timer)
  {
    // First send events.
    TimerEvent e(timer);
    timer->sendEvent(&e);

    // Repeat?
    if (timer)
    {
      timer->getHomeThread()->getEventLoop().postTask(this, true, static_cast<int>(timer->_interval.getMilliseconds()));
      setDestroyOnFinish(false);
    }
    else
    {
      // We must be sure, it can be set to false.
      setDestroyOnFinish(true);
    }
  }
}

// ============================================================================
// [Fog::Timer - Construction / Destruction]
// ============================================================================

Timer::Timer() :
  _task(NULL)
{
}

Timer::~Timer()
{
  stop();
}

// ============================================================================
// [Fog::Timer - Accessors]
// ============================================================================

void Timer::setInterval(TimeDelta interval)
{
  if (_interval == interval)
    return;

  _interval = interval;
}

// ============================================================================
// [Fog::Timer - Start / Stop]
// ============================================================================

bool Timer::start()
{
  stop();

  _task = fog_new TimerTask(this);
  getHomeThread()->getEventLoop().postTask(_task, true, static_cast<int>(_interval.getMilliseconds()));
  return true;
}

void Timer::stop()
{
  if (_task)
  {
    static_cast<TimerTask *>(_task)->timer = NULL;
    _task = NULL;
  }
}

void Timer::onTimer(TimerEvent* e)
{
  // Nothing...
}

} // Fog namespace
