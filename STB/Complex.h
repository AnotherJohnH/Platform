//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>
#include <cmath>

namespace STB {

template <class TYPE>
class Complex
{
private:
   typedef Complex<TYPE> This;

   TYPE  r, i;

public:
   Complex(TYPE real, TYPE imag=0) : r(real), i(imag) {}
   Complex() {}

   inline TYPE real() const { return r; }
   inline TYPE imag() const { return i; }

   inline This squared() const
   {
      return This(r*r-i*i, 2*r*i);
   }

   inline TYPE magnitude_squared() const
   {
      return r*r + i*i;
   }

   inline This conjugate() const
   {
      return This(r, -i);
   }

   inline This reciprical() const
   {
      return conjugate() * (1.0 / magnitude_squared());
   }

   inline This operator+(This& rhs) const
   {
      return This(r + rhs.r, i + rhs.i);
   }

   inline This operator-(This& rhs) const
   {
      return This(r - rhs.r, i - rhs.i);
   }

   inline This operator*(const This& rhs) const
   {
      return This(r*rhs.r - i*rhs.i, r*rhs.i + i*rhs.r);
   }

   inline This operator*(TYPE rhs) const
   {
      return This(r*rhs, i*rhs);
   }

   inline void operator+=(const This& rhs)
   {
      r += rhs.r;
      i += rhs.i;
   }

   inline void operator-=(const This& rhs)
   {
      r -= rhs.r;
      i -= rhs.i;
   }

   void print(FILE* fp)
   {
      fprintf(fp, "%8f + %8fi", real(), imag());
   }
};

typedef Complex<float>   Complex32;
typedef Complex<double>  Complex64;

inline void print(FILE* fp, const STB::Complex64& value)
{
   fprintf(fp, "%8g + %8gj", value.real(), value.imag());
}

} // namespace STB

