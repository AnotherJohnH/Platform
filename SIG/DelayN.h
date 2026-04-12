//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Types.h"

namespace SIG {

//! Fixed length sample delay
template <unsigned LENGTH>
class DelayN
{
   static_assert(LENGTH != 0);

public:
   DelayN() = default;

   //! \return the size of the delay (samples)
   unsigned size() const { return LENGTH; }

   //! Reset the delay to default state
   void reset()
   {
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

      if (++index >= LENGTH)
         index = 0;
   }

   //! Provide the next input sample and return the current output sample
   Signal process(Signal x_)
   {
      Signal y = read();
      write(x_);
      return y;
   }

   unsigned index{0};
   Signal   buffer[LENGTH] = {};
};

} // namespace SIG
