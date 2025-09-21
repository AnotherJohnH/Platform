//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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

   using type = T;

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

   This operator+() const
   { This r; for(unsigned i=0; i<N; ++i) r[i] = +e[i]; return r; }

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

   bool operator==(const This& v) const
   {
      return (x == v.x) && (y == v.y);
   }

   This getNormalised() const
   {
      assert(getMagnitude() != 0.0);

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

   double getMagnitude() const
   {
      return sqrt(getSquare());
   }
};

} // namespace STB

#endif
