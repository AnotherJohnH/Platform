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

// \brief Record pin allocations

#pragma once

#include <cstring>
#include <cstdio>

namespace MTL {

template <unsigned PINS>
class ConfigBase
{
public:
   ConfigBase() = default;

   //! Declare pin usage
   void pin(unsigned pin_, const char* label_)
   {
      getRef(pin_) = label_;
   }

   //! Declare GPIO usage
   void gpio(unsigned gpio_, const char* label_)
   {
      pin(gpioToPin(gpio_), label_);
   }

   //! Format config into the gievn buffer
   size_t format(char* buffer_, size_t size_) const
   {
      unsigned left_max = 0;

      for(unsigned row = 1; row <= (NUM_PINS / 2); ++row)
      {
          unsigned left_pin = row;
          size_t len = strlen(get(left_pin));
          if (len > left_max)
             left_max = len;
      }

      char* s     = buffer_;
      char* end_s = buffer_ + size_;

      s += snprintf(s, end_s - s, "\n");

      for(unsigned row = 1; row <= (NUM_PINS / 2); ++row)
      {
          unsigned left_pin  = row;
          unsigned right_pin = NUM_PINS - row + 1;

          const char* label = get(left_pin);
          for(unsigned i = 0; i < (left_max - strlen(label)); ++i)
          {
             if (s < end_s)
                *s++ = ' ';
          }

          s += snprintf(s, end_s - s, "%s | %2u     %2u | %s\n",
                        label, left_pin, right_pin, get(right_pin));
      }

      return s - buffer_;
   }

protected:
   //! Convert GPIO number to PIN number
   virtual unsigned gpioToPin(unsigned gpio_)
   {
      return 0;
   }

   //! Get a reference to the label for the given pin
   const char*& getRef(unsigned pin_)
   {
      if ((pin_ < 1) || (pin_ > NUM_PINS))
         return label[0];

      return label[pin_];
   }

   //! Get the label for the given pin
   const char* get(unsigned pin_) const
   {
      if ((pin_ < 1) || (pin_ > NUM_PINS))
         return "";

      if (label[pin_] == nullptr)
         return "";

      return label[pin_];
   }

   static constexpr unsigned NUM_PINS = PINS;

private:
   const char* label[NUM_PINS];
};

} // namespace MTL
