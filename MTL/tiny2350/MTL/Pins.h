//------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
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

//! \brief module pins

#pragma once

#include "MTL/chip/Pins.h"

namespace MTL {

static const unsigned PIN_4    = IO_PIN_29;
static const unsigned PIN_5    = IO_PIN_28;
static const unsigned PIN_6    = IO_PIN_27;
static const unsigned PIN_7    = IO_PIN_26;

static const unsigned PIN_9    = IO_PIN_7;
static const unsigned PIN_10   = IO_PIN_6;
static const unsigned PIN_11   = IO_PIN_5;
static const unsigned PIN_12   = IO_PIN_4;
static const unsigned PIN_13   = IO_PIN_3;
static const unsigned PIN_14   = IO_PIN_2;
static const unsigned PIN_15   = IO_PIN_1;
static const unsigned PIN_16   = IO_PIN_0;

static const unsigned PIN_LED1 = IO_PIN_18;
static const unsigned PIN_LED2 = IO_PIN_19;
static const unsigned PIN_LED3 = IO_PIN_20;

static const unsigned PIN_LED_RED = PIN_LED1;
static const unsigned PIN_LED_GRN = PIN_LED2;
static const unsigned PIN_LED_BLU = PIN_LED2;

static const unsigned PIN_IGNORE = 0xFFFFFFFF;

} // namespace MTL
