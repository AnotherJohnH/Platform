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

// \file Gpio.h
// \brief Stub general purpose I/O

#ifndef MTL_GPIO_H
#define MTL_GPIO_H

#include <cstdint>

namespace MTL {

namespace Gpio {


template <unsigned WIDTH, unsigned PIN>
class Out
{
public:
   operator uint32_t() const
   {
      return 0;
   }

   uint32_t operator=(uint32_t data)
   {
      return data;
   }

   void set(uint32_t data)
   {
   }

   void clr(uint32_t data)
   {
   }
};


template <unsigned WIDTH, unsigned PIN>
class In
{
public:
   operator uint32_t() const
   {
      return 0;
   }
};


} // namespace GPIO

} // namespace MTL

#endif // STUB_GPIO_H
