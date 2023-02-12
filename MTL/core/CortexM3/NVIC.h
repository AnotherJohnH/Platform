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

//! \brief Access to Cortex-M3 NVIC

#pragma once

#include "MTL/Periph.h"

namespace MTL {

union NvicReg
{
   REG(0x004,       ict);
   REG_ARRAY(0x100, iser, 2);
   REG_ARRAY(0x180, icer, 2);
   REG_ARRAY(0x200, ispr, 2);
   REG_ARRAY(0x280, icpr, 2);
   REG_ARRAY(0x300, iabr, 2);
   REG_ARRAY(0x400, ipri, 8);
   REG(0xf00,       stir);
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
