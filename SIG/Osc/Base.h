//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Types.h"

#include "Table_delta14_7.h"

namespace SIG {

namespace Osc {

class Base
{
public:
   Base() = default;

   void mute()
   {
      phase = 0;
      delta = 0;
   }

   //! Reset phase to zero
   void sync()
   {
      phase = 0;
   }

   //! Set frequency
   void setFreq(float freq_hz_, unsigned step_freq_ = SAMPLE_RATE)
   {
      float ratio = freq_hz_ / step_freq_;

      setDelta(signal2uphase(ratio * 2.0));
   }

   //! Set frequency for MIDI note value
   void setNote(uint8_t midi_note_)
   {
      midi_note = midi_note_;
      updateExpFreq();
   }

   //! Set exponential frequency detune relative to MIDI note (fixed-point-7)
   void setDetune(int32_t detune_)
   {
      exp_freq_detune = detune_;
      updateExpFreq();
   }

   //! Set exponential frequency detune relatine to MIDI note
   void setDetune(Float detune_)
   {
      exp_freq_detune = signed(detune_ * EXP_FREQ_SCALE);
      updateExpFreq();
   }

   //! Get the current phase
   UPhase getPhase() const { return phase; }

   //! Set the current phase
   void setPhase(UPhase phase_) { phase = phase_; }

protected:
   //! Calculate delta for a frequency modulation input
   uint32_t modDelta(Signal mod_)
   {
      return table_delta14_7[exp_freq + signed(EXP_FREQ_SCALE * mod_)];
   }

   void setDelta(UPhase delta_)
   {
      delta = delta_;
      dt    = uphase2float(delta_);
   }

   //! Polynomial to pre-filter hard edges in waveforms in the range [-dt, dt]
   float polyBLEP(float t)
   {
      if (t < dt)
      {
         t = t / dt;
         return t + t - t * t - 1.0f;
      }
      else if (t > (1.0f - dt))
      {
         t = (t - 1.0f) / dt;
         return t + t + t*t + 1.0f;
      }

      return 0.0f;
   }

   UPhase phase{0}; //!< UPhase     (x2pi) Q0.32
   UPhase delta{0}; //!< UPhase inc (x2pi) Q0.32

   static const UPhase UPHASE_QUARTER = 0x40000000;
   static const UPhase UPHASE_HALF    = UPHASE_QUARTER * 2;

private:
   void updateExpFreq()
   {
      exp_freq = (midi_note << EXP_FREQ_FRAC_BITS) + exp_freq_detune;
      setDelta(table_delta14_7[exp_freq]);
   }

   static const unsigned EXP_FREQ_FRAC_BITS = 7;
   static const unsigned EXP_FREQ_SCALE     = 1 << EXP_FREQ_FRAC_BITS;

   uint32_t exp_freq;           //!< Exponential frequency where 69.0 equivalent to 440 Hz (fixed-point-7)
   int32_t  exp_freq_detune{0}; //!< Detune (fixed-point-7)
   uint8_t  midi_note{0};       //!< MIDI note
   float    dt{};
};

} // namespace Osc

} // namespace SIG
