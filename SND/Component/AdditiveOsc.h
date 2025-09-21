//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
