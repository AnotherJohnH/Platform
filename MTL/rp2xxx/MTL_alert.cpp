//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"

void MTL_alert(bool state_)
{
   static MTL::Digital::Out</* MTL::PIN_LED1 */0x19> onboard_led;

   onboard_led = state_;
}
