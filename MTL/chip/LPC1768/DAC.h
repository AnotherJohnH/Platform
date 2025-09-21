//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1768 DAC
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Config.h"
#include "MTL/Periph.h"

#include "SysCon.h"
#include "PinCon.h"

namespace MTL {

union DACReg
{
   REG(0x000, dacr);      //!< D/A converter
   REG(0x004, dacctrl);   //!< DAC control
   REG(0x008, daccntval); //!< DAC counter value
};

class DAC : public Periph<DACReg,0x4008C000>
{
public:
   DAC(unsigned freq, bool dma = false, bool bias_ = true)
   {
      SysCon syscon;
      PinCon pincon;

      syscon.pclksel0(PCLK_DAC, 1);

      pincon.config(PIN_0_26, 0b10, PinCon::PULL_NONE);

      if (bias_)
          bias = 1<<16;

      reg->daccntval = CLOCK_FREQ / freq;

      // Enable double buffering (DBLBUF_ENA) and counter (CNT_ENA)
      // and optionally DMA (DMA_ENA)
      reg->dacctrl = dma ? 0b1110
                         : 0b0110;

      send(0);
   }

   void send(int16_t value)
   {
      reg->dacr = bias | (value + 0x8000);
   }

private:
   static const unsigned PCLK_DAC = 22;

   uint32_t bias {0};
};

} // namespace MTL
