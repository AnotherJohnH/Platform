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
   Gain(Signal value_ = 1.0f) : value(value_) {}

   operator Signal() const { return value; }

   Signal operator=(Signal value_) { return value = value_; }

   Signal operator()(Signal in_) { return in_ * value; }

private:
   Signal value{1.0f};
};

} // namespace SIG
