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

#ifndef SND_N_POLE_FILTER_H
#define SND_N_POLE_FILTER_H

#include <array>

#include "SND/SND.h"

namespace SND {

//! General N zero filter
template <unsigned N>
class NPoleFilter : public UnaryOp
{
public:
   std::array<ControlIn<Signal>, N> a;
   ControlIn<Signal> b0;

   NPoleFilter() {}

   NPoleFilter(const std::array<Signal, N>& a_, Signal b0_)
      : a(a_)
      , b0(b0_)
   {}

private:
   double y[N + 1] = {0.0};

   Signal output()
   {
      y[0] = b0 * in;

      for(unsigned i = N; i > 0; --i)
      {
         y[0] -= a[i - 1] * y[i];
         y[i] = y[i - 1];
      }

      return y[0];
   }
};

} // namespace SND

#endif
