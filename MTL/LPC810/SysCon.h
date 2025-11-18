//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC810 System Controller
//
// Data source NXP document "LPC81X User Manual UM10601"

#pragma once

#include "MTL/Periph.h"

#include "SwitchMatrix.h"

namespace MTL {

union SysConReg
{
   REG(0x000, symemremap);
   REG(0x004, presetctrl);
   REG(0x008, syspllctrl);
   REG(0x00C, syspllstat);
   REG(0x020, sysoscctrl);
   REG(0x024, wdtoscctrl);
   REG(0x030, sysrststat);
   REG(0x040, syspllclksel);
   REG(0x044, syspllclkuen);
   REG(0x070, mainclksel);
   REG(0x074, mainclkuen);
   REG(0x078, sysahbclkdiv);
   REG(0x080, sysahbclkctrl);
   REG(0x094, uartclkdiv);
   REG(0x0E0, clkoutsel);
   REG(0x0E4, clkoutuen);
   REG(0x0E8, clkoutdiv);
   REG(0x0F0, uartfrgdiv);
   REG(0x0F4, uartfrgmult);
   REG(0x0FC, exttracecmd);
   REG(0x100, pioporcap0);
   REG(0x134, ioconclkdiv6);
   REG(0x138, ioconclkdiv5);
   REG(0x13C, ioconclkdiv4);
   REG(0x140, ioconclkdiv3);
   REG(0x144, ioconclkdiv2);
   REG(0x148, ioconclkdiv1);
   REG(0x14C, ioconclkdiv0);
   REG(0x150, bodctlr);
   REG(0x154, systckcal);
   REG(0x170, irqlatency);
   REG(0x174, nmisrc);
   REG(0x178, pintsel0);
   REG(0x17C, pintsel1);
   REG(0x180, pintsel2);
   REG(0x184, pintsel3);
   REG(0x188, pintsel4);
   REG(0x18C, pintsel5);
   REG(0x190, pintsel6);
   REG(0x194, pintsel7);
   REG(0x204, starterp0);
   REG(0x214, starterp1);
   REG(0x230, pdsleepcfg);
   REG(0x234, pdawakecfg);
   REG(0x238, pdruncfg);
   REG(0x3F8, device_id);
};

class SysCon : public Periph<SysConReg,0x40048000>
{
public:
   void init()
   {
      // Reset values
      //   syscon->pdruncfg     |= 1<<7;    // Power PLL off
      //   syscon->syspllclksel = 0;        // System PLL clock is 12MHz IRC
      //   syscon->syspllclkuen = 1;        // System PLL clock update enable

      reg->syspllctrl   = (1<<6) | 4;    // MSEL=4 (M=5) PSEL=1 (P=2)
      reg->sysahbclkdiv = 2;             // System AHB clock divider

      reg->pdruncfg.clrBit(7);           // Power PLL on

      while((reg->syspllstat & 1) == 0); // Wait for PLL lock

      reg->mainclksel   = 1;             // Main clock is PLL 
      reg->mainclkuen   = 1;             // Main clock update enable

      reg->sysahbclkctrl.setBit(7);      // Enable SWM

      SwitchMatrix swm;

      swm.init();
   }
};

} // namespace MTL
