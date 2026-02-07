//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG.h"

namespace SIG {

template <unsigned MAX_LENGTH>
class DelayV
{
public:
   DelayV() = default;

   void setLength(unsigned samples_)
   {
      if (samples_ <= MAX_LENGTH)
         next_length = samples_;
   }

   void setLengthT(float seconds_)
   {
      setLength(SAMPLE_RATE * seconds_);
   }

   Signal operator()() const { return buffer[index]; }

   void operator=(Signal x_)
   {
      buffer[index] = x_;

      if (++index >= length)
      {
         length = next_length;
         index  = 0;
      }
   }

   Signal operator()(Signal x_)
   {
      Signal y = buffer[index];

      operator=(x_);

      return y;
   }

private:
   unsigned index{0};
   unsigned length{MAX_LENGTH};
   unsigned next_length{MAX_LENGTH};
   Signal   buffer[MAX_LENGTH] = {};
};

} // namespace SIG
