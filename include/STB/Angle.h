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
   static constexpr double DEGREES_PER_RADIAN   = 180.0 / M_PI;
   static constexpr double TURNS_PER_RADIAN     = 0.5   / M_PI;
   static constexpr double GRADIENTS_PER_RADIAN = 200.0 / M_PI;

   enum class Unit
   {
      RAD,
      DEG,
      TURN,
      GON
   };

   //! Access to an angle value in units other than radians
   class Accessor
   {
   public:
      Accessor(double units_per_radian_, TYPE& radians_)
         : units_per_radian(units_per_radian_)
         , radians(radians_)
      {}

      TYPE operator=(TYPE value_in_units)
      {
         radians = value_in_units / units_per_radian;
         return value_in_units;
      }

      operator TYPE() const { return radians * units_per_radian; }

   private:
      const double units_per_radian;
      TYPE&        radians;
   };

   //! Access to a constant angle value in units other than radians
   class ConstAccessor
   {
   public:
      ConstAccessor(double units_per_radian_, const TYPE& radians_)
         : units_per_radian(units_per_radian_)
         , radians(radians_)
      {}

      operator TYPE() const { return radians * units_per_radian; }

   private:
      const double units_per_radian;
      const TYPE&  radians;
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
      case Unit::RAD:  this->rad()  = value; break;
      case Unit::DEG:  this->deg()  = value; break;
      case Unit::TURN: this->turn() = value; break;
      case Unit::GON:  this->gon()  = value; break;
      }
   }

   void operator=(const Angle& that) { value_rad = that.value_rad; }

   //! Get writable reference to angle in radians
   TYPE& rad() { return value_rad; }

   //! Get readable reference to angle in radians
   const TYPE& rad() const { return value_rad; }

   //! Get degrees accessor
   Accessor deg() { return Accessor(DEGREES_PER_RADIAN, rad()); }

   //! Get const degrees accessor
   ConstAccessor deg() const { return ConstAccessor(DEGREES_PER_RADIAN, rad()); }

   //! Get turns accessor
   Accessor turn() { return Accessor(TURNS_PER_RADIAN, rad()); }

   //! Get const turns accessor
   ConstAccessor turn() const { return ConstAccessor(TURNS_PER_RADIAN, rad()); }

   //! Get gradients accessor
   Accessor gon() { return Accessor(GRADIENTS_PER_RADIAN, rad()); }

   //! Get const gradients accessor
   ConstAccessor gon() const { return ConstAccessor(GRADIENTS_PER_RADIAN, rad()); }

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

   //! Factory for angles with initial value in turns
   static Angle turn(TYPE value) { return Angle(value, Unit::TURN); }

   //! Factory for angles with initial value in gradients
   static Angle gon(TYPE value) { return Angle(value, Unit::GON); }

private:
   TYPE value_rad{0.0};
};

} // namespace STB

#endif // STB_ANGLE_H
