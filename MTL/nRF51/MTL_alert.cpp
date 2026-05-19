//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "MTL/Digital.h"
#include "MTL/nRF51/module/microbit.h"

static MTL::Digital::Out<MTL::PIN_LED_COL1> led_gnd{false};

void MTL_alert(bool state_)
{
   MTL::Digital::Out<MTL::PIN_LED_COL1> led_gnd{false};
   MTL::Digital::Out<MTL::PIN_LED_ROW1> led{state_};
}
