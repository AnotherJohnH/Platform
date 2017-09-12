//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

#ifndef STB_ENDIAN_H
#define STB_ENDIAN_H

#include <cstdint>
#include <cassert>

namespace STB {


//! Endian swap a value
//  T is assumed to be an integer type
template <typename T>
inline T endianSwap(T data)
{
   switch(sizeof(T))
   {
   case 1: return data;
   case 2: return (data >> 8) | (data << 8);
   case 4: return (T(endianSwap(uint16_t(data)))<<16) | endianSwap(uint16_t(data>>16));
   case 8: return (T(endianSwap(uint32_t(data)))<<32) | endianSwap(uint32_t(data>>32));

   default: break;
   }

   assert(!"unexpected type");
   return 0;
}


//! Base class helper for endian sensative storage of integers
template <typename WHOLE, typename HALF, bool BIG>
class EndianHelper
{
protected:
   static const unsigned SHIFT = sizeof(HALF)*8;
   static const unsigned LS    = BIG ? 0 : 1;
   static const unsigned MS    = BIG ? 1 : 0;

   EndianHelper() {}

   WHOLE read() const
   {
      return (WHOLE(half[LS])<<SHIFT) | half[MS];
   }

   WHOLE write(const WHOLE value)
   {
      half[LS] = value >> SHIFT;
      half[MS] = value;
      return value;
   }

private:
   HALF  half[2];
};


//! A 16-bit unsigned integer that is stored in big-endian format
class Big16 : public EndianHelper<uint16_t,uint8_t,/* BIG */true>
{
public:
   Big16(const uint16_t value = 0)
   {
      write(value);
   }

   operator uint16_t() const { return read(); }

   const uint16_t operator=(const uint16_t value) { return write(value); }
};


//! A 32-bit unsigned integer that is stored in big-endian format
class Big32 : public EndianHelper<uint32_t,Big16,/* BIG */true>
{
public:
   Big32(const uint32_t value = 0)
   {
      write(value);
   }

   operator uint32_t() const { return read(); }

   const uint32_t operator=(const uint32_t value) { return write(value); }
};


//! A 64-bit unsigned integer that is stored in big-endian format
class Big64 : public EndianHelper<uint64_t,Big32,/* BIG */true>
{
public:
   Big64(const uint64_t value = 0)
   {
      write(value);
   }

   operator uint64_t() const { return read(); }

   const uint64_t operator=(const uint64_t value) { return write(value); }
};


//! A 16-bit unsigned integer that is stored in little-endian format
class Ltl16 : public EndianHelper<uint16_t,uint8_t,/* BIG */false>
{
public:
   Ltl16(const uint16_t value = 0)
   {
      write(value);
   }

   operator uint16_t() const { return read(); }

   const uint16_t operator=(const uint16_t value) { return write(value); }
};


//! A 32-bit unsigned integer that is stored in little-endian format
class Ltl32 : public EndianHelper<uint32_t,Ltl16,/* BIG */false>
{
public:
   Ltl32(const uint32_t value = 0)
   {
      write(value);
   }

   operator uint32_t() const { return read(); }

   const uint32_t operator=(const uint32_t value) { return write(value); }
};


//! A 64-bit unsigned integer that is stored in little-endian format
class Ltl64 : public EndianHelper<uint64_t,Ltl32,/* BIG */false>
{
public:
   Ltl64(const uint64_t value = 0)
   {
      write(value);
   }

   operator uint64_t() const { return read(); }

   const uint64_t operator=(const uint64_t value) { return write(value); }
};


} // namespace STB

#endif
