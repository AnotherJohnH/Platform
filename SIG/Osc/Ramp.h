//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

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

   Gain gain{};
};

} // namespace Osc

} // namespace SIG
