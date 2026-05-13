//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

namespace {

void usleep(unsigned microseconds)
{
   unsigned long future = MTL_us_clock() + microseconds + 1;

   while(signed(MTL_us_clock() - future) < 0);
}

} // namespace

void MTL_halt(unsigned status_)
{
   while(true)
   {
      for(unsigned i = 0; i < status_; ++i)
      {
         MTL_alert(true);
         usleep(100000);

         MTL_alert(false);
         usleep(100000);
      }

      usleep(1000000);
   }
}
