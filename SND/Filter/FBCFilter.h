//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_FBCFILTER_H
#define SND_FBCFILTER_H

#include "SND/SND.h"

namespace SND {


//! Feedback Comb Filter
class FBCFilter : public UnaryComponent
{
public:
   ControlPort<Signal>   a; // Feedback coefficient
   ControlPort<unsigned> m; // Delay (samples)

   FBCFilter(Signal a_ = 1.0, unsigned m_ = 1)
   {
      // Forward signal path
      port() = junc(add(in, fb_gain));

      // Backward signal path
      fb_gain.in = delay(junc);

      // Control paths
      a >> fb_gain.control;
      m >> delay.control;

      // Initialise
      a = a_;
      m = m_;
   }

private:
   Add        add;
   SignalJunc junc;
   Gain       fb_gain;
   Delay      delay;
};


} // namespace SND

#endif
