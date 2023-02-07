//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

// Sounder implementation using PWM

#include "PLT/Sounder.h"
#include "MTL/nRF52/Pwm.h"
#include "MTL/Pins.h"

void PLT::Sounder::setEnable(bool enable)
{
   control(enable ? 0xFF : 0x00, 440);
}

void PLT::Sounder::control(unsigned gain_8, unsigned freq)
{
   static MTL::nRF52::Pwm1<MTL::PIN_SPEAKER> pwm;

   if (gain_8 == 0)
   {
      pwm.stop();
   }
   else
   {
      unsigned period  = pwm.getClock() / freq;
      unsigned divisor = 0;

      for(; divisor < 7; divisor++)
      {
         period = period / 2;
         if (period < 0x7FFF) break;
      }

      unsigned space = period / 2;

      pwm.setClkDiv(divisor);
      pwm.program(space, period);
      pwm.start();
   }
}
