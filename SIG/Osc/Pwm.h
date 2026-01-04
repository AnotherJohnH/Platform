//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

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

   Gain gain{};

private:
   UPhase limit{UPHASE_HALF};
};

using Pwm = PwmShift<+1.0f,-1.0f>;

} // namespace Osc

} // namespace SIG
