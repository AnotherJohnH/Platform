//------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
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

// \file  Gpio.h
// \brief nRF52 General Purpose I/O
//
// Data source Nordic Semiconductor "nRF52833 specification"

#ifndef MTL_NRF52_GPIO_H
#define MTL_NRF52_GPIO_H

#include "MTL/Periph.h"

namespace MTL {

namespace Gpio {

union Reg
{
   REG(0x504, out);
   REG(0x508, outset);
   REG(0x50C, outclr);
   REG(0x510, in);
   REG(0x514, dir);
   REG(0x518, dirset);
   REG(0x51C, dirclr);
   REG(0x520, latch);
   REG(0x524, detectmode);

   REG_ARRAY(0x700, pin_cnf, 32);
};

enum GpioDrive
{
   GPIO_S0_S1 = 0b000, // Standard   0, standard   1
   GPIO_H0_S1 = 0b001, // High drive 0, standard   1
   GPIO_S0_H1 = 0b010, // Standard   0, high drive 1
   GPIO_H0_H1 = 0b011, // high drive 0, high drive 1
   GPIO_D0_S1 = 0b100, // disconnect 0, standard   1
   GPIO_D0_H1 = 0b101, // disconnect 0, high drive 1
   GPIO_S0_D1 = 0b110, // standard   0, disconnect 1
   GPIO_H0_D1 = 0b111  // high drive 0, disconnect 1
};

template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<Reg, 0x50000000,0,0x300>
{
public:
   Out(GpioDrive drive=GPIO_S0_S1)
   {
      reg->dirset.setField(MSB, LSB, DATA_MASK);

      for(unsigned bit=LSB; bit<=MSB; ++bit)
      {
         reg->pin_cnf[bit] = (0     <<16) | // Sensing disabled
                             (drive << 8) | // Drive configuration
                             (0     << 2) | // No pull
                             (1     << 1) | // Disconnect input buffer
                             (1     << 0);  // Output
      }
   }

   operator uint32_t() const
   {
      return reg->out.getField(MSB, LSB);
   }

   uint32_t operator=(uint32_t data)
   {
      reg->out.setField(MSB, LSB, data);
      return data;
   }

   void set(uint32_t data)
   {
      reg->outset = data << LSB;
   }

   void clr(uint32_t data)
   {
      reg->outclr = data << LSB;
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};

template <unsigned WIDTH, unsigned PIN>
class In : public Periph<Reg, 0x50000000,0,0x300>
{
public:
   In(GpioDrive drive=GPIO_S0_S1)
   {
      reg->dirclr.setField(MSB, LSB, DATA_MASK);

      for(unsigned bit=LSB; bit<=MSB; ++bit)
      {
         reg->pin_cnf[bit] = (0     <<16) | // Sensing disabled
                             (drive << 8) | // Drive configuration
                             (0     << 2) | // No pull
                             (0     << 1) | // Disconnect input buffer
                             (0     << 0);  // Input
      }
   }

   operator uint32_t() const
   {
      return reg->in.getField(MSB, LSB);
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};

} // namespace Gpio

} // namespace MTL

#endif // MTL_NRF52_GPIO_H
