//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// BBC microbit

#pragma once

#include "MTL/nRF51/Uart.h"
#include "MTL/Pins.h"
#include "MTL/PS2Keyboard.h"

namespace MTL {

using Keyboard = PS2Keyboard<nRF51::Uart<PIN_PAD_12> >;

}
