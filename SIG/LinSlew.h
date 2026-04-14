//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Const.h"
#include "Types.h"

namespace SIG {

class LinSlew
{
public:
   LinSlew(unsigned rate_ = SAMPLE_RATE)
   {
      setUpdateRate(rate_);
   }

   //! Set rate of target updates (Hz)
   void setUpdateRate(unsigned rate_)
   {
      recip_steps = Float(rate_) / SAMPLE_RATE;
   }

   //! Set output to constant value (no slew)
   void set(Signal value_)
   {
      value = value_;
      delta = Signal{0.0};
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

   //! Multiply by current value
   Signal operator()(Signal in_)
   {
      return in_ * operator()();
   }

private:
   Signal value{0.0};
   Signal delta{0.0};
   Float  recip_steps{0.0};
};

} // namespace SIG
