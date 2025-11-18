//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC11U24 I2C Bus Controller
//
// Data source NXP document "LPC11U3X-2X-1X User Manual UM10462"

#pragma once

#include "MTL/Periph.h"

#include "IoCon.h"
#include "SysCon.h"

namespace MTL {

union I2CReg
{
   REG(0x000, conset);
   REG(0x004, stat);
   REG(0x008, dat);
   REG(0x00C, adr0);
   REG(0x010, sclh);
   REG(0x014, scll);
   REG(0x018, conclr);
   REG(0x01C, mmctrl);
   REG(0x020, adr1);
   REG(0x024, adr2);
   REG(0x028, adr3);
   REG(0x02C, data_buffer);
   REG(0x030, mask0);
   REG(0x034, mask1);
   REG(0x038, mask2);
   REG(0x03C, mask3);
};

class I2C : public Periph<I2CReg,0x40000000>
{
public:
   void init()
   {
      IoCon  iocon;
      SysCon syscon;

      // 1. Configure Pins
      iocon.config(PIN_0_4, IoCon::I2C_SCL, IoCon::STANDARD_I2C);
      iocon.config(PIN_0_5, IoCon::I2C_SDA, IoCon::STANDARD_I2C);

      // 2. Enable I2C Power and peripheral clock
      syscon.enableAHBClkCtrl(5);

      // 3. De-assert I2C Reset
      syscon.enablePresetCtrl(1);
   }
};

} // namespace MTL
