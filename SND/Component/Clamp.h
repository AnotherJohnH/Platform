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

#ifndef SND_CLAMP_H
#define SND_CLAMP_H

#include "SND/Control.h"
#include "SND/SignalOp.h"

namespace SND {

//! Clamp signal against controllable max and min limits
class Clamp : public UnaryOp
{
public:
   ControlIn<Signal> max;
   ControlIn<Signal> min;

   Clamp(Signal max_ = +1.0, Signal min_ = -1.0)
      : max(max_)
      , min(min_)
   {}

private:
   virtual Signal output() override
   {
      Signal value = in;

      if(value > max)
      {
         value = max;
      }
      else if(value < min)
      {
         value = min;
      }

      return value;
   }
};


} // namespace SND

#endif
