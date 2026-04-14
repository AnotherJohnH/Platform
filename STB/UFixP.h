//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <iomanip>

namespace STB {

//! 32-bit unsigned fixed point type
template <unsigned N>
class UFixP
{
   static_assert(N < 32);

public:
   using RAW = uint32_t;

   constexpr UFixP() = default;

   //! Construct from another fixed point value
   template <unsigned M>
   constexpr UFixP(UFixP<M> that)
   {
      if (M >= N)
         value = that.raw() >> (M - N);
      else
         value = that.raw() << (N - M);
   }

   //! Construct from another value
   template <typename TYPE>
   requires std::is_arithmetic_v<TYPE>
   constexpr UFixP(TYPE value_) { value = UNITY * value_; }

   //! Assign from another value
   template <typename TYPE>
   constexpr UFixP<N> operator=(TYPE that)
   {
      value = UFixP<N>(that).value;
      return *this;
   }

   //! Add a value
   template <typename TYPE>
   constexpr UFixP<N> operator+=(TYPE that)
   {
      value += UFixP<N>(that).value;
      return *this;
   }

   //! Subtract a value
   template <typename TYPE>
   constexpr UFixP<N> operator-=(TYPE that)
   {
      value -= UFixP<N>(that).value;
      return *this;
   }

   //! Multiply by an integer value
   //! XXX special case for better optimisation
   constexpr UFixP<N> operator*=(uint8_t  that) { value *= that; return *this; }
   constexpr UFixP<N> operator*=(uint16_t that) { value *= that; return *this; }
   constexpr UFixP<N> operator*=(uint32_t that) { value *= that; return *this; }
   constexpr UFixP<N> operator*=(uint64_t that) { value *= that; return *this; }

   //! Multiply by a value
   template <typename TYPE>
   constexpr UFixP<N> operator*=(TYPE that)
   {
      WIDE wide = value;
      wide *= UFixP<N>(that).value;
      value = wide >> N;
      return *this;
   }

   //! Divide by an integer value
   //! XXX special case for better optimisation
   constexpr UFixP<N> operator/=(uint8_t  that) { value /= that; return *this; }
   constexpr UFixP<N> operator/=(uint16_t that) { value /= that; return *this; }
   constexpr UFixP<N> operator/=(uint32_t that) { value /= that; return *this; }
   constexpr UFixP<N> operator/=(uint64_t that) { value /= that; return *this; }

   //! Divide by a value
   template <typename TYPE>
   constexpr UFixP<N> operator/=(TYPE that)
   {
      WIDE wide = WIDE(value) << N;
      wide /= UFixP<N>(that).value;
      value = wide;
      return *this;
   }

   //! Return addition with a value
   template <typename TYPE>
   constexpr UFixP<N> operator+(TYPE that) const
   {
      UFixP<N> result {*this};
      result += that;
      return result;
   }

   //! Return subtraction with a value
   template <typename TYPE>
   constexpr UFixP<N> operator-(TYPE that) const
   {
      UFixP<N> result {*this};
      result -= that;
      return result;
   }

   //! Return multiplication with a value
   template <typename TYPE>
   constexpr UFixP<N> operator*(TYPE that) const
   {
      UFixP<N> result {*this};
      result *= that;
      return result;
   }

   //! Return division with a value
   template <typename TYPE>
   constexpr UFixP<N> operator/(TYPE that) const
   {
      UFixP<N> result {*this};
      result /= that;
      return result;
   }

   // Cast to unsigned integer
   constexpr operator uint8_t()  const { return value >> FRAC_BITS; }
   constexpr operator uint16_t() const { return value >> FRAC_BITS; }
   constexpr operator uint32_t() const { return value >> FRAC_BITS; }
   constexpr operator uint64_t() const { return value >> FRAC_BITS; }

   // Cast to floating point
   constexpr operator float()  const { return float(value) / UNITY; }
   constexpr operator double() const { return double(value) / UNITY; }

   // Comparison operators
   template <typename TYPE>
   constexpr bool operator<(TYPE that) const { return value < UFixP<N>(that).value; }

   template <typename TYPE>
   constexpr bool operator<=(TYPE that) const { return value <= UFixP<N>(that).value; }

   template <typename TYPE>
   constexpr bool operator>(TYPE that) const { return value > UFixP<N>(that).value; }

   template <typename TYPE>
   constexpr bool operator>=(TYPE that) const { return value >= UFixP<N>(that).value; }

   template <typename TYPE>
   constexpr bool operator==(TYPE that) const { return value == UFixP<N>(that).value; }

   template <typename TYPE>
   constexpr bool operator!=(TYPE that) const { return value != UFixP<N>(that).value; }

   // iostream support
   friend std::ostream& operator<<(std::ostream& os, const UFixP<N>& that)
   {
      constexpr unsigned SHIFT = 3 - ((FRAC_BITS - 1) % 4);

      RAW units = that.value >> FRAC_BITS;

      os << std::hex << std::setfill('0')
         << std::setw((INT_BITS + 3) / 4) << units
         << '.';

      if (FRAC_BITS > 0)
         os << std::setw(FRAC_BITS / 4) << (that.frac() << SHIFT);

      return os;
   }


   //! Return raw value
   constexpr RAW raw() const { return value; }

   //! Return the fraction
   constexpr RAW frac() const
   {
      const RAW MASK = (WIDE(1) << N) - 1;
      return value & MASK;
   }

   //! Translate raw value into UFixP value
   constexpr static UFixP<N> fromRaw(RAW raw)
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

   static constexpr unsigned FRAC_BITS = N;
   static constexpr unsigned INT_BITS  = sizeof(RAW) * 8 - N;

protected:
   using WIDE = uint64_t;

   static constexpr unsigned BITS  = sizeof(RAW) * 8;
   static constexpr WIDE     UNITY = WIDE(1) << FRAC_BITS;

   RAW value;
};

} // namespace STB
