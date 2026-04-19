//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "Types.h"

namespace SIG {

//! Single sample delay
class Delay
{
public:
   Delay() = default;

   //! \return the size of the delay (samples)
   unsigned size() const { return 1; }

   //! Reset the delay to default state
   void reset() { buffer = Signal{}; }

   //! Read the current output sample
   Signal operator()() const { return read(); }

   //! Provide the next input sample
   void operator=(Signal x_) { write(x_); }

   //! Provide the next input sample and return the current output sample
   Signal operator()(Signal x_) { return process(x_); }

private:
   //! Read the current output sample
   Signal read() const { return buffer; }

   //! Provide the next input sample
   void write(Signal x_) { buffer = x_; }

   //! Provide the next input sample and return the current output sample
   Signal process(Signal x_)
   {
      Signal y = read();
      write(x_);
      return y;
   }

   Signal buffer{};
};

} // namespace SIG
