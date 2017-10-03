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

#ifndef SND_APFILTER_H
#define SND_APFILTER_H

#include "SND/SND.h"

namespace SND {


//! Schroeder All Pass Filter
class APFilter : public UnaryComponent
{
public:
   ControlPort<Signal>    coef;  // Coefficient
   ControlPort<unsigned>  m;     // Delay (samples)

   APFilter(Signal coef_ = 1.0, unsigned m_ = 1)
   {
      junc1.in = sum1( in,
                       gain_fb( junc2 ) );
      port() = sum2( junc2( delay( junc1 ) ),
                     gain_ff( junc1 ) );

      // Control paths
      coef >> neg;
      neg  >> gain_ff.control;
      coef >> gain_fb.control;
      m    >> delay.control;

      // Initialise
      coef = coef_;
      m = m_;
   }

private:
   Add          sum1;
   SignalJunc   junc1;
   Delay        delay;
   Gain         gain_ff;
   SignalJunc   junc2;
   Add          sum2;
   Gain         gain_fb;

   ControlMul<Signal>  neg{-1.0};
};


} // namespace SND

#endif
