//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "SND/SignalOp.h"

namespace SND {

//! Gate control
class Gate : public UnaryOp
{
public:
   ControlIn<bool> control;

private:
   virtual Signal output() override { return control ? Signal(in) : Signal(0); }
};

} // namespace SND

