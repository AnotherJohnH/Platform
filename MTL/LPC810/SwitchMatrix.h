//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC810 Switch Matrix
//
// Data source NXP document "LPC81X User Manual UM10601"

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union SwitchMatrixReg
{
   REG_ARRAY(0x000, pinassign, 9);
   REG(0x1C0, pinenable);
};

class SwitchMatrix : public Periph<SwitchMatrixReg,0x4000C000>
{
public:
   void init()
   {
      // Disable SWDIO SWCLK
      reg->pinenable = reg->pinenable | (1<<3) | (1<<2);
   }
};

} // namespace MTL
