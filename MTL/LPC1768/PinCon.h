//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1768 pin connection peripheral
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include "MTL/Periph.h"
#include "MTL/LPC1768/Pins.h"

namespace MTL {

union PinConReg
{
   REG_ARRAY(0x000, pinsel,     11);
   REG_ARRAY(0x040, pinmode,    11);
   REG_ARRAY(0x068, pinmode_od,  5);
   REG(      0x07C, i2cpadcfg);
};

class PinCon : public Periph<PinConReg,0x4002C000>
{
public:
   enum Mode
   {
      PULL_UP   = 0,
      REPEAT    = 1,
      PULL_NONE = 2,
      PULL_DOWN = 3
   };

   void config(uint32_t pin, unsigned func, Mode mode)
   {
      unsigned index = pin >> 4;
      unsigned lsb   = (pin & 0xF)<<1;

      reg->pinsel[index].setField( lsb + 1, lsb, func);
      reg->pinmode[index].setField(lsb + 1, lsb, mode);
   }
};

} // namespace MTL
