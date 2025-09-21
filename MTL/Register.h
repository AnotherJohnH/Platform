//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Memory mapped register helper

#pragma once

//! Bare metal layer
namespace MTL {

//! Generic register access
template <typename TYPE>
class Register
{
public:
   //! Read all the register state
   operator TYPE() const volatile
   {
      return state;
   }

   //! Write all the register state
   TYPE operator=(TYPE value) volatile
   {
      state = value;
      return value;
   }

   //! Read a bit field
   TYPE getField(unsigned msb, unsigned lsb) const volatile
   {
      return (state & mask(msb, lsb)) >> lsb;
   }

   //! Write a bit field
   void setField(unsigned msb, unsigned lsb, TYPE value) volatile
   {
      state = (state & ~mask(msb, lsb)) | (value << lsb);
   }

   //! Read a single bit
   bool getBit(unsigned bit) const volatile
   {
      return (mask(bit) & state) != 0;
   }

   //! Set a single bit
   void setBit(unsigned bit, bool value = true) volatile
   {
      if(value)
         state |= mask(bit);
      else
         state &= ~mask(bit);
   }

   //! Clear a single bit
   void clrBit(unsigned bit) volatile
   {
      setBit(bit, false);
   }

   //! Set some bits
   void setBits(unsigned msb, unsigned lsb, TYPE mask_) volatile
   {
      state = state | (mask_ << lsb);
   }

   //! Clear some bits
   void clrBits(unsigned msb, unsigned lsb, TYPE mask_) volatile
   {
      state = state & ~(mask_ << lsb);
   }

private:
   TYPE state;

   // single bit mask
   static TYPE mask(unsigned bit_num)
   {
      return TYPE(1)<<bit_num;
   }

   // field mask
   static TYPE mask(unsigned msb, unsigned lsb)
   {
      TYPE mask = (1 << (msb - lsb + 1)) - 1;
      return mask << lsb;
   }
};

} // namespace MTL
