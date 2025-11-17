//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1768 System Controller peripheral
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union SysConReg
{
   // External interrupts
   REG(0x140, extint);
   REG(0x148, extmode);
   REG(0x14C, extpolar);

   // Reset 
   REG(0x180, rsid);

   // Misc
   REG(0x1A0, scs);

   // Clocking and power control
   REG(0x10C, clksrcsel);

   REG(0x080, pll0con);
   REG(0x084, pll0cfg);
   REG(0x088, pll0stat);
   REG(0x08C, pll0feed);

   REG(0x0A0, pll1con);
   REG(0x0A4, pll1cfg);
   REG(0x0A8, pll1stat);
   REG(0x0AC, pll1feed);

   REG(0x104, cclkcfg);
   REG(0x108, usbclkcfg);
   REG(0x1A8, pclksel0);
   REG(0x1AC, pclksel1);

   REG(0x0C0, pcon);
   REG(0x0C4, pconp);

   REG(0x1C8, clkoutcfg);
};


class SysCon : public Periph<SysConReg,0x400FC000>
{
public:
   // M 
   // Fcco      = (2 * M * Fin) / N
   // CPU clock = Fcco / D
   void init(unsigned N=1, unsigned M=12, unsigned D=3)
   {
      // START THE MAIN OSCILLATOR

      // Enable main (driven by external XTAL) oscillator
      reg->scs.setBit(5);

      // Wait for main oscillator to be ready
      while(!reg->scs.getBit(6));

      // Configure division PLL0 clock
      reg->cclkcfg = D-1;

      // Select the main oscillator as input to PLL0
      reg->clksrcsel = 1;

      // CONFIGURE, ENABLE AND START PLL0

      // Configure PLL0
      reg->pll0cfg  = ((N-1)<<16) | ((M-1)<<0);
      reg->pll0feed = 0xAA;
      reg->pll0feed = 0x55;

      // Enable PLL0
      reg->pll0con  = 1<<0;
      reg->pll0feed = 0xAA;
      reg->pll0feed = 0x55;

      // Wait for PLL0 locked
      while(!reg->pll0stat.getBit(26));

      // Connect PLL0
      reg->pll0con  = (1<<1) | (1<<0);
      reg->pll0feed = 0xAA;
      reg->pll0feed = 0x55;

      // Wait for PLL0 connected and enabled
      while(reg->pll0stat.getField(25, 24) != 3);
   }

   void pconp(unsigned bit, bool state)
   {
      reg->pconp.setBit(bit, state);
   }

   void pclksel0(unsigned bit, unsigned value)
   {
      reg->pclksel0.setField(bit + 1, bit, value);
   }

   void pclksel1(unsigned bit, unsigned value)
   {
      reg->pclksel1.setField(bit + 1, bit, value);
   }
};


} // namespace MTL
