//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/Digital.h"

extern "C" {

void platform_shutdown()
{
   MTL::Digital::Out<MTL::PIN_LED_COL1> led_gnd(false);
   MTL::Digital::Out<MTL::PIN_LED_ROW1> led{true};

   while(true);
}

void MTL_halt()
{
   MTL::Digital::Out<MTL::PIN_LED_COL1> led_gnd(false);
   MTL::Digital::Out<MTL::PIN_LED_ROW3> led{true};

   while(true);
}

}
