//-------------------------------------------------------------------------------
// Copyright (c) 2018 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
         radians = fmod(value_in_units / units_per_radian, 2.0 * M_PI);
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

   //! Default constructor
   Angle() = default;

   //! Constructor allowing use of 0.0 without units
   Angle(TYPE value)
   {
      assert(value == 0.0 && value_rad == 0.0);
   }

   //! Constructor with units
   Angle(TYPE value, Unit unit)
   {
      switch(unit)
      {
      case Unit::RAD:  rad()  = value; break;
      case Unit::DEG:  deg()  = value; break;
      case Unit::TURN: turn() = value; break;
      case Unit::GON:  gon()  = value; break;
      }
   }

   void operator=(const Angle& that) { value_rad = that.value_rad; }

   //! Get writable reference to angle in radians
   Accessor rad() { return Accessor(1.0, value_rad); }

   //! Get readable reference to angle in radians
   ConstAccessor rad() const { return ConstAccessor(1.0, value_rad); }

   //! Get degrees accessor
   Accessor deg() { return Accessor(DEGREES_PER_RADIAN, value_rad); }

   //! Get const degrees accessor
   ConstAccessor deg() const { return ConstAccessor(DEGREES_PER_RADIAN, value_rad); }

   //! Get turns accessor
   Accessor turn() { return Accessor(TURNS_PER_RADIAN, value_rad); }

   //! Get const turns accessor
   ConstAccessor turn() const { return ConstAccessor(TURNS_PER_RADIAN, value_rad); }

   //! Get gradients accessor
   Accessor gon() { return Accessor(GRADIENTS_PER_RADIAN, value_rad); }

   //! Get const gradients accessor
   ConstAccessor gon() const { return ConstAccessor(GRADIENTS_PER_RADIAN, value_rad); }

   //! Get the whole degrees part of an angle
   signed degrees() const { return signed(deg()); }

   //! Get the whole minutes part of an angle
   unsigned minutes() const { return unsigned(abs((deg() - degrees()) * 60)); }

   //! Get the seconds part of an angle
   double seconds() const { return fmod(fabs(deg() * 3600), 60); }

   // Relational operators (be very careful!)
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

   void operator+=(const Angle& that) { rad() = rad() + that.rad(); }
   void operator-=(const Angle& that) { rad() = rad() - that.rad(); }
   void operator*=(TYPE value)        { rad() = rad() * value; }

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
