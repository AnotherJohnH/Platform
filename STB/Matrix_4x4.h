//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef STB_MATRIX_4X4_H
#define STB_MATRIX_4X4_H

#include <cmath>

#include "Vector3.h"

namespace STB {

//! 4x4 matrix template
template <typename T>
class Matrix_4x4
{
public:
   using Vector = Vector3<T>;

   Matrix_4x4() = default;

   void setIdentity()
   {
      a = 1; b = 0; c = 0; d = 0;
      e = 0; f = 1; g = 0; h = 0;
      i = 0; j = 0; k = 1; l = 0;
      m = 0; n = 0; o = 0; p = 1;
   }

   void setTranslate(T x, T y, T z)
   {
      a = 1; b = 0; c = 0; d = x;
      e = 0; f = 1; g = 0; h = y;
      i = 0; j = 0; k = 1; l = z;
      m = 0; n = 0; o = 0; p = 1;
   }

   void setTranslate(const Vector& t) { setTranslate(t.x, t.y, t.z); }

   void setScale(T x, T y, T z)
   {
      a = x; b = 0; c = 0; d = 0;
      e = 0; f = y; g = 0; h = 0;
      i = 0; j = 0; k = z; l = 0;
      m = 0; n = 0; o = 0; p = 1;
   }

   void setScale(const Vector& s) { setScale(s.x, s.y, s.z); }

   void setRotateX(double angle)
   {
      a = 1; b = 0; c = 0; d = 0;
      e = 0; f = cos(angle); g = sin(angle); h = 0;
      i = 0; j = -g; k = f; l = 0;
      m = 0; n = 0; o = 0; p = 1;
   }

   void setRotateY(double angle)
   {
      a = cos(angle); b = 0; c = sin(angle); d = 0;
      e = 0; f = 1; g = 0; h = 0;
      i = -c; j = 0; k = a; l = 0;
      m = 0; n = 0; o = 0; p = 1;
   }

   void setRotateZ(double angle)
   {
      a = cos(angle); b = sin(angle); c = 0; d = 0;
      e = -b; f = a; g = 0; h = 0;
      i = 0; j = 0; k = 1; l = 0;
      m = 0; n = 0; o = 0; p = 1;
   }

   Matrix_4x4<T> operator*(const Matrix_4x4& that) const
   {
      Matrix_4x4 result;

      result.a = a*that.a + b*that.e + c*that.i + d*that.m;
      result.b = a*that.b + b*that.f + c*that.j + d*that.n;
      result.c = a*that.c + b*that.g + c*that.k + d*that.o;
      result.d = a*that.d + b*that.h + c*that.l + d*that.p;

      result.e = e*that.a + f*that.e + g*that.i + h*that.m;
      result.f = e*that.b + f*that.f + g*that.j + h*that.n;
      result.g = e*that.c + f*that.g + g*that.k + h*that.o;
      result.h = e*that.d + f*that.h + g*that.l + h*that.p;

      result.i = i*that.a + j*that.e + k*that.i + l*that.m;
      result.j = i*that.b + j*that.f + k*that.j + l*that.n;
      result.k = i*that.c + j*that.g + k*that.k + l*that.o;
      result.l = i*that.d + j*that.h + k*that.l + l*that.p;

      result.m = m*that.a + n*that.e + o*that.i + p*that.m;
      result.n = m*that.b + n*that.f + o*that.j + p*that.n;
      result.o = m*that.c + n*that.g + o*that.k + p*that.o;
      result.p = m*that.d + n*that.h + o*that.l + p*that.p;

      return result;
   }

   //! Multiply the matrix by a translation matrix
   void translate(const Vector& t)
   {
      Matrix_4x4 m;
      m.setTranslate(t);
      *this = *this * m;
   }

   //! Multiply the matrix by a scale matrix
   void scale(const Vector& t)
   {
      Matrix_4x4 m;
      m.setScale(t);
      *this = *this * m;
   }

   //! Multiply the matrix by a rotation about X
   void rotateX(double angle)
   {
      Matrix_4x4 m;
      m.setRotateX(angle);
      *this = *this * m;
   }

   //! Multiply the matrix by a rotation about Y
   void rotateY(double angle)
   {
      Matrix_4x4 m;
      m.setRotateY(angle);
      *this = *this * m;
   }

   //! Multiply the matrix by a rotation about Z
   void rotateZ(double angle)
   {
      Matrix_4x4 m;
      m.setRotateZ(angle);
      *this = *this * m;
   }

   Vector3<T> transformPos(const Vector& in) const
   {
      return Vector3<T>(in.x * a + in.y * b + in.z * c + d,
                        in.x * e + in.y * f + in.z * g + h,
                        in.x * i + in.y * j + in.z * k + l);
   }

   Vector3<T> transformDir(const Vector& in) const
   {
      return Vector3<T>(in.x * a + in.y * b + in.z * c,
                        in.x * e + in.y * f + in.z * g,
                        in.x * i + in.y * j + in.z * k);
   }

   T a{1.0}, b{0.0}, c{0.0}, d{0.0};
   T e{0.0}, f{1.0}, g{0.0}, h{0.0};
   T i{0.0}, j{0.0}, k{1.0}, l{0.0};
   T m{0.0}, n{0.0}, o{0.0}, p{1.0};
};

} // namespace STB

#endif
