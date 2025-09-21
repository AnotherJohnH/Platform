//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"

void MTL_halt(uint32_t status)
{
   if (status & 0b1000) MTL::Digital::Out<MTL::PIN_LED1> led{true};
   if (status & 0b0100) MTL::Digital::Out<MTL::PIN_LED2> led{true};
   if (status & 0b0010) MTL::Digital::Out<MTL::PIN_LED3> led{true};
   if (status & 0b0001) MTL::Digital::Out<MTL::PIN_LED4> led{true};

   while(true);
}
