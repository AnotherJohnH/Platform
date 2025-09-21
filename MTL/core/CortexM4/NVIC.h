//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file  NVIC.h
//! \brief Access to Cortex-M4 NVIC

#pragma once

#include "MTL/Periph.h"

namespace MTL {


union NvicReg
{
   REG(0x004,       ictr);
   REG_ARRAY(0x100, iser, 8);
   REG_ARRAY(0x180, icer, 8);
   REG_ARRAY(0x200, ispr, 8);
   REG_ARRAY(0x280, icpr, 8);
   REG_ARRAY(0x300, iabr, 8);
   REG_ARRAY(0x400, ipri, 8);
};


template <unsigned IRQ>
class NVIC : public Periph<NvicReg,0xE000E000>
{
private:
   static const unsigned BIT       = IRQ & 0x1F;
   static const uint32_t BIT_MASK  = 1<<BIT;
   static const unsigned INDEX     = IRQ / 32;
   static const unsigned PRI_INDEX = IRQ / 4;
   static const unsigned PRI_LSB   = (IRQ & 3) * 8;
   static const unsigned PRI_MSB   = PRI_LSB + 7;

public:
   //! Check if interrupt is enabled
   bool isEnabled() const
   {
      return reg->iser[INDEX].getBit(BIT);
   }

   //! Check if interrupt is pending
   bool isPending() const
   {
      return reg->ispr[INDEX].getBit(BIT);
   }

   //! Check if interrupt is active
   bool isActive() const
   {
      return reg->iabr[INDEX].getBit(BIT);
   }

   //! Get priority
   uint8_t getPriority() const
   {
      return uint8_t(reg->ipri[PRI_INDEX].getField(PRI_MSB, PRI_LSB));
   }

   //! Set priority
   //
   //! \param priority, 0 is highest, 255 is lowest
   void setPriority(uint8_t priority)
   {
      reg->ipri[PRI_INDEX].setField(PRI_MSB, PRI_LSB, priority);
   }

   //! Enable interrupt
   void enable()     { reg->iser[INDEX] = BIT_MASK; }

   //! Disable interrupt
   void disable()    { reg->icer[INDEX] = BIT_MASK; }

   //! Set interrupt pending
   void setPending() { reg->ispr[INDEX] = BIT_MASK; }

   //! Clear pending interrupt
   void clrPending() { reg->icpr[INDEX] = BIT_MASK; }
};

} // namespace MTL

