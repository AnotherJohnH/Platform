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

#ifndef STB_MATRIX_3X3_H
#define STB_MATRIX_3X3_H

#include <cmath>

#include "Vector2.h"

namespace STB {

//! Three dimensional vector template
template <typename T>
class Matrix_3x3
{
public:
   Matrix_3x3() = default;

   void translate(T x, T y)
   {
      c += x;
      f += y;
   }

   void scale(T x, T y)
   {
      a *= x;
      e *= y;
   }

   void rotate(double angle)
   {
      a = cos(angle);
      b = sin(angle);
      d = -b;
      e = a;
   }

   Matrix_3x3<T> operator*(const Matrix_3x3& that) const
   {
      Matrix_3x3 result;

      result.a = a*that.a + b*that.d + c*that.g;
      result.b = a*that.b + b*that.e + c*that.h;
      result.c = a*that.c + b*that.f + c*that.i;

      result.d = d*that.a + e*that.d + f*that.g;
      result.e = d*that.b + e*that.e + f*that.h;
      result.f = d*that.c + e*that.f + f*that.i;

      result.g = g*that.a + h*that.d + i*that.g;
      result.h = g*that.b + h*that.e + i*that.h;
      result.i = g*that.c + h*that.f + i*that.i;

      return result;
   }

   Vector2<T> transform(const Vector2<T>& in) const
   {
      return Vector2<T>(in.x * a + in.y * b + c,
                        in.x * d + in.y * e + f);
   }

   T a{1.0}, b{0.0}, c{0.0};
   T d{0.0}, e{1.0}, f{0.0};
   T g{0.0}, h{0.0}, i{1.0};
};

} // namespace STB

#endif
