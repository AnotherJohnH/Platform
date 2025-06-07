//------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
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

// \brief Record of hardware pin allocations

#include <cstdint>

#include "MTL/Config.h"

MTL::Config::Config()
{
   MTL::Config& self = *this;

   for(unsigned p = 3; p < NUM_PINS; p += 5)
   {
      pin(p, "gnd");
   }

   pin(30, "run");
   pin(33, "agnd");
   pin(35, "adc-vref");
   pin(36, "3v3");
   pin(37, "3v3-en");
   pin(39, "vsys");
   pin(40, "vbus +5v");
}

unsigned MTL::Config::gpioToPin(unsigned gpio_)
{
   static const uint8_t map[32] = 
   {
       1,  2,  4,  5,  6,  7,  9, 10, 11, 12, 14, 15, 16, 17, 19, 20,
      21, 22, 24, 25, 26, 27, 29,  0,  0,  0, 31, 32, 34,  0,  0,  0
   };

   return gpio_ < 32 ? map[gpio_] : 0;
}

MTL::Config MTL::config{};
