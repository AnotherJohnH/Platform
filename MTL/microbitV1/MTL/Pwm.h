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
