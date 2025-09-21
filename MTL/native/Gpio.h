//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \file Gpio.h
// \brief Stub general purpose I/O

#pragma once

#include <atomic>
#include <cstdint>

extern std::atomic<uint8_t> gpio;

namespace MTL {

namespace Gpio {

template <unsigned WIDTH, unsigned PIN>
class Out
{
public:
   operator uint32_t() const
   {
      return (gpio >> LSB) & MASK;
   }

   uint32_t operator=(uint32_t data)
   {
      // XXX hazard
      clr(MASK);
      set(data);
      return data;
   }

   void set(uint32_t data)
   {
      gpio |= (data & MASK) << LSB;
   }

   void clr(uint32_t data)
   {
      gpio &= ~((data & MASK) << LSB);
   }

private:
   static const unsigned LSB  = PIN & 0x7;
   static const uint32_t MASK = (1<<WIDTH) - 1;
};


template <unsigned WIDTH, unsigned PIN>
class In
{
public:
   operator uint32_t() const
   {
      return (gpio >> LSB) & MASK;
   }

private:
   static const unsigned LSB  = PIN & 0x7;
   static const uint32_t MASK = (1<<WIDTH) - 1;
};

} // namespace GPIO

} // namespace MTL

