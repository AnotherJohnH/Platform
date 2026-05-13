//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/LPC11U24/module/mbed.h"

void MTL_alert(bool state_)
{
   static MTL::Digital::Out<MTL::PIN_LED4> led{};

   led = state_;
}
