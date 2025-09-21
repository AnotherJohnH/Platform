//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Low level digital I/O

#pragma once

#include "MTL/Pins.h"
#include "MTL/Gpio.h"

//! Bare metal layer
namespace MTL {

//! Low level digital I/O
namespace Digital {

//! Single digital output
template <unsigned PIN>
class Out
{
public:
   Out(bool init = false)
   {
      operator=(init);
   }

   //! Set state of digital output
   //
   //! \param true => set output high, false => set output low
   bool operator=(bool state)
   {
      if (state)
         gpio.set(1);
      else
         gpio.clr(1);

      return state;
   }

   //! Get current state of digital output
   operator bool() const
   {
      return gpio != 0;
   }

private:
   Gpio::Out<1,PIN>  gpio;
};


//! Single digital input
template <unsigned PIN>
class In
{
public:
   //! Get current state of digital input
   operator bool() const
   {
      return gpio != 0;
   }

private:
   Gpio::In<1,PIN>  gpio;
};

} // namespace Digital

} // namespace MTL
