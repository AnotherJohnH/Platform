//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Base.h"

namespace SIG::osc {

template <Signal HIGH, Signal LOW>
class PwmShift : public Base
{
public:
   PwmShift() = default;

   //! Set pulse width 0.0 => square
   void setWidth(Signal width_)
   {
      limit = UPHASE_HALF + signal2uphase(width_);
   }

   Signal operator()()
   {
      Signal signal = phase < limit ? HIGH : LOW;

      float t = uphase2float(phase);
      signal += polyBLEP(t);
      t = uphase2float(phase - limit);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      setDelta(modDelta(mod_));

      Signal signal = phase < limit ? HIGH : LOW;

      float t = uphase2float(phase);
      signal += polyBLEP(t);
      t = uphase2float(phase - limit);
      signal -= polyBLEP(t);

      phase += delta;

      return gain(signal);
   }

private:
   UPhase limit{UPHASE_HALF};
};

using Pwm = PwmShift<Signal{+1.0},Signal{-1.0}>;

} // namespace SIG::osc
