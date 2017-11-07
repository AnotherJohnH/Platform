//------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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
// \brief NXP LPC1768 General Purpose I/O

#ifndef LPC1768_GPIO_H
#define LPC1768_GPIO_H

#include "MTL/Periph.h"

#include "PinCon.h"


namespace MTL {

namespace Gpio {


struct Reg
{
   struct
   {
      Register<uint32_t>  dir;
      Register<uint32_t>  pad[3];
      Register<uint32_t>  mask;
      Register<uint32_t>  pin;
      Register<uint32_t>  set;
      Register<uint32_t>  clr;

   } fio[5];
};


template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<GpioReg,0x2009C000>
{
public:
   Out()
   {
      reg->fio[PORT].dir.setField(MSB, LSB, DATA_MASK);

      for(unsigned i=0; i<WIDTH; ++i)
      {
         PinCon().config(PIN + i, 0, PinCon::PULL_NONE);
      }
   }

   operator uint32_t() const
   {
      return reg->fio[PORT].pin.getField(MSB, LSB);
   }

   uint32_t operator=(uint32_t data)
   {
      reg->fio[PORT].pin.setField(MSB, LSB, data);
      return data;
   }

   void set(uint32_t data)
   {
      reg->fio[PORT].set = data << LSB;
   }

   void clr(uint32_t data)
   {
      reg->fio[PORT].clr = data << LSB;
   }

private:
   static const unsigned PORT      = PIN >> 5;
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};


template <unsigned WIDTH, unsigned PIN>
class In : public Periph<GpioReg,0x2009C000>
{
public:
   In()
   {
      reg->fio[PORT].dir.setField(MSB, LSB, 0);

      for(unsigned i=0; i<WIDTH; ++i)
      {
         PinCon().config(PIN + i, 0, PinCon::PULL_NONE);
      }
   }

   operator uint32_t() const
   {
      return reg->fio[PORT].pin.getField(MSB, LSB);
   }

private:
   static const unsigned PORT      = PIN >> 5;
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
};


} // namespace Gpio

} // namespace MTL

#endif // LPC1768_GPIO_H
