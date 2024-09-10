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

// \brief RP2040 IO_BANK peripheral

#pragma once

#include "MTL/Periph.h"
#include "PadsBank.h"

namespace MTL {

struct IoBankReg
{
   struct Gpio
   {
       uint32_t status;
       uint32_t ctrl;
   };

   Gpio     gpio[30];
   uint32_t intr[4];
   uint32_t proc0_inte[4];
   uint32_t proc0_intf[4];
   uint32_t proc0_ints[4];
   uint32_t proc1_inte[4];
   uint32_t proc1_intf[4];
   uint32_t proc1_ints[4];
   uint32_t dormant_wake_inte[4];
   uint32_t dormant_wake_intf[4];
   uint32_t dormant_wake_ints[4];
};

class IoBank : public Periph<IoBankReg, 0x40014000>
{
public:
   enum Func
   {
       XIP   = 0,
       SPI   = 1,
       UART  = 2,
       I2C   = 3,
       PWM   = 4,
       SIO   = 5,
       PIO0  = 6,
       PIO1  = 7,
       CLOCK = 8,
       USB   = 9
   };

   //! Set the function for an I/O pin
   void setFunc(unsigned io_pin, Func func, uint8_t pad_ctrl = 0)
   {
      PadsBank pads;

      pads.set(io_pin, PadsBank::IE | pad_ctrl);
      reg->gpio[io_pin].ctrl = func;
   }
};

} // namespace MTL
