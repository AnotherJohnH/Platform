//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "Types.h"
#include "Table_atten15.h"

namespace SIG {

class Gain
{
public:
   Gain(Signal value_ = 1.0) : value(value_) {}

   //! Set attenuation in dB
   void setAtten_dB(float atten_)
   {
      signed index = 0x7FFF * atten_ / 60.0;

      if (index < 0)
         value = 1.0;
      else if (index > 0x7FFF)
         value = 0.0;
      else
         value = table_atten15[index];
   }

   //! Set gain using a sine panning function [0..1] => [0..1]
   void setPan(Signal pan_)
   {
      value = sinf(pan_ * float(M_PI) / 2.0f);
   }

   operator Signal() const { return value; }

   Signal operator=(Signal value_) { return value = value_; }

   Signal operator()(Signal in_) { return in_ * value; }

private:
   Signal value{1.0};
};

} // namespace SIG
