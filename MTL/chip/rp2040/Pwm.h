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
// \brief RP2040 PWM peripheral

#pragma once

#include "MTL/Periph.h"
#include "MTL/Pins.h"

#include "PadsBank.h"
#include "IoBank.h"

namespace MTL {

struct PwmReg
{
   struct Slice
   {
      uint32_t csr;
      uint32_t div;
      uint32_t ctr;
      uint32_t cc;
      uint32_t top;
   };
   
   Slice    ch[8];
   uint32_t en;
   uint32_t intr;
   uint32_t inte;
   uint32_t intf;
   uint32_t ints;
};

//! PWM peripheral
template <unsigned PIN, bool PAIR = false>
class Pwm : public Periph<PwmReg, 0x40050000>
{
public:
   //! Configure a single PWM output
   Pwm(unsigned sys_clk_div8_4_ = 0b10000,
       unsigned period_         = 0x10000,
       bool     dreq_only_      = false)
   {
      IoBank io_bank;

      if (PAIR)
      {
         unsigned pin_a = PIN & 0b11110;
         unsigned pin_b = pin_a | 1;

         io_bank.setFunc(pin_a, IoBank::PWM, PadsBank::DRIVE_2MA);
         io_bank.setFunc(pin_b, IoBank::PWM, PadsBank::DRIVE_2MA);
      }
      else if (not dreq_only_)
      {
         io_bank.setFunc(PIN, IoBank::PWM, PadsBank::DRIVE_2MA);
      }

      // Configure slice XXX may be shared with another PWM pin
      reg->ch[SLICE].top = period_ - 1;
      reg->ch[SLICE].div = sys_clk_div8_4_;
      reg->ch[SLICE].cc  = 0;
      reg->ch[SLICE].csr = (1 << 0); // EN
   }

   //! Get DMA req id
   unsigned getDREQ() const { return DREQ_BASE + SLICE; }

   //! Get address of slice output register
   volatile uint32_t* getOut() const { return &reg->ch[SLICE].cc; }

   //! Set system clock divider for PWM counter clock in fixed-point 8.4
   void setSysClkDiv8_4(unsigned sys_clk_div8_4_)
   {
      reg->ch[SLICE].div = sys_clk_div8_4_;
   }

   //! Set PWM period in PWM counter clock ticks for a slice
   void setPeriod(uint16_t period_)
   {
      reg->ch[SLICE].top = period_ - 1;
   }

   //! Set pulse width in PWM counter clock ticks for a single channel
   void setWidth(uint16_t value_)
   {
      uint32_t mask  = ~(0xFFFF << (16 * CHAN));

      reg->ch[SLICE].cc = (reg->ch[SLICE].cc & mask) | (value_ << (16 * CHAN));
   }

   //! Set width for both channels of a single slice
   void setPair(uint32_t value_)
   {
      reg->ch[SLICE].cc = value_;
   }

   //! Invert a single channel
   void invert()
   {
      reg->ch[SLICE].csr |= 1 << (CHAN + 2);
   }

   //! Set pulse width in PWM counter clock ticks for a single channel
   uint16_t operator=(uint16_t value_)
   {
      setWidth(value_);
      return value_;
   }

   //! Get counter value for slice
   uint32_t getCounter() const { return reg->ch[SLICE].ctr; }

private:
   static const unsigned DREQ_BASE = 24;
   static const unsigned SLICE     = (PIN >> 1) & 0b111; //!< PWM slice (0..7)
   static const unsigned CHAN      = PIN & 1;            //!< PWM channel (0..1)
};

} // namespace MTL
