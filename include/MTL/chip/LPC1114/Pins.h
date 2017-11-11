//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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
// \brief NXP LPC1114FN28 pins
//
// Data source NXP document "LPC111x User Manual UM10398"

#ifndef LPC1114_PINS_H
#define LPC1114_PINS_H


namespace MTL {

static const unsigned  PIN_1  = 0x008;
static const unsigned  PIN_2  = 0x009;
static const unsigned  PIN_3  = 0x00A;
static const unsigned  PIN_4  = 0x00B;
static const unsigned  PIN_5  = 0x005;
static const unsigned  PIN_6  = 0x006;
//                     PIN_7
//                     PIN_8
static const unsigned  PIN_9  = 0x100;
static const unsigned  PIN_10 = 0x101;
static const unsigned  PIN_11 = 0x102;
static const unsigned  PIN_12 = 0x103;
static const unsigned  PIN_13 = 0x104;
static const unsigned  PIN_14 = 0x105;
static const unsigned  PIN_15 = 0x106;
static const unsigned  PIN_16 = 0x107;
static const unsigned  PIN_17 = 0x108;
static const unsigned  PIN_18 = 0x109;
//                     PIN_19
//                     PIN_20
//                     PIN_21
//                     PIN_22
static const unsigned  PIN_23 = 0x000;
static const unsigned  PIN_24 = 0x001;
static const unsigned  PIN_25 = 0x002;
static const unsigned  PIN_26 = 0x003;
static const unsigned  PIN_27 = 0x004;
static const unsigned  PIN_28 = 0x007;


static const unsigned  PIN_LED1 = PIN_1;

} // namespace MTL

#endif // LPC1114_PINS_H
