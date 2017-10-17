//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

// Functions f(x) for some common waveforms were 0.0 <= x <= 1.0

#ifndef MTH_WAVEFORM_H
#define MTH_WAVEFORM_H

#include <cmath>

namespace MTH {

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

} // namespace MTH

#endif
