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

#ifndef STB_VECTOR2_H
#define STB_VECTOR2_H

#include <cassert>
#include <cstddef>
#include <cmath>

namespace STB {

//! Two dimensional vector template
template <typename T>
class Vector2
{
private:
   static const unsigned N = 2;

protected:
   using Function = T (*)(T);
   using This     = Vector2<T>;

public:
   union
   {
      T e[N];
      struct
      {
         T x, y;
      };
   };

   static size_t size() { return N; }

   // clang-format off
   Vector2()              {}
   Vector2(const T& v)    { operator=(v); }
   Vector2(const This& v) { operator=(v); }
   Vector2(Function f)    { operator=(f); }
   Vector2(const T& x_, const T& y_) : e{ x_, y_} {}

         T& operator[](unsigned i)       { assert(i<N); return e[i]; }
   const T& operator[](unsigned i) const { assert(i<N); return e[i]; }

   void operator= (const T& v)    { for(unsigned i=0; i<N; ++i) e[i] =  v; }
   void operator+=(const T& v)    { for(unsigned i=0; i<N; ++i) e[i] += v; }
   void operator-=(const T& v)    { for(unsigned i=0; i<N; ++i) e[i] -= v; }
   void operator*=(const T& v)    { for(unsigned i=0; i<N; ++i) e[i] *= v; }

   void operator= (const This& v) { for(unsigned i=0; i<N; ++i) e[i] =  v[i]; }
   void operator+=(const This& v) { for(unsigned i=0; i<N; ++i) e[i] += v[i]; }
   void operator-=(const This& v) { for(unsigned i=0; i<N; ++i) e[i] -= v[i]; }
   void operator*=(const This& v) { for(unsigned i=0; i<N; ++i) e[i] *= v[i]; }
   void operator^=(const This& v) { *this = getCrossProduct(v); }

   This operator-() const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = -e[i]; return r; }

   This operator+(const T& v) const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = e[i] + v; return r; }

   This operator-(const T& v) const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = e[i] - v; return r; }

   This operator*(const T& v) const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = e[i] * v; return r; }

   This operator/(const T& v) const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = e[i] / v; return r; }

   This operator+(const This& v) const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = e[i] + v[i]; return r; }

   This operator-(const This& v) const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = e[i] - v[i]; return r; }
   // clang-format on

   This operator*(const This& v) const
   {
      This r;
      for(unsigned i = 0; i < N; ++i)
         r[i] = e[i] * v[i];
      return r;
   }

   T operator&(const This& v) const { return getDotProduct(v); }

   This operator^(const This& v) const { return getCrossProduct(v); }

   void operator=(Function f)
   {
      for(unsigned i = 0; i < N; ++i)
      {
         e[i] = (*f)((T)i / N);
      }
   }

   This getNormalised() const
   {
      assert(getMagnitude() != 0);

      return operator*(1.0 / getMagnitude());
   }

   void normalise()
   {
      *this = getNormalised();
   }

   T getDotProduct(const This& v) const
   {
      T sum = 0;

      for(unsigned i = 0; i < N; ++i)
      {
         sum += e[i] * v[i];
      }

      return sum;
   }

   T getSquare() const
   {
      return getDotProduct(*this);
   }

   T getMagnitude() const
   {
      return sqrt(getSquare());
   }
};

} // namespace STB

#endif
