//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Types.h"
#include "SIG/Const.h"

namespace SIG {

//! Convert 32-bit unsigned phase to floating-point -1.0..1.0 (pi)
inline Signal uphase2signal(UPhase uphase_)
{
   return Signal(SPhase(uphase_)) / 0x80000000;
}

//! Convert floating point sample -1.0..1.0 to 32-bit unsigned phase
inline UPhase signal2uphase(Signal signal_)
{
   return UPhase(SPhase(signal_ * SPHASE_MAX));
}

//! Convert 32-bit unsigned phase to floating-point 0.0..1.0 (2pi)
inline Phase uphase2float(UPhase uphase_)
{
   return Phase(uphase_ >> 1) / 0x80000000;
}

} // namespace SIG
