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

//! \file Digital.h
//! \brief Low level digital I/O

#ifndef MTL_DIGITAL_H
#define MTL_DIGITAL_H

#include "MTL/Pins.h"
#include "MTL/Gpio.h"

//! Bare metal interfaces
namespace MTL {

//! Low level digital I/O
namespace Digital {


//! Single digital output
template <unsigned PIN>
class Out
{
public:
   Out(bool init = false) : gpio(true)
   {
      operator=(init);
   }

   bool operator=(bool state)
   {
      if (state)
         gpio.set(1);
      else
         gpio.clr(1);

      return state;
   }

   operator bool() const
   {
      return gpio != 0;
   }

private:
   Gpio<1,PIN>  gpio;
};


} // namespace Digital

} // namespace MTL

#endif // MTL_DIGITAL_H
