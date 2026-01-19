//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

#include "SIG/Types.h"

namespace SIG::Env {

class Adsr
{
public:
   Adsr()
   {
      phase_rate[OFF]      = 0;
      phase_level[OFF]     = 0;

      phase_rate[ATTACK]   = MAX;
      phase_level[ATTACK]  = MAX;

      phase_rate[DECAY]    = 0;
      phase_level[DECAY]   = MAX; 

      phase_rate[SUSTAIN]  = 0;
      phase_level[SUSTAIN] = MAX;

      phase_rate[RELEASE]  = MAX;
      phase_level[RELEASE] = 0;
   }

   bool isOff() const { return phase == OFF; }

   void setAttack_mS(unsigned time_ms_)
   {
      phase_samples[ATTACK] = (time_ms_ * SAMPLE_RATE / 1000) + 1;
      phase_rate[ATTACK]    = MAX / phase_samples[ATTACK];

      if (phase == ATTACK)
      {
         rate = phase_rate[ATTACK];
      }
   }

   void setDecay_mS(unsigned time_ms_)
   {
      phase_samples[DECAY] = (time_ms_ * SAMPLE_RATE / 1000) + 1;
      phase_rate[DECAY]    = -int32_t(MAX / phase_samples[DECAY]);

      if (phase == DECAY)
      {
         rate = phase_rate[DECAY];
      }
   }

   void setSustain(uint8_t level7_)
   {
      phase_level[DECAY] = phase_level[SUSTAIN] = scaleLevel(level7_);

      if (phase == DECAY)
      {
         target = phase_level[SUSTAIN];
      }
      else if (phase == SUSTAIN)
      {
         level = target = phase_level[SUSTAIN];
      }
   }

   void setSustain_mS(unsigned time_ms_)
   {
      phase_samples[SUSTAIN] = time_ms_ * SAMPLE_RATE / 1000;
      phase_rate[SUSTAIN]    = 0;
   }

   void setRelease_mS(unsigned time_ms_)
   {
      phase_samples[RELEASE] = (time_ms_ * SAMPLE_RATE / 1000) + 1;
      phase_rate[RELEASE]    = -int32_t(MAX / phase_samples[RELEASE]);

      if (phase == RELEASE)
      {
         rate = phase_rate[RELEASE];
      }
   }

   //! Gate on
   void on() { setPhase(ATTACK); }

   //! Gate off
   void off() { setPhase(RELEASE); }

   void mute() { setPhase(OFF); }

   //! Return next envelope sample
   Signal operator()()
   {
      level += rate;

      if (rate <= 0)
      {
         if (level < target)
         {
            level = target;
            setPhase(EnvPhase(phase + 1));
         }
      }
      else
      {
         if (level >= target)
         {
            level = target;
            setPhase(EnvPhase(phase + 1));
         }
      }

      return dBatten15_2signal(0x7FFF - (level >> 8));
   }

private:
   static constexpr int32_t MAX = 0x7FFFFF;

   enum EnvPhase { OFF, ATTACK, DECAY, SUSTAIN, RELEASE, NUM_PHASES };

   static int32_t scaleLevel(uint8_t level7_)
   {
      return (level7_ << 16) | (level7_ << 9) | (level7_ << 2) | (level7_ >> 5);
   }

   void setPhase(EnvPhase phase_)
   {
      if (phase_ == NUM_PHASES)
      {
         phase_ = OFF;
      }

      phase   = phase_;
      rate    = phase_rate[phase];
      target  = phase_level[phase];
      samples = phase_samples[phase];
   }

   EnvPhase phase{OFF};
   int32_t  level{0};
   int32_t  rate{0};
   int32_t  target{0};
   int32_t  samples{0};
   int32_t  phase_rate[NUM_PHASES]  = {};
   int32_t  phase_level[NUM_PHASES] = {};
   int32_t  phase_samples[NUM_PHASES] = {};
};

} // namespace SIG::Env
