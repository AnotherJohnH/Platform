//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"

namespace {

void usleep(unsigned microseconds)
{
   unsigned long future = MTL_us_clock() + microseconds + 1;

   while(signed(MTL_us_clock() - future) < 0);
}

} // namespace

void MTL_halt(unsigned status_)
{
#if 0
   MTL::Digital::Out<MTL::PIN_LED1> onboard_led;
#else
   bool onboard_led;
#endif

   while(true)
   {
      for(unsigned i = 0; i < status_; ++i)
      {
         onboard_led = true;
         usleep(100000);

         onboard_led = false;
         usleep(100000);
      }

      usleep(1000000);
   }
}
