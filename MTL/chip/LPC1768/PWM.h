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

// \file PWM.h
// \brief NXP LPC1768 Pulse Width Modulator
//
// Data source NXP document "LPC17xx User Manual UM10360"

#ifndef LPC1768_PWM_H
#define LPC1768_PWM_H

#include "MTL/Periph.h"

#include "MTL/core/CortexM3/NVIC.h"

#include "SysCon.h"
#include "PinCon.h"

namespace MTL {


union PWMReg
{
   REG(0x00, ir);
   REG(0x04, tcr);
   REG(0x08, tc);
   REG(0x0C, pr);
   REG(0x10, pc);
   REG(0x14, mcr);
   REG(0x18, mr0);
   REG(0x1C, mr1);
   REG(0x20, mr2);
   REG(0x24, mr3);
   REG(0x28, ccr);
   REG(0x2C, cr0);
   REG(0x30, cr1);
   REG(0x34, cr2);
   REG(0x38, cr3);
   REG(0x40, mr4);
   REG(0x44, mr5);
   REG(0x48, mr6);
   REG(0x4C, pcr);
   REG(0x50, ler);
   REG(0x70, ctcr);
};


class PWM : public Periph<PWMReg,0x40018000>
{
private:
   NVIC<9>  nvic;

   template <unsigned N>
   void loadMatch(uint32_t offset)
   {
      switch(N)
      {
      case 0: reg->mr0 = offset; break;
      case 1: reg->mr1 = offset; break;
      case 2: reg->mr2 = offset; break;
      case 3: reg->mr3 = offset; break;
      case 4: reg->mr4 = offset; break;
      case 5: reg->mr5 = offset; break;
      case 6: reg->mr6 = offset; break;
      default: break;
      }

      // Load new match value next cycle
      reg->ler.setBit(N);
   }

public:
   PWM(unsigned rate)
   {
      // Enable power to PWM1 (is already 1 at reset)
      // SysCon().pconp(6, 1);

      // Select PCLK for PWM1 to be CCLK
      SysCon().pclksel0(12, 1);

      // Select PWM6 function for P2.5
      //PinCon().config(PIN_2_0, 1, PinCon::PULL_NONE);
      //PinCon().config(PIN_2_1, 1, PinCon::PULL_NONE);
      //PinCon().config(PIN_2_2, 1, PinCon::PULL_NONE);
      //PinCon().config(PIN_2_3, 1, PinCon::PULL_NONE);
      //PinCon().config(PIN_2_4, 1, PinCon::PULL_NONE);
      PinCon().config(PIN_2_5, 1, PinCon::PULL_NONE);

      // Configure PWM1
      reg->tcr  = 1<<1;              // Reset
      reg->tcr  = 1<<3;              // PWM mode (and clear reset)
      reg->ctcr = 0;                 // Timer mode
      reg->pr   = rate - 1;          // Set prescaler for 'rate' PCLK ticks
      reg->pcr  = (1<<14) | (1<<6);  // Enable PWM6 output double edged
   }

   void setPeriod(uint32_t period)
   {
      loadMatch<0>(period);
   }

   void setRise(uint32_t offset)
   {
      loadMatch<5>(offset);
   }

   void setFall(uint32_t offset)
   {
      loadMatch<6>(offset);
   }

   template <unsigned N>
   void setIRQ(uint32_t offset)
   {
      loadMatch<N>(offset);

      // Raise interrupt on match
      reg->mcr.setBit(3*N);

      nvic.setPriority(0);
      nvic.enable();
   }

   template <unsigned N>
   bool isIRQ()
   {
      unsigned n = N > 3 ? N + 4 : N;

      if (reg->ir.getBit(n))
      {
         reg->ir.setBit(n);
         return true;
      }

      return false;
   }

   void start()
   {
      reg->tcr.setBit(0);
   }
};


} // namespace MTL

#endif // LPC1768_PWM_H
