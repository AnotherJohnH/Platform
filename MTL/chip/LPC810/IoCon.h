//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC810 I/O Controller
//
// Data source NXP document "LPC81X User Manual UM10601"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union IoConReg
{
   REG_ARRAY(0x000, pio, 18);
};

class IoCon : public Periph<IoConReg,0x40044000>
{
public:
   enum Mode
   {
      PULL_NONE = 0,    // HI-Z
      PULL_DOWN = 1,
      PULL_UP   = 2,
      REPEAT    = 3
   };

   void config(unsigned pin, unsigned func, Mode mode,
               bool hys = false, bool inv = false, bool od = false)
   {
      // TODO this is LPC11U24 code needs checking at very list
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
