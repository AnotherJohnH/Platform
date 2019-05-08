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

#ifndef SND_ADDITIVE_OSC_H
#define SND_ADDITIVE_OSC_H

#include "WaveTableOsc.h"

namespace SND {

//! Oscilator with fourier coefficient controls
template <unsigned N>
class AdditiveOsc : public WaveTableOsc
{
public:
   ControlIn<double> a[N+1];
   ControlIn<double> b[N+1];

   AdditiveOsc(Freq freq_hz = 0.0)
      : WaveTableOsc(freq_hz)
   {
      for(size_t n = 0; n <= N; n++)
      {
         a[n].setObserver(this);
         b[n].setObserver(this);
      }
   }

private:
   // TODO convert back to Lambda when embedded builds allow
   double func(double t)
   {
      double value = a[0] / 2.0;
      for(size_t n = 1; n <= N; n++)
      {
         value += a[n] * STB::Waveform::sine(n * t);
         value += b[n] * STB::Waveform::cosine(n * t);
      }
      return value;
   }

   static double thunk(double t, void* that)
   {
      AdditiveOsc* osc = (AdditiveOsc*)that;
      return osc->func(t);
   }

   virtual void controlEvent(Control* control) override
   {
      WaveTableOsc::controlEvent(control);
      computeWave(thunk, this);
   }
};

} // namespace SND

#endif
