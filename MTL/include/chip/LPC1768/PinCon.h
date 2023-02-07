//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

// \file PinCon.h
// \brief NXP LPC1768 pin connection peripheral
//
// Data source NXP document "LPC17xx User Manual UM10360"

#ifndef LPC1768_PIN_CON_H
#define LPC1768_PIN_CON_H

#include "MTL/Periph.h"
#include "MTL/Pins.h"


namespace MTL {


union PinConReg
{
   REG_ARRAY(0x000, pinsel,     11);
   REG_ARRAY(0x040, pinmode,    11);
   REG_ARRAY(0x068, pinmode_od,  5);
   REG(      0x07C, i2cpadcfg);
};


class PinCon : public Periph<PinConReg,0x4002C000>
{
public:
   enum Mode
   {
      PULL_UP   = 0,
      REPEAT    = 1,
      PULL_NONE = 2,
      PULL_DOWN = 3
   };

   void config(uint32_t pin, unsigned func, Mode mode)
   {
      unsigned index = pin >> 4;
      unsigned lsb   = (pin & 0xF)<<1;

      reg->pinsel[index].setField( lsb + 1, lsb, func);
      reg->pinmode[index].setField(lsb + 1, lsb, mode);
   }
};


} // namespace MTL

#endif // LPC1768_PIN_CON_H
