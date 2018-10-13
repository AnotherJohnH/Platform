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

#ifndef STB_ANGLE_H
#define STB_ANGLE_H

#include <cassert>
#include <cmath>

namespace STB {

//! An angular value which can be accessed in degrees or radians
template <typename TYPE = double>
class Angle
{
public:
   enum class Unit
   {
      RAD,
      DEG
   };

   //! Access an angle value in degrees
   class Degrees
   {
   public:
      Degrees(TYPE& ref_) : ref(ref_) {}

      TYPE operator=(TYPE value)
      {
         ref = value * M_PI / 180.0;
         return value;
      }

      operator TYPE() const { return ref * 180.0 / M_PI; }

   private:
      TYPE& ref;
   };

   //! Access a const angle value in degrees
   class ConstDegrees
   {
   public:
      ConstDegrees(const TYPE& ref_) : ref(ref_) {}

      operator TYPE() const { return ref * 180.0 / M_PI; }

   private:
      const TYPE& ref;
   };

   Angle() = default;

   Angle(TYPE value)
   {
      assert(value == 0.0 && value_rad == 0.0);
   }

   //! Constructor with units
   Angle(TYPE value, Unit unit)
   {
      switch(unit)
      {
      case Unit::RAD: this->rad() = value; break;
      case Unit::DEG: this->deg() = value; break;
      }
   }

   void operator=(const Angle& that) { value_rad = that.value_rad; }

   //! Get writable reference to angle in radians
   TYPE& rad() { return value_rad; }

   //! Get readable reference to angle in radians
   const TYPE& rad() const { return value_rad; }

   //! Get degrees accessor
   Degrees deg() { return Degrees(rad()); }

   //! Get degrees accessor
   ConstDegrees deg() const { return ConstDegrees(rad()); }

   // Relational operators
   bool operator!=(const Angle& that) const { return rad() != that.rad(); }
   bool operator==(const Angle& that) const { return rad() == that.rad(); }
   bool operator>=(const Angle& that) const { return rad() >= that.rad(); }
   bool operator<=(const Angle& that) const { return rad() <= that.rad(); }
   bool operator>( const Angle& that) const { return rad() >  that.rad(); }
   bool operator<( const Angle& that) const { return rad() <  that.rad(); }

   // Arithmetic operators
   Angle operator-(const Angle& that) const { return Angle::rad(rad() - that.rad()); }
   Angle operator+(const Angle& that) const { return Angle::rad(rad() + that.rad()); }

   TYPE operator/(const Angle& that) const { return rad() / that.rad(); }
   Angle operator*(TYPE value)       const { return Angle::rad(rad() * value); }

   void operator+=(const Angle& that) { rad() += that.rad(); }
   void operator-=(const Angle& that) { rad() -= that.rad(); }
   void operator*=(TYPE value)      { rad() *= value; }

   //! Factory for angles with initial value in degrees
   static Angle rad(TYPE value) { return Angle(value, Unit::RAD); }

   //! Factory for angles with initial value in radians
   static Angle deg(TYPE value) { return Angle(value, Unit::DEG); }

private:
   TYPE value_rad{0.0};
};

} // namespace STB

#endif // STB_ANGLE_H
