//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1114 General Purpose I/O
//
// Data source NXP document "LPC111X User Manual UM10398"

#pragma once

#include "MTL/Periph.h"

#include "IoCon.h"

namespace MTL {

namespace Gpio {

union Reg
{
   REG_ARRAY(0x000, data, 0x1000); //!< Data
   REG(0x8000, dir);               //!< Direction
   REG(0x8004, is);                //!< Interrupt sense
   REG(0x8008, ibe);               //!< Interrupt both edges
   REG(0x800C, iev);               //!< Interrupt event
   REG(0x8010, ie);                //!< Interrupt mask
   REG(0x8014, ris);               //!< Raw interrupt status
   REG(0x8018, mis);               //!< Masked inetrrupt status
   REG(0x801C, ic);                //!< Interrupt clear
};

template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<Reg,0x50000000>
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
      return reg->data[REG_MASK].getField(MSB, LSB);
   }

   uint32_t operator=(uint32_t data)
   {
      reg->data[REG_MASK].setField(MSB, LSB, data);
      return data;
   }

   void set(uint32_t data)
   {
      reg->data[REG_MASK].setField(MSB, LSB, data);
   }

   void clr(uint32_t data)
   {
      reg->data[REG_MASK].setField(MSB, LSB, ~data);
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
   static const uint32_t REG_MASK  = DATA_MASK << LSB;
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
      return reg->data[REG_MASK].getField(MSB, LSB);
   }

private:
   static const unsigned LSB       = PIN & 0x1F;
   static const unsigned MSB       = LSB + WIDTH - 1;
   static const uint32_t DATA_MASK = (1<<WIDTH) - 1;
   static const uint32_t REG_MASK  = DATA_MASK << LSB;
};


} // namespace Gpio

} // namespace MTL
