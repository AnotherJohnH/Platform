//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief RP2040 RESETS peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct ResetsReg
{
   uint32_t reset;
   uint32_t wdsel;
   uint32_t reset_done;
};

class Resets : public Periph<ResetsReg, 0x4000C000>
{
public:
   //! Remove reset from peripherals and wait for done
   void clrReset(uint32_t mask)
   {
      reg->reset &= ~mask;

      while((reg->reset_done & mask) != mask);
   }

   //! Set reset for peripherals
   void setReset(uint32_t mask)
   {
      reg->reset |= mask;
   }

   //! Set and then clear reset for peripherals
   void tglReset(uint32_t mask)
   {
      setReset(mask);
      clrReset(mask);
   }

   // Peripheral bit masks
   static const uint32_t USBCTRL    = 1 << 24;

   static const uint32_t UART1      = 1 << 23;
   static const uint32_t UART0      = 1 << 22;
   static const uint32_t TIMER      = 1 << 21;
   static const uint32_t TBMAN      = 1 << 20;

   static const uint32_t SYSINFO    = 1 << 19;
   static const uint32_t SYSCFG     = 1 << 18;
   static const uint32_t SPI1       = 1 << 17;
   static const uint32_t SPI0       = 1 << 16;

   static const uint32_t RTC        = 1 << 15;
   static const uint32_t PWM        = 1 << 14;
   static const uint32_t PLL_USB    = 1 << 13;
   static const uint32_t PLL_SYS    = 1 << 12;

   static const uint32_t PIO1       = 1 << 11;
   static const uint32_t PIO0       = 1 << 10;
   static const uint32_t PADS_QSPI  = 1 <<  9;
   static const uint32_t PADS_BANK0 = 1 <<  8;

   static const uint32_t JTAG       = 1 <<  7;
   static const uint32_t IO_QSPI    = 1 <<  6;
   static const uint32_t IO_BANK0   = 1 <<  5;
   static const uint32_t I2C1       = 1 <<  4;

   static const uint32_t I2C0       = 1 <<  3;
   static const uint32_t DMA        = 1 <<  2;
   static const uint32_t BUSCTRL    = 1 <<  1;
   static const uint32_t ADC        = 1 <<  0;

   static const uint32_t ALL        = 0x1FFFFFF;
};

} // namespace MTL
