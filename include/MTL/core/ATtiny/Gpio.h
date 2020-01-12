//------------------------------------------------------------------------------
// Copyright (c) 2011-2020 John D. Haughton
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
// \brief Atmel tiny AVR General Purpose I/O
//
// Data source Atmel document ""

#ifndef ATTINY_GPIO_H
#define ATTINY_GPIO_H

#include "MTL/Periph.h"

namespace MTL {

namespace Gpio {

struct Reg
{
   Register<uint8_t> pin;
   Register<uint8_t> ddr;
   Register<uint8_t> port;
};

template <unsigned WIDTH, unsigned PIN>
class Out : private Periph<Reg,PIN & 0xFF>
{
public:
   Out()
   {
      this->reg->port.setBits(MSB, LSB, DATA_MASK);
      this->reg->ddr.setBits( MSB, LSB, DATA_MASK);
   }

   operator uint8_t() const
   {
      return this->reg->pin.getField(MSB, LSB);
   }

   uint8_t operator=(uint8_t data)
   {
      this->reg->port.setField(MSB, LSB, data);
      return data;
   }

   void set(uint8_t mask)
   {
      this->reg->port.setField(MSB, LSB, this->reg->port.getField(MSB, LSB) | mask);
   }

   void clr(uint8_t mask)
   {
      this->reg->port.setField(MSB, LSB, this->reg->port.getField(MSB, LSB) & ~mask);
   }

private:
   static const unsigned LSB       = PIN >> 8;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint8_t  DATA_MASK = (1<<WIDTH) - 1;
};


template <unsigned WIDTH, unsigned PIN>
class In : Periph<Reg,PIN & 0xFF>
{
public:
   In()
   {
      this->reg->ddr.clrBits(MSB, LSB, DATA_MASK);
      // Set port register bits for pull-up
   }

   operator uint8_t() const
   {
      return this->reg->pin.getField(MSB, LSB);
   }

private:
   static const unsigned LSB       = PIN >> 8;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint8_t  DATA_MASK = (1<<WIDTH) - 1;
};

} // namespace Gpio

} // namespace MTL

#endif // ATTINY_GPIO_H
