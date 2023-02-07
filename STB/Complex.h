//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef STB_COMPLEX_H
#define STB_COMPLEX_H

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

#endif // STB_COMPLEX_H
