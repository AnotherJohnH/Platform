//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/Digital.h"

inline void wait(volatile unsigned n)
{
   while(n--);
}

int MTL_main()
{
#if defined(PLT_TARGET_microbit)
   MTL::Digital::Out<MTL::PIN_LED_COL1> led_gnd(false);
   MTL::Digital::Out<MTL::PIN_LED_ROW1> led;
#else
   MTL::Digital::Out<MTL::PIN_LED1> led;
#endif

   while(true)
   {
      led = !led;

      wait(1000000);
   }
}
