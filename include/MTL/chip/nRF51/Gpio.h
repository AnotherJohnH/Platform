//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

// \file Gpio.h
// \brief nRF51 General Purpose I/O
//
// Data source Nordic Semiconductor "nRF51 Series Reference Manual Version 3.0"

#ifndef NRF51_GPIO_H
#define NRF51_GPIO_H

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

   REG_ARRAY(0x700, pin_cnf, 32);
};


enum GpioDrive
{
   GPIO_S0_S1 = 0,
   GPIO_H0_S1 = 1,
   GPIO_S0_H1 = 2,
   GPIO_H0_H1 = 3,
   GPIO_D0_S1 = 4,
   GPIO_D0_H1 = 5,
   GPIO_S0_D1 = 6,
   GPIO_H0_D1 = 7
};


template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<Reg,0x50000000>
{
public:
   Out(GpioDrive drive=GPIO_S0_S1)
   {
      reg->dirset.setField(MSB, LSB, DATA_MASK);

      for(unsigned bit=LSB; bit<=MSB; ++bit)
      {
         reg->pin_cnf[bit] = (0     <<16) |
                             (drive << 8) |
                             (0     << 2) |
                             (1     << 1) |
                             (1     << 0);
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
   static const unsigned LSB = PIN & 0x1F;
   static const unsigned MSB = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};


template <unsigned WIDTH, unsigned PIN>
class In : public Periph<Reg,0x50000000>
{
public:
   In(GpioDrive drive=GPIO_S0_S1)
   {
      reg->dirclr.setField(MSB, LSB, DATA_MASK);

      for(unsigned bit=LSB; bit<=MSB; ++bit)
      {
         reg->pin_cnf[bit] = (0     <<16) |
                             (drive << 8) |
                             (0     << 2) |
                             (0     << 1) |
                             (0     << 0);
      }
   }

   operator uint32_t() const
   {
      return reg->in.getField(MSB, LSB);
   }

private:
   static const unsigned LSB = PIN & 0x1F;
   static const unsigned MSB = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};


} // namespace Gpio

} // namespace MTL

#endif // NRF51_GPIO_H
