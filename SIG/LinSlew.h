//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG.h"

namespace SIG {

class LinSlew
{
public:
   LinSlew() = default;

   //! Set rate of target updates (Hz)
   void setUpdateRate(unsigned rate_)
   {
      recip_steps = rate_ / Float(SAMPLE_RATE);
   }

   //! Set output to constant value (no slew)
   void set(Signal value_)
   {
      value = value_;
      delta = 0.0f;
   }

   //! Set next target
   void setTarget(Signal target_)
   {
      delta = (target_ - value) * recip_steps;
   }

   //! Set next target
   Signal operator=(Signal target_)
   {
      setTarget(target_);
      return target_;
   }

   //! Sample current value
   Signal operator()()
   {
      value += delta;
      return value;
   }

private:
   Signal value{0.0f};
   Signal delta{0.0f};
   Float  recip_steps{0.0f};
};

} // namespace SIG
