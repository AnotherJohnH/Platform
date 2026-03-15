//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace STB {

template <typename TYPE>
class BitfieldRef
{
public:
   BitfieldRef() = default;

   BitfieldRef(TYPE&    container_,
               unsigned width_,
               unsigned lsb_)
   {
      init(container_, width_, lsb_);
   }

   void init(TYPE&    container_,
             unsigned width_,
             unsigned lsb_)
   {
      ptr  = &container_;
      lsb  = lsb_;
      mask = ((TYPE(1) << width_) - 1) << lsb_;
   }

   operator unsigned() const
   {
      return (*ptr & mask) >> lsb;
   }

   unsigned operator=(unsigned value_) const
   {
      *ptr = (*ptr & ~mask) | ((value_ << lsb) & mask);
      return value_;
   }

private:
   TYPE*   ptr{};
   TYPE    mask{};
   uint8_t lsb{};
};

} // namespace STB
