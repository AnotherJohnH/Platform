//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
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
   REG_TYPE_ARRAY(0x0000, uint8_t, b0, 24); //!< Byte pin register
   REG_TYPE_ARRAY(0x0020, uint8_t, b1, 32); //!< Byte pin register
   REG_ARRAY(0x1000, w0,  24);              //!< Word pin register
   REG_ARRAY(0x1080, w1,  32);              //!< Word pin register
   REG_ARRAY(0x2000, dir,  2);              //!< Direction
   REG_ARRAY(0x2080, mask, 2);              //!< Mask register
   REG_ARRAY(0x2100, pin,  2);              //!< Bit pin register
   REG_ARRAY(0x2180, mpin, 2);              //!< Masked pin register
   REG_ARRAY(0x2200, set,  2);              //!< Bit set register
   REG_ARRAY(0x2280, clr,  2);              //!< Bit clear register
   REG_ARRAY(0x2300, tgl,  2);              //!< Bit toggle register
};

template <unsigned WIDTH, unsigned PIN>
class Out : public Periph<Reg,0x50000000>
{
public:
   Out()
   {
      reg->dir[PORT].setField(MSB, LSB, DATA_MASK);

      for(unsigned i=0; i<WIDTH; ++i)
      {
         IoCon().config(PIN + i, 0, IoCon::PULL_NONE);
      }
   }

   operator uint32_t() const
   {
      return reg->pin[PORT].getField(MSB, LSB);
   }

   uint32_t operator=(uint32_t data)
   {
      reg->pin[PORT].setField(MSB, LSB, data);
      return data;
   }

   void set(uint32_t data)
   {
      reg->set[PORT] = data << LSB;
   }

   void clr(uint32_t data)
   {
      reg->clr[PORT] = data << LSB;
   }

private:
   static const unsigned PORT      = PIN >> 5;
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
      reg->dir[PORT].setField(MSB, LSB, 0);

      for(unsigned i=0; i<WIDTH; ++i)
      {
         IoCon().config(PIN + i, 0, IoCon::PULL_NONE);
      }
   }

   operator uint32_t() const
   {
      return reg->pin[PORT].getField(MSB, LSB);
   }

private:
   static const unsigned PORT = PIN >> 5;
   static const unsigned LSB  = PIN & 0x1F;
   static const unsigned MSB  = LSB + WIDTH - 1;
};

} // namespace Gpio

} // namespace MTL
