//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Base.h"

namespace SIG::osc {

class Ramp : public Base
{
public:
   Ramp() = default;

   Signal operator()()
   {
      Signal signal = uphase2signal(phase);

      float t = uphase2float(phase - UPHASE_HALF);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      setDelta(modDelta(mod_));

      Signal signal = uphase2signal(phase);

      float t = uphase2float(phase - UPHASE_HALF);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }
};

} // namespace SIG::osc
