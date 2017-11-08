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

#ifndef MICROBIT_PINS_H
#define MICROBIT_PINS_H

#include "MTL/chip/nRF51/Pins.h"

namespace MTL {

static const unsigned  PIN_LED_COL1   =  4;
static const unsigned  PIN_LED_COL2   =  5;
static const unsigned  PIN_LED_COL3   =  6;
static const unsigned  PIN_LED_COL4   =  7;
static const unsigned  PIN_LED_COL5   =  8;
static const unsigned  PIN_LED_COL6   =  9;
static const unsigned  PIN_LED_COL7   = 10;
static const unsigned  PIN_LED_COL8   = 11;
static const unsigned  PIN_LED_COL9   = 12;

static const unsigned  PIN_LED_ROW1   = 13;
static const unsigned  PIN_LED_ROW2   = 14;
static const unsigned  PIN_LED_ROW3   = 15;

static const unsigned  PIN_BTN_A      = 17;
static const unsigned  PIN_BTN_B      = 26;

static const unsigned  PIN_RESET      = 19;

static const unsigned  PIN_SPI_MOSI   = 21;
static const unsigned  PIN_SPI_MISO   = 22;
static const unsigned  PIN_SPI_SCK    = 23;

static const unsigned  PIN_UART_TX    = 24;
static const unsigned  PIN_UART_RX    = 25;

static const unsigned  PIN_ACCEL_INT2 = 27;
static const unsigned  PIN_ACCEL_INT1 = 28;

static const unsigned  PIN_MAG_INT    = 29;

static const unsigned  PIN_I2C_SCL    =  0;
static const unsigned  PIN_I2C_SDA    = 30;

static const unsigned  PIN_PAD_0  =  3;
static const unsigned  PIN_PAD_1  =  2;
static const unsigned  PIN_PAD_2  =  1;
static const unsigned  PIN_PAD_3  =  4;
static const unsigned  PIN_PAD_4  = 17;
static const unsigned  PIN_PAD_5  =  5;
static const unsigned  PIN_PAD_6  = 14;
static const unsigned  PIN_PAD_7  = 13;
static const unsigned  PIN_PAD_8  = 18;
static const unsigned  PIN_PAD_9  = 15;
static const unsigned  PIN_PAD_10 =  6;
static const unsigned  PIN_PAD_11 = 26;
static const unsigned  PIN_PAD_12 = 20;
static const unsigned  PIN_PAD_13 = 23;
static const unsigned  PIN_PAD_14 = 22;
static const unsigned  PIN_PAD_15 = 21;
static const unsigned  PIN_PAD_16 = 16;
static const unsigned  PIN_PAD_19 =  0;
static const unsigned  PIN_PAD_20 = 30;

static const unsigned  PIN_LED1 = PIN_LED_COL1;

static const unsigned  PIN_NOT_CONNECTED = 0xFFFFFFFF;

} // namespace MTL

#endif // MICROBIT_PINS_H
