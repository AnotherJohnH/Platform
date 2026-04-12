//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Const.h"
#include "Types.h"

#include <cmath>

namespace SIG {

class ExpSlew
{
public:
   ExpSlew(Float tc_ = Float{1.0f})
   {
      setTimeConst(tc_);
   }

   //! Set time constant (s)
   void setTimeConst(Float time_)
   {
#if defined(SIG_FP32)
      alpha = 1.0f - expf(-1.0f / (time_ * SAMPLE_RATE));
#elif defined(SIG_FP64)
      alpha = 1.0 - exp(-1.0 / (time_ * SAMPLE_RATE));
#endif
   }

   //! Set output to constant value (no slew)
   void set(Signal value_)
   {
      value = target = value_;
   }

   //! Set next target
   void setTarget(Signal target_)
   {
      target = target_;
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
      value += alpha * (target - value);
      return value;
   }

   //! Multiply by current value
   Signal operator()(Signal in_)
   {
      return in_ * operator()();
   }

private:
   Signal value{};
   Signal target{};
   Float  alpha{};
};

} // namespace SIG
