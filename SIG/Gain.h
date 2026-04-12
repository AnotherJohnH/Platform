//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "Types.h"

namespace SIG {

class Gain
{
public:
   Gain(Signal value_ = Signal{1.0}) : value(value_) {}

   //! \return current (linear) gain
   operator Signal() const { return value; }

   //! Set current (linear) gain
   Signal operator=(Signal value_) { return value = value_; }

   //! Apply gain to a signal
   Signal operator()(Signal in_) const { return in_ * value; }

private:
   Signal value{1.0};
};

} // namespace SIG
