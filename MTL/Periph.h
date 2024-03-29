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

//! \brief Memory mapped peripheral helper

#pragma once

#include <cstdint>

#include "MTL/Register.h"

#define  REG_TYPE(OFFSET, TYPE, NAME)  \
    struct { uint8_t pad_##NAME[OFFSET]; TYPE NAME; }

#define  REG_TYPE_ARRAY(OFFSET, TYPE, NAME, SIZE)  \
    struct { uint8_t pad_##NAME[OFFSET]; TYPE NAME[SIZE]; }

#define  REG(OFFSET, NAME) \
    REG_TYPE(OFFSET, MTL::Register<uint32_t>, NAME)

#define  REG_ARRAY(OFFSET, NAME, SIZE)  \
    REG_TYPE_ARRAY(OFFSET, MTL::Register<uint32_t>, NAME, SIZE)

namespace MTL {

//! Base class for memory mapped peripherals
template <typename REG_TYPE,
          uint32_t BASE_ADDR,
          unsigned INSTANCE=0,
          uint32_t SIZE = 0x1000>
class Periph
{
protected:
   const uint32_t PERIPH_ADDR = BASE_ADDR + INSTANCE*SIZE;

public:
   volatile REG_TYPE* const reg = (volatile REG_TYPE*)(PERIPH_ADDR);

   //! Register helper to set a field
   void setField(volatile uint32_t& reg, unsigned msb, unsigned lsb, unsigned data)
   {
      uint32_t mask = (1 << (msb - lsb + 1)) - 1;

      reg = (reg & ~(mask << lsb)) | (data << lsb);
   }

   //! Register helper to get a field
   unsigned getField(volatile uint32_t& reg, unsigned msb, unsigned lsb) const
   {
      uint32_t mask = (1 << (msb - lsb + 1)) - 1;

      return (reg >> lsb) & mask;
   }

   //! Register helper to set a bit
   void setBit(volatile uint32_t& reg, unsigned bit, unsigned data = 1)
   {
       setField(reg, bit, bit, data);
   }

   //! Register helper to get a bit
   unsigned getBit(volatile uint32_t& reg, unsigned bit) const
   {
       return getField(reg, bit, bit);
   }

   //! Register helper to clear a field
   void clrField(volatile uint32_t& reg, unsigned msb, unsigned lsb)
   {
       setField(reg, msb, lsb, 0);
   }

   //! Register helper to clear a field
   void clrBit(volatile uint32_t& reg, unsigned bit)
   {
       clrField(reg, bit, bit);
   }
};

} // namespace MTL
