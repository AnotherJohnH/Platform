//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace STB {

//! 32-bit unsigned fixed point type
template <unsigned N>
class UFixP
{
public:
   using RAW = uint32_t;

   UFixP() = default;

   //! Construct from another fixed point value
   template <unsigned M>
   UFixP(UFixP<M> that)
   {
      if (M >= N)
         value = that.value >> (M - N);
      else
         value = that.value << (N - M);
   }

   //! Construct from another value
   template <typename TYPE>
   UFixP(TYPE value_) { value = UNITY * value_; }

   //! Assign from another value
   template <typename TYPE>
   UFixP<N> operator=(TYPE that)
   {
      value = UFixP<N>(that).value;
      return *this;
   }

   //! Add a value
   template <typename TYPE>
   UFixP<N> operator+=(TYPE that)
   {
      value += UFixP<N>(that).value;
      return *this;
   }

   //! Subtract a value
   template <typename TYPE>
   UFixP<N> operator-=(TYPE that)
   {
      value -= UFixP<N>(that).value;
      return *this;
   }

   //! Multiply by an integer value
   //! XXX special case for better optimisation
   UFixP<N> operator*=(uint8_t  that) { value *= that; return *this; }
   UFixP<N> operator*=(uint16_t that) { value *= that; return *this; }
   UFixP<N> operator*=(uint32_t that) { value *= that; return *this; }
   UFixP<N> operator*=(uint64_t that) { value *= that; return *this; }

   //! Multiply by a value
   template <typename TYPE>
   UFixP<N> operator*=(TYPE that)
   {
      WIDE wide = value;
      wide *= UFixP<N>(that).value;
      value = wide >> N;
      return *this;
   }

   //! Return addition with a value
   template <typename TYPE>
   UFixP<N> operator+(TYPE that) const
   {
      UFixP<N> result {*this};
      result += that;
      return result;
   }

   //! Return subtraction with a value
   template <typename TYPE>
   UFixP<N> operator-(TYPE that) const
   {
      UFixP<N> result {*this};
      result -= that;
      return result;
   }

   //! Return multiplication with a value
   template <typename TYPE>
   UFixP<N> operator*(TYPE that) const
   {
      UFixP<N> result {*this};
      result *= that;
      return result;
   }

   // Cast to unsigned integer
   operator uint8_t()  const { return value >> FRAC_BITS; }
   operator uint16_t() const { return value >> FRAC_BITS; }
   operator uint32_t() const { return value >> FRAC_BITS; }
   operator uint64_t() const { return value >> FRAC_BITS; }

   // Cast to floating point
   operator float()  const { return float(value) / UNITY; }
   operator double() const { return double(value) / UNITY; }

   // Comparison operators
   template <typename TYPE>
   bool operator<(TYPE that) const { return value < UFixP<N>(that).value; }

   template <typename TYPE>
   bool operator<=(TYPE that) const { return value <= UFixP<N>(that).value; }

   template <typename TYPE>
   bool operator>(TYPE that) const { return value > UFixP<N>(that).value; }

   template <typename TYPE>
   bool operator>=(TYPE that) const { return value >= UFixP<N>(that).value; }

   template <typename TYPE>
   bool operator==(TYPE that) const { return value == UFixP<N>(that).value; }

   template <typename TYPE>
   bool operator!=(TYPE that) const { return value != UFixP<N>(that).value; }

   //! Return raw value
   RAW raw() const { return value; }

   //! Return the fraction
   RAW frac() const
   {
      const RAW MASK = (WIDE(1) << N) - 1;
      return value & MASK;
   }

   //! Translate raw value into UFixP value
   static UFixP<N> fromRaw(RAW raw)
   {
      UFixP<N> result {};
      result.value = raw;
      return result;
   }

   //! Return smallest representable positive value
   static UFixP<N> DELTA()
   {
      UFixP<N> result {};
      result.value = 1;
      return result;
   }

   //! Return number of delta() between 0 and 1
   static unsigned SCALE()
   {
      return UNITY;
   }

   static const unsigned FRAC_BITS = N;

protected:
   using WIDE = uint64_t;

   static const unsigned BITS  = sizeof(RAW) * 8;
   static const WIDE     UNITY = WIDE(1) << FRAC_BITS;

   RAW value;
};

} // namespace STB
