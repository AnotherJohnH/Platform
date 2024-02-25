//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------
// \brief MCP9808 I2C temperature sensor

#pragma once

#include "MTL/MTL.h"

#include "MTL/rp2040/I2C.h"

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
