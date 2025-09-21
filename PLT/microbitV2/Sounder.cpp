//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
