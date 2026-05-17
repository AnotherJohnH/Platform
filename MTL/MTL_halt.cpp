//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

namespace {

void delay(unsigned ticks_)
{
   for(volatile unsigned i=0; i < ticks_; ++i);
}

} // namespace

void MTL_halt(unsigned status_)
{
   const unsigned SHORT = 1000000;
   const unsigned LONG  = 5000000;

   while(true)
   {
      for(unsigned i = 0; i < status_; ++i)
      {
         MTL_alert(true);
         delay(SHORT);

         MTL_alert(false);
         delay(SHORT);
      }

      delay(LONG);
   }
}
