//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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
//
// \brief Piromoni badger 2040

#pragma once

#include "MTL/rp2040/Pins.h"
#include "MTL/rp2040/Adc.h"
#include "MTL/rp2040/Pwm.h"
#include "MTL/rp2040/Gpio.h"
#include "MTL/rp2040/Spi.h"

#include "MTL/EPaper_UC8151.h"

namespace MTL {

namespace badger2040 {

static const unsigned PIN_SW_DN    = rp2040::IO_PIN_11;
static const unsigned PIN_SW_A     = rp2040::IO_PIN_12;
static const unsigned PIN_SW_B     = rp2040::IO_PIN_13;
static const unsigned PIN_SW_C     = rp2040::IO_PIN_14;
static const unsigned PIN_SW_UP    = rp2040::IO_PIN_15;
static const unsigned PIN_SW_USR   = rp2040::IO_PIN_23;
static const unsigned PIN_USER_LED = rp2040::IO_PIN_25;

using EPaper = EPaper_UC8151</* WIDTH  */ 296,
                             /* HEIGHT */ 128,
                             /* DC  */ rp2040::IO_PIN_20,
                             /* CS  */ rp2040::IO_PIN_17,
                             /* RST */ rp2040::IO_PIN_21,
                             /* BSY */ rp2040::IO_PIN_26,
                             Spi0_ALT2>;

using LedUser    = Gpio::Out<1, PIN_USER_LED>;
using PwmLedUser = Pwm<PIN_USER_LED>;

using SwitchUp  = Gpio::In<1, PIN_SW_UP,  PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchDn  = Gpio::In<1, PIN_SW_DN,  PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchA   = Gpio::In<1, PIN_SW_A,   PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchB   = Gpio::In<1, PIN_SW_B,   PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchC   = Gpio::In<1, PIN_SW_C,   PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchUsr = Gpio::In<1, PIN_SW_USR, PadsBank::PULL_DOWN, /* schmitt trigger */ true>;

} // namespace badger2040

} // namespace MTL
