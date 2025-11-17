//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC11U24 I/O Controller
//
// Data source NXP document "LPC11U3X-2X-1X User Manual UM10462"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union IoConReg
{
   REG_ARRAY(0x000, pio, 56);
};

class IoCon : public Periph<IoConReg,0x40044000>
{
public:
   enum Func : unsigned
   {
      I2C_SCL   = 1, // use with PIO0_4
      I2C_SDA   = 1, // use with PIO0_5
   };

   enum Mode
   {
      PULL_NONE = 0,
      PULL_DOWN = 1,
      PULL_UP   = 2,
      REPEAT    = 3,

      STANDARD_I2C = 0
   };

   void config(unsigned pin,
               unsigned func,
               Mode     mode,
               bool     hys = false,
               bool     inv = false,
               bool     od  = false)
   {
      unsigned port  = pin >> 5;
      unsigned bit   = pin & 0x1F;
      unsigned index = port * 24 + bit;

      uint32_t data = reg->pio[index];

      data = (data & 0xFFFFFB80) |
             (func <<  0) |
             (mode <<  3) |
             (hys  <<  5) |
             (inv  <<  6) |
             (od   << 10);

      reg->pio[index] = data;
   }
};

} // namespace MTL
