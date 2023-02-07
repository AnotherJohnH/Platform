//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

//! \file  Pins.h
//! \brief micro::bit V2 module pins

#ifndef MTL_MICROBIT_V2_PINS_H
#define MTL_MICROBIT_V2_PINS_H

#include "MTL/nRF52/Pins.h"

namespace MTL {

static const unsigned PIN_LED_COL1   = nRF52::PIN_0_28;
static const unsigned PIN_LED_COL2   = nRF52::PIN_0_11;
static const unsigned PIN_LED_COL3   = nRF52::PIN_0_31;
static const unsigned PIN_LED_COL4   = nRF52::PIN_1_5;
static const unsigned PIN_LED_COL5   = nRF52::PIN_0_30;

static const unsigned PIN_LED_ROW1   = nRF52::PIN_0_21;
static const unsigned PIN_LED_ROW2   = nRF52::PIN_0_22;
static const unsigned PIN_LED_ROW3   = nRF52::PIN_0_15;
static const unsigned PIN_LED_ROW4   = nRF52::PIN_0_24;
static const unsigned PIN_LED_ROW5   = nRF52::PIN_0_19;

static const unsigned PIN_BTN_A      = nRF52::PIN_0_14;
static const unsigned PIN_BTN_B      = nRF52::PIN_0_23;
static const unsigned PIN_TOUCH      = nRF52::PIN_1_4;

static const unsigned PIN_SPEAKER    = nRF52::PIN_0_0;

static const unsigned PIN_PAD_0      = nRF52::PIN_0_2;
static const unsigned PIN_PAD_1      = nRF52::PIN_0_3;
static const unsigned PIN_PAD_2      = nRF52::PIN_0_4;
static const unsigned PIN_PAD_3      = nRF52::PIN_0_31;
static const unsigned PIN_PAD_4      = nRF52::PIN_0_28;
static const unsigned PIN_PAD_5      = nRF52::PIN_0_14;
static const unsigned PIN_PAD_6      = nRF52::PIN_1_5;
static const unsigned PIN_PAD_7      = nRF52::PIN_0_11;
static const unsigned PIN_PAD_8      = nRF52::PIN_0_10;
static const unsigned PIN_PAD_9      = nRF52::PIN_0_9;
static const unsigned PIN_PAD_10     = nRF52::PIN_0_30;
static const unsigned PIN_PAD_11     = nRF52::PIN_0_23;
static const unsigned PIN_PAD_12     = nRF52::PIN_0_12;
static const unsigned PIN_PAD_13     = nRF52::PIN_0_17;
static const unsigned PIN_PAD_14     = nRF52::PIN_0_1;
static const unsigned PIN_PAD_15     = nRF52::PIN_0_13;
static const unsigned PIN_PAD_16     = nRF52::PIN_1_2;
static const unsigned PIN_PAD_19     = nRF52::PIN_0_26;
static const unsigned PIN_PAD_20     = nRF52::PIN_1_0;

static const unsigned PIN_LED1 = PIN_LED_COL1;

static const unsigned PIN_NOT_CONNECTED = 0xFFFFFFFF;

} // namespace MTL

#endif // MTL_MICROBIT_V2_PINS_H
