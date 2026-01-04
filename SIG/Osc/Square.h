//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

class Square : public Base
{
public:
   Square() = default;

   Signal operator()()
   {
      Signal signal = phase < UPHASE_HALF ? +1.0f : -1.0f;

      float t = uphase2float(phase);
      signal += polyBLEP(t);
      t = uphase2float(phase - UPHASE_HALF);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      setDelta(modDelta(mod_));

      Signal signal = phase < UPHASE_HALF ? +1.0f : -1.0f;

      float t = uphase2float(phase);
      signal += polyBLEP(t);
      t = uphase2float(phase - UPHASE_HALF);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

   Gain gain{};
};

} // namespace Osc

} // namespace SIG
