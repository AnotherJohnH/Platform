//------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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

// \brief NXP LPC1768 UART
//
// Data source NXP document "LPC176x/5x User Manual UM10360"

#pragma once

#include "MTL/Periph.h"

#include "PinCon.h"
#include "Pins.h"
#include "SysCon.h"

namespace MTL {

union UART023Reg
{
   REG(0x000, rbr_thr_dll);
   REG(0x004, ier_dlm);
   REG(0x008, iir_fcr);
   REG(0x00C, lcr);
   REG(0x014, lsr);
   REG(0x01C, scr);
   REG(0x020, acr);
   REG(0x024, icr);
   REG(0x028, fdr);
   REG(0x030, ter);
};

namespace UART
{
   enum Baud
   {
      BAUD_9600,
      BAUD_19200,
      BAUD_38400,
      BAUD_57600,
      BAUD_74800,
      BAUD_115200
   };

   enum Parity
   {
      NONE,
      ODD,
      EVEN
   };
}

template <uint32_t BASE,
          unsigned POWER_BIT,
          unsigned CLOCK_BIT,
          unsigned FUNC,
          unsigned TX_PIN,
          unsigned RX_PIN>
class UART023 : public Periph<UART023Reg,BASE>
{
public:
   UART023(UART::Baud   baud,
           unsigned     data_bits = 8,
           UART::Parity parity    = UART::NONE,
           unsigned     stop_bits = 1)
   {
      SysCon syscon;
      PinCon pincon;

      // 1. Power up
      syscon.pconp(POWER_BIT, true);

      // 2. Select PCLK/1 clock
      if (CLOCK_BIT & (1<<6))
         syscon.pclksel1(CLOCK_BIT & 0x1F, 1);
      else
         syscon.pclksel0(CLOCK_BIT, 1);

      // 2. Enable pins
      pincon.config(TX_PIN, FUNC, PinCon::PULL_NONE);
      pincon.config(RX_PIN, FUNC, PinCon::PULL_NONE);

      // 4. UART divisor and fraction
      static const uint32_t FRAC_1_1_4   = ( 4 << 4) |  1;
      static const uint32_t FRAC_1_13_14 = (14 << 4) | 13;
      static const uint32_t FRAC_1_12_14 = (14 << 4) | 12;

      unsigned divisor;

      switch(baud)
      {
      default: // Fall through to 9600
      case UART::BAUD_9600:   divisor = 500; this->reg->fdr = FRAC_1_1_4;   break;
      case UART::BAUD_19200:  divisor = 250; this->reg->fdr = FRAC_1_1_4;   break;
      case UART::BAUD_38400:  divisor = 125; this->reg->fdr = FRAC_1_1_4;   break;
      case UART::BAUD_57600:  divisor =  54; this->reg->fdr = FRAC_1_13_14; break;
      case UART::BAUD_74800:  divisor =  42; this->reg->fdr = FRAC_1_12_14; break;
      case UART::BAUD_115200: divisor =  28; this->reg->fdr = FRAC_1_12_14; break;
      }

      this->reg->lcr.setBit(7); // DLAB - Enable access to divisor register
      this->reg->rbr_thr_dll = divisor & 0xFF;
      this->reg->ier_dlm     = divisor >> 8;
      this->reg->lcr.clrBit(7); // DLAB - Disable access to divisor register

      // 5. Configure protocol
      uint32_t lcr;
      switch (data_bits)
      {
      case 5: lcr = 0; break;
      case 6: lcr = 1; break;
      case 7: lcr = 2; break;
      default: case 8: lcr = 3; break;
      }

      if (stop_bits == 2)
         lcr |= 1<<2;

      switch (parity)
      {
      case UART::NONE: break;
      case UART::ODD:  lcr |= (0<<4) | (1<<3); break;
      case UART::EVEN: lcr |= (1<<4) | (1<<3); break;
      }
      this->reg->lcr = lcr;

      // 6. Enable FIFOs
      this->reg->iir_fcr.setBit(0);
   }

   void tx(uint8_t data)
   {
      while((this->reg->lsr & (1<<5)) == 0);

      this->reg->rbr_thr_dll = data;
   }

   uint8_t rx()
   {
      while((this->reg->lsr & (1<<0)) == 0);

      return this->reg->rbr_thr_dll;
   }
};

using UART0 = UART023<0x4000C000,  3,     6, 1,PIN_0_2, PIN_0_3>;
using UART2 = UART023<0x40098000, 24, 32+16, 1,PIN_0_10,PIN_0_11>;
using UART3 = UART023<0x4009C000, 25, 32+18, 3,PIN_0_0, PIN_0_1>;

} // namespace MTL
