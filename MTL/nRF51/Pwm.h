//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// BBC microbit

#pragma once

#include "MTL/Digital.h"
#include "MTL/nRF51/Timer.h"

namespace MTL {

template <unsigned PIN, unsigned TIMER>
class Pwm
{
private:
   MTL::Digital::Out<PIN>   pin;
   MTL::nRF51::Timer<TIMER> timer;

public:
   Pwm()
      : timer(MTL::nRF51::TIMER_WIDTH_16_BITS)
   {
      timer.setClock(MTL::nRF51::TIMER_16_MHZ);
      timer.setAction(0, 0x8000, MTL::nRF51::TIMER_IRQ);
      timer.setAction(1, 0xFFFF, MTL::nRF51::TIMER_CLEAR | MTL::nRF51::TIMER_IRQ);

      timer.start();
   }

   void operator=(uint16_t value)
   {
      timer.setCompare(0, value);
   }

   void update()
   {
      pin = timer.isCompareEvent(0);
      timer.clrCompareEvent(0);
      timer.clrCompareEvent(1);
   }
};

} // namespace MTL
