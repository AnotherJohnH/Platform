//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Functions f(t) for some common waveforms were 0.0 <= t <= 1.0

#pragma once

#include <cmath>

namespace STB {

namespace Waveform {


inline double impulse(double t)
{
   return t == 0 ? 1 : 0;
}

inline double square(double t)
{
   return t < 0.5 ? 1 : -1;
}

inline double sawTooth(double t)
{
   return 2 * t - 1;
}

inline double reverseSawTooth(double t)
{
   return 1 - 2 * t;
}

inline double triangle(double t)
{
   return t < 0.5 ? (+4 * t - 1)
                  : (-4 * t + 3);
}

inline double sine(double t)
{
   return sin(2 * M_PI * t);
}

inline double cosine(double t)
{
   return cos(2 * M_PI * t);
}


} // namespace Waveform

} // namespace STB

