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

//! \file Pins.h
//! \brief micro::bit module pins

#ifndef MTL_MICROBIT_PINS_H
#define MTL_MICROBIT_PINS_H

#include "MTL/nRF51/Pins.h"

namespace MTL {

// TODO shouldn't these be in the nRF51 header?
static const unsigned  PIN_UART_TX    = nRF51::PIN_24;
static const unsigned  PIN_UART_RX    = nRF51::PIN_25;
static const unsigned  PIN_SPI_MOSI   = nRF51::PIN_21;
static const unsigned  PIN_SPI_MISO   = nRF51::PIN_22;
static const unsigned  PIN_SPI_SCK    = nRF51::PIN_23;
static const unsigned  PIN_I2C_SCL    = nRF51::PIN_0;
static const unsigned  PIN_I2C_SDA    = nRF51::PIN_30;


static const unsigned  PIN_LED_COL1   = nRF51::PIN_4;
static const unsigned  PIN_LED_COL2   = nRF51::PIN_5;
static const unsigned  PIN_LED_COL3   = nRF51::PIN_6;
static const unsigned  PIN_LED_COL4   = nRF51::PIN_7;
static const unsigned  PIN_LED_COL5   = nRF51::PIN_8;
static const unsigned  PIN_LED_COL6   = nRF51::PIN_9;
static const unsigned  PIN_LED_COL7   = nRF51::PIN_10;
static const unsigned  PIN_LED_COL8   = nRF51::PIN_11;
static const unsigned  PIN_LED_COL9   = nRF51::PIN_12;

static const unsigned  PIN_LED_ROW1   = nRF51::PIN_13;
static const unsigned  PIN_LED_ROW2   = nRF51::PIN_14;
static const unsigned  PIN_LED_ROW3   = nRF51::PIN_15;

static const unsigned  PIN_BTN_A      = nRF51::PIN_17;
static const unsigned  PIN_BTN_B      = nRF51::PIN_26;

static const unsigned  PIN_RESET      = nRF51::PIN_19;

static const unsigned  PIN_ACCEL_INT2 = nRF51::PIN_27;
static const unsigned  PIN_ACCEL_INT1 = nRF51::PIN_28;

static const unsigned  PIN_MAG_INT    = nRF51::PIN_29;

static const unsigned  PIN_PAD_0      = nRF51::PIN_3;
static const unsigned  PIN_PAD_1      = nRF51::PIN_2;
static const unsigned  PIN_PAD_2      = nRF51::PIN_1;
static const unsigned  PIN_PAD_3      = nRF51::PIN_4;
static const unsigned  PIN_PAD_4      = nRF51::PIN_17;
static const unsigned  PIN_PAD_5      = nRF51::PIN_5;
static const unsigned  PIN_PAD_6      = nRF51::PIN_14;
static const unsigned  PIN_PAD_7      = nRF51::PIN_13;
static const unsigned  PIN_PAD_8      = nRF51::PIN_18;
static const unsigned  PIN_PAD_9      = nRF51::PIN_15;
static const unsigned  PIN_PAD_10     = nRF51::PIN_6;
static const unsigned  PIN_PAD_11     = nRF51::PIN_26;
static const unsigned  PIN_PAD_12     = nRF51::PIN_20;
static const unsigned  PIN_PAD_13     = nRF51::PIN_23;
static const unsigned  PIN_PAD_14     = nRF51::PIN_22;
static const unsigned  PIN_PAD_15     = nRF51::PIN_21;
static const unsigned  PIN_PAD_16     = nRF51::PIN_16;
static const unsigned  PIN_PAD_19     = nRF51::PIN_0;
static const unsigned  PIN_PAD_20     = nRF51::PIN_30;

static const unsigned  PIN_LED1 = PIN_LED_COL1;

static const unsigned  PIN_NOT_CONNECTED = 0xFFFFFFFF;

} // namespace MTL

#endif // MTL_MICROBIT_PINS_H
