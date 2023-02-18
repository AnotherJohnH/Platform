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

// \brief NXP LPC11U24 System Controller
//
// Data source NXP document "LPC11U3X-2X-1X User Manual UM10462"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union SysConReg
{
   REG(0x000, symemremap);
   REG(0x004, presetctrl);
   REG(0x008, syspllctrl);
   REG(0x00C, syspllstat);
   REG(0x010, usbpllctrl);
   REG(0x014, usbpllstat);
   REG(0x020, sysoscctrl);
   REG(0x024, wdtoscctrl);
   REG(0x028, ircctrl);
   REG(0x030, sysrststat);
   REG(0x040, syspllclksel);
   REG(0x044, syspllclkuen);
   REG(0x048, usbpllclksel);
   REG(0x04C, usbpllclkuen);
   REG(0x070, mainclksel);
   REG(0x074, mainclkuen);
   REG(0x078, sysahbclkdiv);
   REG(0x080, sysahbclkctrl);
   REG(0x094, ssp0clkdiv);
   REG(0x098, uartclkdiv);
   REG(0x09C, ssp1clkdiv);
   REG(0x0c0, usbclksel);
   REG(0x0c4, usbclkuen);
   REG(0x0c8, usbclkdiv);
   REG(0x0e0, clkoutsel);
   REG(0x0e4, clkoutuen);
   REG(0x0e8, clkoutdiv);

   REG(0x238, pdruncfg);
};

class SysCon : public Periph<SysConReg,0x40048000>
{
public:
   void init()
   {
      // Power up the crystal oscillator TODO redundant?
      reg->pdruncfg.clrBit(5);

      // Not bypassed and 1-20 MHz range TODO redundant?
      reg->sysoscctrl = 0;

      // Select crystal oscillator TODO redunadnt?
      reg->syspllclksel = 1;

      // and ensure update with a rising edge
      reg->syspllclkuen = 1;
      reg->syspllclkuen = 0;
      reg->syspllclkuen = 1;

      // Set System PLL to generate desired system clock
      // PSEL=1 (P=2), MSEL=3 (M=4)
      reg->syspllctrl = (1<<5) | 3;

      // Power up System PLL
      reg->pdruncfg.clrBit(7);

      // wait for PLL to lock
      while(reg->syspllstat == 0);

      // Select System PLL as main clock source
      reg->mainclksel = 3;
      reg->mainclkuen = 1;
      reg->mainclkuen = 0;
      reg->mainclkuen = 1;

      // 
      reg->sysahbclkdiv = 1;

      // Enable clock for GPIO and IOCON
      enableAHBClkCtrl(6);
      enableAHBClkCtrl(16);
   }

   void enableAHBClkCtrl(unsigned bit)
   {
      reg->sysahbclkctrl.setBit(bit);
   }

   void enablePResetCtrl(unsigned bit)
   {
      reg->presetctrl.setBit(bit);
   }
};

} // namespace MTL
