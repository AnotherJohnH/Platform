//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace STB {

//! Endian swap a 16-bit value
constexpr uint16_t endianSwap(uint16_t data)
{
   return (data >> 8) | (data << 8);
}

//! Endian swap a 32-bit value
constexpr  uint32_t endianSwap(uint32_t data)
{
   return (uint32_t(endianSwap(uint16_t(data))) << 16) |
          endianSwap(uint16_t(uint32_t(data) >> 16));
}

//! Endian swap a 64-bit value
constexpr uint64_t endianSwap(uint64_t data)
{
   return (uint64_t(endianSwap(uint32_t(data))) << 32) |
          endianSwap(uint32_t(uint64_t(data) >> 32));
}

// XXX following assume running on little endian arch

//! Read big-endian 16-bit value using only byte acceses
inline uint16_t safeReadBig16(const void* ptr_)
{
   auto byte = (volatile const uint8_t*)ptr_;
   return (byte[0] << 8) + byte[1];
}

//! Read big-endian 32-bit value using only byte acceses
inline uint32_t safeReadBig32(const void* ptr_)
{
   auto byte = (volatile const uint8_t*)ptr_;
   return (byte[0] << 24) + (byte[1] << 16) + (byte[2] << 8) + byte[3];
}


//! Base class helper for endian sensative storage of integers
template <typename WHOLE, typename HALF, bool BIG>
class EndianHelper
{
protected:
   static constexpr unsigned SHIFT = sizeof(HALF) * 8;
   static constexpr unsigned LS    = BIG ? 0 : 1;
   static constexpr unsigned MS    = BIG ? 1 : 0;

   EndianHelper() = default;

   WHOLE read() const
   {
      return (WHOLE(half[LS])<<SHIFT) | half[MS];
   }

   WHOLE write(const WHOLE value)
   {
      half[LS] = HALF(value >> SHIFT);
      half[MS] = HALF(value);
      return value;
   }

private:
   HALF half[2];
};


//! A 16-bit unsigned integer that is stored in big-endian format
class Big16 : public EndianHelper<uint16_t, uint8_t, /* BIG */ true>
{
public:
   Big16(const uint16_t value = 0) { write(value); }

   operator uint16_t() const { return read(); }

   const uint16_t operator=(const uint16_t value) { return write(value); }
};


//! A 32-bit unsigned integer that is stored in big-endian format
class Big32 : public EndianHelper<uint32_t, Big16, /* BIG */ true>
{
public:
   Big32(const uint32_t value = 0) { write(value); }

   operator uint32_t() const { return read(); }

   const uint32_t operator=(const uint32_t value) { return write(value); }
};


//! A 64-bit unsigned integer that is stored in big-endian format
class Big64 : public EndianHelper<uint64_t, Big32, /* BIG */ true>
{
public:
   Big64(const uint64_t value = 0) { write(value); }

   operator uint64_t() const { return read(); }

   const uint64_t operator=(const uint64_t value) { return write(value); }
};


//! A 16-bit unsigned integer that is stored in little-endian format
class Ltl16 : public EndianHelper<uint16_t, uint8_t, /* BIG */ false>
{
public:
   Ltl16(const uint16_t value = 0) { write(value); }

   operator uint16_t() const { return read(); }

   const uint16_t operator=(const uint16_t value) { return write(value); }
};


//! A 32-bit unsigned integer that is stored in little-endian format
class Ltl32 : public EndianHelper<uint32_t, Ltl16, /* BIG */ false>
{
public:
   Ltl32(const uint32_t value = 0) { write(value); }

   operator uint32_t() const { return read(); }

   const uint32_t operator=(const uint32_t value) { return write(value); }
};


//! A 64-bit unsigned integer that is stored in little-endian format
class Ltl64 : public EndianHelper<uint64_t, Ltl32, /* BIG */ false>
{
public:
   Ltl64(const uint64_t value = 0) { write(value); }

   operator uint64_t() const { return read(); }

   const uint64_t operator=(const uint64_t value) { return write(value); }
};

} // namespace STB

