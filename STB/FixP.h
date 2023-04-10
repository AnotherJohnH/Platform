//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

#pragma once

#include <cstdint>

namespace STB {

//! 32-bit fixed point type
template <unsigned N>
class FixP
{
public:
   using RAW = int32_t;

   FixP() = default;

   //! Construct from another fixed point value
   template <unsigned M>
   FixP(FixP<M> that)
   {
      if (M >= N)
         value = that.value >> (M - N);
      else
         value = that.value << (N - M);
   }

   //! Construct from another value
   template <typename TYPE>
   FixP(TYPE value_) { value = UNITY * value_; }

   //! Assign from another value
   template <typename TYPE>
   FixP<N> operator=(TYPE that)
   {
      value = FixP<N>(that).value;
      return *this;
   }

   //! Add a value
   template <typename TYPE>
   FixP<N> operator+=(TYPE that)
   {
      value += FixP<N>(that).value;
      return *this;
   }

   //! Subtract a value
   template <typename TYPE>
   FixP<N> operator-=(TYPE that)
   {
      value -= FixP<N>(that).value;
      return *this;
   }

   //! Multiply by an integer value
   //! XXX special case for better optimisation
   FixP<N> operator*=(int8_t  that) { value *= that; return *this; }
   FixP<N> operator*=(int16_t that) { value *= that; return *this; }
   FixP<N> operator*=(int32_t that) { value *= that; return *this; }
   FixP<N> operator*=(int64_t that) { value *= that; return *this; }

   //! Multiply by a value
   template <typename TYPE>
   FixP<N> operator*=(TYPE that)
   {
      WIDE wide = value;
      wide *= FixP<N>(that).value;
      value = wide >> N;
      return *this;
   }

   //! Return addition with a value
   template <typename TYPE>
   FixP<N> operator+(TYPE that) const
   {
      FixP<N> result {*this};
      result += that;
      return result;
   }

   //! Return subtraction with a value
   template <typename TYPE>
   FixP<N> operator-(TYPE that) const
   {
      FixP<N> result {*this};
      result -= that;
      return result;
   }

   //! Return multiplication with a value
   template <typename TYPE>
   FixP<N> operator*(TYPE that) const
   {
      FixP<N> result {*this};
      result *= that;
      return result;
   }

   // Cast to signed integer
   operator int8_t()  const { return value >> FRAC_BITS; }
   operator int16_t() const { return value >> FRAC_BITS; }
   operator int32_t() const { return value >> FRAC_BITS; }
   operator int64_t() const { return value >> FRAC_BITS; }

   // Cast to floating point
   operator float()  const { return float(value) / UNITY; }
   operator double() const { return double(value) / UNITY; }

   // Comparison operators
   template <typename TYPE>
   bool operator<(TYPE that) const { return value < FixP<N>(that).value; }

   template <typename TYPE>
   bool operator<=(TYPE that) const { return value <= FixP<N>(that).value; }

   template <typename TYPE>
   bool operator>(TYPE that) const { return value > FixP<N>(that).value; }

   template <typename TYPE>
   bool operator>=(TYPE that) const { return value >= FixP<N>(that).value; }

   template <typename TYPE>
   bool operator==(TYPE that) const { return value == FixP<N>(that).value; }

   template <typename TYPE>
   bool operator!=(TYPE that) const { return value != FixP<N>(that).value; }

   //! Return raw value
   RAW raw() const { return value; }

   //! Return the fraction
   RAW frac() const
   {
      const RAW MASK = (WIDE(1) << N) - 1;
      return value & MASK;
   }

   //! Translate raw value into FixP value
   static FixP<N> fromRaw(RAW raw)
   {
      FixP<N> result {};
      result.value = raw;
      return result;
   }

   //! Return smallest representable positive value
   static FixP<N> DELTA()
   {
      FixP<N> result {};
      result.value = 1;
      return result;
   }

   //! Return number of delta() between 0 and 1
   static unsigned SCALE()
   {
      return UNITY;
   }

protected:
   using WIDE = int64_t;

   static const unsigned FRAC_BITS = N;
   static const unsigned BITS      = sizeof(RAW) * 8;
   static const WIDE     UNITY     = WIDE(1) << FRAC_BITS;

   RAW value;
};

} // namespace STB
