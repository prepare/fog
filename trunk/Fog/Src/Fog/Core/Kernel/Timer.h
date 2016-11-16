// [Fog-Core]
//
// [License]
// MIT, See COPYING file in package

// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// [Guard]
#ifndef _FOG_CORE_KERNEL_TIMER_H
#define _FOG_CORE_KERNEL_TIMER_H

// [Dependencies]
#include <Fog/Core/Global/Api.h>
#include <Fog/Core/Kernel/Object.h>
#include <Fog/Core/Kernel/Task.h>
#include <Fog/Core/Tools/Time.h>

namespace Fog {

//! @addtogroup Fog_Core_Kernel
//! @{

// ============================================================================
// [Fog::Timer]
// ============================================================================

//! @brief Timer.
struct FOG_API Timer : public Object
{
  FOG_DECLARE_OBJECT(Timer, Object)

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  Timer();
  virtual ~Timer();

  // --------------------------------------------------------------------------
  // [Accessors]
  // --------------------------------------------------------------------------

  FOG_INLINE bool isRunning() const { return _task != NULL; }
  FOG_INLINE TimeDelta getInterval() const { return _interval; }

  void setInterval(TimeDelta interval);

  // --------------------------------------------------------------------------
  // [Start / Stop]
  // --------------------------------------------------------------------------

  bool start();
  void stop();

  // --------------------------------------------------------------------------
  // [Event Handlers]
  // --------------------------------------------------------------------------

  virtual void onTimer(TimerEvent* e);

  FOG_EVENT_BEGIN()
    FOG_EVENT_DEF(EVENT_TIMER, onTimer, TimerEvent, OVERRIDE)
  FOG_EVENT_END()

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  Task* _task;
  TimeDelta _interval;
};

//! @}

} // Fog namespace

#endif  // _FOG_CORE_KERNEL_TIMER_H
