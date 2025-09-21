//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#ifndef SND_GAIN_H
#define SND_GAIN_H

#include "SND/SignalOp.h"

namespace SND {

//! Gain controller
class Gain : public UnaryOp
{
public:
   ControlIn<Signal> control;

   Gain(Signal initial = 0.0)
      : control(initial)
   {}

private:
   virtual Signal output() override { return in * control; }
};

} // namespace SND

#endif
