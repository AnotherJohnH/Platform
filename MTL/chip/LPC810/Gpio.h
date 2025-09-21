//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC810 General Purpose I/O
//
// Data source NXP document "LPC81X User Manual UM10601"

#pragma once

#include "MTL/Periph.h"

#include "IoCon.h"

namespace MTL {

namespace Gpio {

union Reg
{
   REG_TYPE_ARRAY(0x0000,  uint8_t, byte, 18); //!< Byte pin register
   REG_TYPE_ARRAY(0x1000, uint32_t, word, 18); //!< Word pin register
   REG(0x2000, dir);                           //!< Direction
   REG(0x2080, mask);                          //!< Mask register
   REG(0x2100, pin);                           //!< Bit pin register
   REG(0x2180, mpin);                          //!< Masked pin register
   REG(0x2200, set);                           //!< Bit set register
   REG(0x2280, clr);                           //!< Bit clear register
   REG(0x2300, tgl);                           //!< Bit toggle register
};

template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<Reg,0xA0000000>
{
public:
   Out()
   {
      reg->dir.setField(MSB, LSB, DATA_MASK);

      for(unsigned i=0; i<WIDTH; ++i)
      {
         IoCon().config(PIN + i, 0, IoCon::PULL_NONE);
      }
   }

   operator uint32_t() const
   {
      return reg->pin.getField(MSB, LSB);
   }

   uint32_t operator=(uint32_t data)
   {
      reg->pin.setField(MSB, LSB, data);
      return data;
   }

   void set(uint32_t data)
   {
      reg->set = data << LSB;
   }

   void clr(uint32_t data)
   {
      reg->clr = data << LSB;
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
};


template <unsigned WIDTH, unsigned PIN>
class In : public Periph<Reg,0x50000000>
{
public:
   In()
   {
      reg->dir.setField(MSB, LSB, 0);

      for(unsigned i=0; i<WIDTH; ++i)
      {
         IoCon().config(PIN + i, 0, IoCon::PULL_NONE);
      }
   }

   operator uint32_t() const
   {
      return reg->pin.getField(MSB, LSB);
   }

private:
   static const unsigned LSB = PIN & 0x1F;
   static const unsigned MSB = LSB + WIDTH - 1;
};


} // namespace Gpio

} // namespace MTL
