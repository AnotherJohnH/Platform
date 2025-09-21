//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief MCP9808 I2C temperature sensor

#pragma once

#include <unistd.h>

#include "MTL/MTL.h"

namespace MTL {

template <typename I2C>
class TempSens_MCP9808
{
public:
   TempSens_MCP9808(uint8_t addr_ = 0b000)
   {
      i2c.setTarget(0x18 | addr_);
   }

   void start()
   {
      write16(REG_CONFIG, 0x0000);

      // Wait for 250ms to allow time for first conversion
      usleep(250 * 1000);

      // Seem to need another cycle for this to work !
      (void) read();
      usleep(250 * 1000);
   }

   void stop()
   {
      write16(REG_CONFIG, 0x0100);
   }

   //! Return temperature deg C * 256
   signed read()
   {
      // Scale up to 16-bit 2's complement
      int16_t value_fp7 = read16(REG_TEMP_AMB) << 3;

      return signed(value_fp7) << 1;
   }

private:
   void write16(uint8_t reg, uint16_t value)
   {
      i2c.write(reg,        /* stop */ false);
      i2c.write(value >> 8, /* stop */ false);
      i2c.write(value,      /* stop */ true);
   }

   uint16_t read16(uint8_t reg)
   {
      i2c.write(reg, /* stop */ false);

      uint8_t msb = i2c.read(/* stop */ false);
      uint8_t lsb = i2c.read(/* stop */ true);

      return (msb << 8) | lsb;
   }

   static const uint8_t REG_CONFIG   = 0x01;
   static const uint8_t REG_TEMP_AMB = 0x05;

   I2C i2c{};
};

}
