//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SND/SND.h"

namespace SND {


//! Schroeder All Pass Filter
class APFilter : public UnaryComponent
{
public:
   ControlPort<Signal>   coef; // Coefficient
   ControlPort<unsigned> m;    // Delay (samples)

   APFilter(Signal coef_ = 1.0, unsigned m_ = 1)
   {
      junc1.in = sum1(in, gain_fb(junc2));
      port()   = sum2(junc2(delay(junc1)), gain_ff(junc1));

      // Control paths
      coef >> neg;
      neg >> gain_ff.control;
      coef >> gain_fb.control;
      m >> delay.control;

      // Initialise
      coef = coef_;
      m    = m_;
   }

private:
   Add        sum1;
   SignalJunc junc1;
   Delay      delay;
   Gain       gain_ff;
   SignalJunc junc2;
   Add        sum2;
   Gain       gain_fb;

   ControlMul<Signal> neg{-1.0};
};


} // namespace SND

