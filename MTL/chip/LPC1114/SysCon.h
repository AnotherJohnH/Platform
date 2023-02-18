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

// \brief NXP LPC1114 System Configuration
//
// Data source NXP document "LPC111x User Manual UM10398"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union SysConReg
{
   REG(0x000, symemremap);
   REG(0x004, presetctrl);
   REG(0x008, syspllctrl);
   REG(0x00C, syspllstat);
   REG(0x020, sysoscctrl);
   REG(0x024, wdtoscctrl);
   REG(0x028, ircctrl);
   REG(0x030, sysrststat);
   REG(0x040, syspllclksel);
   REG(0x044, syspllclkuen);
   REG(0x070, mainclksel);
   REG(0x074, mainclkuen);
   REG(0x078, sysahbclkdiv);
   REG(0x080, sysahbclkctrl);
   REG(0x094, ssp0clkdiv);
   REG(0x098, uartclkdiv);
   REG(0x09C, ssp1clkdiv);
   REG(0x0D0, wdtclksel);
   REG(0x0D4, wdtclkuen);
   REG(0x0D8, wdtclkdiv);
   REG(0x0E0, clkoutsel);
   REG(0x0E4, clkoutuen);
   REG(0x0E8, clkoutdiv);
   REG(0x100, pioporcap0);
   REG(0x104, pioporcap1);
   REG(0x150, bodctrl);
   REG(0x154, systckcal);
   REG(0x170, irqlatency);
   REG(0x174, nmisrc);
   REG(0x200, startaprp0);
   REG(0x204, starterp0);
   REG(0x208, startrsrp0clr);
   REG(0x20C, startsrp0);
   REG(0x230, pdsleepcfg);
   REG(0x234, pdawakecfg);
   REG(0x238, pdruncfg);
   REG(0x3f4, device_id);
};

class SysCon : public Periph<SysConReg,0x40048000>
{
public:
   void init()
   {
      // Set System PLL to generate desired system clock
      reg->syspllctrl = (1<<5) | 4;    // sysclock = 60 MHz  M=5  P=2  FCCO=240 MHz !!
      //reg->syspllctrl = (1<<5) | 3;     // sysclock = 48 MHz  M=4  P=2  FCCO=192 MHz
      //reg->syspllctrl = (2<<5) | 2;     // sysclock = 36 MHz  M=3  P=4  FCCO=288 MHz
      //reg->syspllctrl = (2<<5) | 1;     // sysclock = 24 MHz  M=2  P=4  FCCO=192 MHz
      //reg->syspllctrl = (3<<5) | 0;     // sysclock = 12 MHz  M=1  P=8  FCCO=192 MHz

      // Power up System PLL
      reg->pdruncfg.clrBit(7);

      // wait for PLL to lock
      while(reg->syspllstat == 0);

      // Select System PLL as main clock source
      reg->mainclksel = 3;
      reg->mainclkuen = 0;
      reg->mainclkuen = 1;

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
