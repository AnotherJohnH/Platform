//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

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
