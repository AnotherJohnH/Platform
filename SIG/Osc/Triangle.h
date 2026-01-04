//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

class Triangle : public Base
{
public:
   Triangle() = default;

   Signal operator()()
   {
      UPhase phase_shift = phase + UPHASE_QUARTER;

      UPhase p = phase_shift >= UPHASE_HALF ? -phase * 2
                                          : +phase * 2;

      Signal signal = uphase2signal(p);

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      UPhase phase_shift = phase + UPHASE_QUARTER;

      UPhase p = phase_shift >= UPHASE_HALF ? -phase * 2
                                          : +phase * 2;

      Signal signal = uphase2signal(p);

      phase += modDelta(mod_);

      return gain(signal);
   }

   Gain gain{};
};

} // namespace Osc

} // namespace SIG
