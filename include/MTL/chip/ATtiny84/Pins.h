//------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
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

// \file Pins.h
// \brief ATMEL ATtiny84 pins
//
// Data source Atmel document "doc8006 8-bit AVR Microcontroller ATtiny84"

#ifndef ATTINY84_PINS_H
#define ATTINY84_PINS_H

namespace MTL {

// Pin constants encode [bit:3][reg:8]

static const unsigned  PIN_2  = 0x016; // PB0
static const unsigned  PIN_3  = 0x116; // PB1
static const unsigned  PIN_4  = 0x316; // PB3
static const unsigned  PIN_5  = 0x216; // PB2
static const unsigned  PIN_6  = 0x719; // PA7
static const unsigned  PIN_7  = 0x619; // PA6
static const unsigned  PIN_8  = 0x519; // PA5
static const unsigned  PIN_9  = 0x419; // PA4
static const unsigned  PIN_10 = 0x319; // PA3
static const unsigned  PIN_11 = 0x219; // PA2
static const unsigned  PIN_12 = 0x119; // PA1
static const unsigned  PIN_13 = 0x019; // PA0

static const unsigned  PIN_LED1 = PIN_2;

} // namespace MTL

#endif // ATTINY84_PINS_H
