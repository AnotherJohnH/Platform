//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1768 General Purpose I/O
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

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
class Out : public Periph<Reg,0x2009C000>
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
class In : public Periph<Reg,0x2009C000>
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
