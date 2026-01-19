//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include <cmath>

#include "SIG/Types.h"
#include "SIG/Const.h"

#include "Table_atten15.h"

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

//! Convert a 7-bit integer attenuation (dB) to a linear value
//! 0x00 =>    0 dB   ( 1.0)
//! 0x7E =>  -60 dB   (~0.0)
//! 0x7F => -inf dB   ( 0.0)
inline Signal dBatten7_2signal(uint8_t atten7_)
{
   return table_atten15[(atten7_ << 8) | (atten7_ << 1) | (atten7_ >> 6)];
}

//! Convert a 15-bit integer attenuation to a linear value
//! 0x0000 =>    0 dB   ( 1.0)
//! 0x7FFE =>  -60 dB   (~0.0)
//! 0x7FFF => -inf dB   ( 0.0)
inline Signal dBatten15_2signal(unsigned atten15_)
{
   return table_atten15[atten15_];
}

//! Convert an attenuation (dB) to a linear value
//! Any atten_ above 60 dB will be infinity e.g. return 0.0
inline Signal dBatten2signal(Float atten_)
{
   signed index15 = 0x7FFF * atten_ / 60.0f;

   if (index15 < 0)
      return 1.0f;
   else if (index15 > 0x7FFF)
      return 0.0f;

   return dBatten15_2signal(index15);
}

//! Convert a pan to signal using a sine panning function
inline Signal pan2signal(Float pan_)
{
   return sinf(pan_ * Float(M_PI) / 2.0f);
}

} // namespace SIG
