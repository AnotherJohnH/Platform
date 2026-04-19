//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Const.h"
#include "Types.h"

namespace SIG {

//! Variable length sample delay
template <unsigned MAX_LENGTH>
class DelayV
{
   static_assert(MAX_LENGTH != 0);

public:
   DelayV() = default;

   //! Set delay length in samples
   void setLength(unsigned samples_)
   {
           if (samples_ == 0)          next_length = 1;
      else if (samples_ <= MAX_LENGTH) next_length = samples_;
   }

   //! Set delay length in seconds
   void setLengthT(float seconds_)
   {
      if (seconds_ < 0.0f)
         return;

      setLength(SAMPLE_RATE * seconds_);
   }

   //! \return the size of the delay (samples)
   unsigned size() const { return length; }

   //! Reset the delay to default state
   void reset()
   {
      index       = 0;
      length      = MAX_LENGTH;
      next_length = MAX_LENGTH;

      for(unsigned i = 0; i < size(); ++i)
         buffer[i] = Signal{};
   }

   //! Read the current output sample
   Signal operator()() const { return read(); }

   //! Provide the next input sample
   void operator=(Signal x_) { write(x_); }

   //! Provide the next input sample and return the current output sample
   Signal operator()(Signal x_) { return process(x_); }

private:
   //! Read the current output sample
   Signal read() const { return buffer[index]; }

   //! Provide the next input sample
   void write(Signal x_)
   {
      buffer[index] = x_;

      if (++index >= length)
      {
         length = next_length;
         index  = 0;
      }
   }

   //! Provide the next input sample and return the current output sample
   Signal process(Signal x_)
   {
      Signal y = read();
      write(x_);
      return y;
   }

   unsigned index{0};
   unsigned length{MAX_LENGTH};
   unsigned next_length{MAX_LENGTH};
   Signal   buffer[MAX_LENGTH] = {};
};

} // namespace SIG
