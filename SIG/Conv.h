//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cmath>

#include "SIG/Types.h"
#include "SIG/Const.h"

#include "Table_gain.h"
#include "Table_note.h"

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


//! Convert MIDI note with 7 bit frac to a phase increment for the current SIG::SAMPLE_RATE
//!   0.0 C-2 =>     8.18 Hz
//!  69.0 A4  =>   440.00 Hz
//! 127.0 G9  => 12543.85 Hz
inline UPhase noteLookup_7(unsigned note_7_)
{
   return table_note[note_7_];
}

//! Convert MIDI note to a phase increment for the current SIG::SAMPLE_RATE
//!   0 C-2 =>     8.18 Hz
//!  69 A4  =>   440.00 Hz
//! 127 G9  => 12543.85 Hz
inline UPhase noteLookup(uint8_t note_, uint8_t frac7_ = 0)
{
   return noteLookup_7((note_ << 7) | frac7_);
}


//! Convert a 15-bit signed integer gain dB to a linear value
//! -0x6000 => is  -inf dB    0.0         Hard mute
//! -0x5FFF => is ~ -60 dB   ~0.001
//! -0x4000 => is   -40 dB    0.01
//! -0x2000 => is   -20 dB    0.1
//!  0x0000 => is     0 dB    1.0
//!  0x1FFF => is ~ +20 dB  ~10.0
inline Signal dBGainLookup_15(signed gain15_)
{
   constexpr Float    TABLE_STEP_PER_DB = TABLE_GAIN_SIZE / (GAIN_MAX_DB - GAIN_MIN_DB);
   constexpr signed   INDEX_ZERO_DB     = -GAIN_MIN_DB * TABLE_STEP_PER_DB;
   constexpr unsigned GAIN_SHIFT        = 15 - LOG2_TABLE_GAIN_SIZE;

   signed index = INDEX_ZERO_DB + (gain15_ >> GAIN_SHIFT);

   if (index < 0)
      index = 0;
   else if (index >= TABLE_GAIN_SIZE)
      index = TABLE_GAIN_SIZE - 1;

   return table_gain[index];
}

//! Convert a gain -60..0..+20 (dB) to a linear value 0.001..1.0..10.0
inline Signal dBGainLookup(Float gain_)
{
   constexpr Float GAIN15_STEP_PER_DB = (1 << 15) / (GAIN_MAX_DB - GAIN_MIN_DB);

   signed gain15 = signed(gain_ * GAIN15_STEP_PER_DB + 0.5f);

   return dBGainLookup_15(gain15);
}

//! Convert a 7-bit integer attenuation (dB) to a linear value
//! 0x00 =>     0 dB   ( 1.000)
//! 0x7E => ~ -60 dB   (~0.001)
//! 0x7F =>  -inf dB   ( 0.000)
inline Signal dBAttenLookup_7(uint8_t atten7_)
{
   signed gain15 = -atten7_ * (GAIN15_60_DB / 0x7E); // Fudged to make 0x7F -> 0.0

   return dBGainLookup_15(gain15);
}

//! Convert a pan to signal using a sine panning function
inline Signal pan2signal(Float pan_)
{
   return sinf(pan_ * Float(M_PI) / 2.0f);
}

} // namespace SIG
