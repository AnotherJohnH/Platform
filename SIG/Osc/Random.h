//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Osc/Base.h"
#include "SIG/Gain.h"

namespace SIG {

namespace Osc {

class Random : public Base
{
public:
   Random() = default;

   Signal operator()()
   {
      if (phase < last_phase)
         nextSample();

      last_phase = phase;

      phase += delta;

      return gain(signal);
   }

   Signal operator()(Signal mod_)
   {
      if (phase < last_phase)
         nextSample();

      setDelta(modDelta(mod_));

      last_phase = phase;

      phase += delta;

      return gain(signal);
   }

   Gain gain{};

private:
   void nextSample()
   {
      noise_state ^= noise_state << 13;
      noise_state ^= noise_state >> 17;
      noise_state ^= noise_state << 5;

      signal = Signal(int32_t(noise_state)) / 0x7FFFFFFF;
   }

   uint32_t noise_state{1};
   Signal   signal{};
   UPhase   last_phase{UPHASE_MAX};
};

} // namespace Osc

} // namespace SIG
