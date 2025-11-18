//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//  Platform:    BBC microbit
//  Class:       ClockOut
//  Description: Generate a square wave on an external pin
//               with no overhead for the ARM core
//

#pragma once

#include "MTL/nRF51/Timer.h"
#include "MTL/nRF51/GpioTE.h"
#include "MTL/nRF51/Ppi.h"

#include "MTL/Digital.h"

namespace MTL {

template <unsigned PIN, unsigned TIMER, unsigned GPIOTE, unsigned PPI>
class ClockOut
{
private:
   static const unsigned TIMER_CMP = 0;   // Use compare register 0

   nRF51::Timer<TIMER>   timer;
   nRF51::GpioTE<GPIOTE> toggle;
   nRF51::Ppi<PPI>       chan;
   Digital::Out<PIN>     pin;

public:
   ClockOut(unsigned freq, bool start_now=true)
      : timer(nRF51::TIMER_WIDTH_16_BITS)
      , toggle(PIN, nRF51::GPIO_TE_TASK | nRF51::GPIO_TE_TOGGLE)
   {
      chan.config(timer.getEvent(TIMER_CMP), toggle.getTask());

      setFreq(freq);

      if (start_now) start();
   }

   //! Set clock frequency, maximum is 8 MHz
   //  TODO restart the clock if it was already running
   void setFreq(unsigned freq)
   {
      // double frequency as toggle is in effect a divide by 2
      timer.setFreqHz(TIMER_CMP, freq * 2);
   }

   //! Start the clock
   void start()
   {
      timer.start();
   }

   //! Stop the clock
   void stop()
   {
      timer.stop();
   }
};

} // namespace MTL
