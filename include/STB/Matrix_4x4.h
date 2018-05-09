//------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
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

#ifndef STB_MATRIX_4X4_H
#define STB_MATRIX_4X4_H

#include <cmath>

#include "Vector3.h"

namespace STB {

//! Three dimensional vector template
template <typename T>
class Matrix_4x4
{
public:
   Matrix_4x4() = default;

   void translate(T x, T y, T z)
   {
      j += x;
      h += y;
      l += z;
   }

   void scale(T x, T y, T z)
   {
      a *= x;
      f *= y;
      k *= z;
   }

   void rotateX(double angle)
   {
   }

   void rotateY(double angle)
   {
   }

   void rotateZ(double angle)
   {
   }

   Matrix_4x4<T> operator*(const Matrix_4x4& that) const
   {
      Matrix_4x4 result;

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

   Vector3<T> transform(const Vector3<T>& in) const
   {
      return Vector3<T>(in.x * a + in.y * b + in.z * c + d,
                        in.x * e + in.y * f + in.z * g + h,
                        in.x * i + in.y * j + in.z * k + l);
   }

   T a{1.0}, b{0.0}, c{0.0}, d{0.0};
   T e{0.0}, f{1.0}, g{0.0}, h{0.0};
   T i{0.0}, j{0.0}, k{1.0}, l{0.0};
   T m{0.0}, n{0.0}, o{0.0}, p{1.0};
};

} // namespace STB

#endif
