//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/nRF51/module/microbit.h"

void MTL_alert(bool state_)
{
   static MTL::Digital::Out<MTL::PIN_LED1> led{};

   led = state_;
}
