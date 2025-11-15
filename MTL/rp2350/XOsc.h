//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2350 XOSC peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct XOscReg
{
   uint32_t ctrl;
   uint32_t status;
   uint32_t dormant;
   uint32_t startup;
   uint32_t count;
};

class XOsc : public Periph<XOscReg, 0x40048000>
{
public:
   //! Start the XTAL oscillator
   void start()
   {
      reg->ctrl    = CTRL_FREQ_RANGE_1_15MHZ;
      reg->startup = ((XTAL_FREQ_MHZ * 1000) + 128) >> 8;

      // enable
      reg->ctrl    = (0xFAB << 12) | CTRL_FREQ_RANGE_1_15MHZ;

      // Wait until stable
      while((reg->status & (1<<12)) == 0);
   }

   static const unsigned XTAL_FREQ_MHZ           = 12;
   static const uint32_t CTRL_FREQ_RANGE_1_15MHZ = 0xAA0;
};

} // namespace MTL
