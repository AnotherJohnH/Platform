//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

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
