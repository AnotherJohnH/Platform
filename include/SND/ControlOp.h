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

#ifndef SND_CONTROL_OP_H
#define SND_CONTROL_OP_H

#include <cstdlib>

#include "SND/Control.h"

namespace SND {


template <typename TYPE>
class ControlMul : public ControlPort<TYPE>
{
public:
   ControlIn<TYPE>  control;

   ControlMul(const TYPE& initial = 1.0)
      : control(this)
   {
      control = initial;
   }

private:
   virtual void controlEvent(Control*) override
   {
      this->send(this->recv() * control);
   }
};


template <typename TYPE>
class ControlAdd : public ControlPort<TYPE>
{
public:
   ControlIn<TYPE>  control;

   ControlAdd(const TYPE& initial = 0.0)
      : control(this)
   {
      control = initial;
   }

private:
   virtual void controlEvent(Control*) override
   {
      this->send(this->recv() + control);
   }
};


} // namespace SND

#endif
