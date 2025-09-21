//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SND/SignalOp.h"

namespace SND {

//! Add a controller signal to an audio signal
class Bias : public UnaryOp
{
public:
   ControlIn<Signal> control;

   Bias(Signal initial = 0.0)
      : control(initial)
   {}

private:
   virtual Signal output() override { return in + control; }
};

} // namespace SND

