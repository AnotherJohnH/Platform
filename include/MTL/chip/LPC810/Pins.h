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
// \brief NXP LPC810 pins
//
// Data source NXP document "LPC81X User Manual UM10601"

#ifndef LPC810_PINS_H
#define LPC810_PINS_H


namespace MTL {

static const unsigned  PIN_1 = 0x5;
static const unsigned  PIN_2 = 0x4;
static const unsigned  PIN_3 = 0x3;
static const unsigned  PIN_4 = 0x2;
static const unsigned  PIN_5 = 0x1;
//                     PIN_6 VDD
//                     PIN_7 VSS
static const unsigned  PIN_8 = 0x0;


static const unsigned  PIN_LED1 = PIN_1;

} // namespace MTL

#endif // LPC810_PINS_H
