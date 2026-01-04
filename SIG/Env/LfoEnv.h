//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------
   
#pragma once

namespace SIG {

class LfoEnv
{
public:
   LfoEnv() = default;

   //! Set LFO delay (s)
   void setDelay(float delay_s_)
   {
      delay_samples = (delay_s_ * SAMPLE_RATE) + 1;

      if ((phase == DELAY) && (samples > delay_samples))
      {
         samples = delay_samples;
      }
   }

   //! Set LFO attack time (s)
   void setAttack(float attack_s_)
   {
      attack_samples = (attack_s_ * SAMPLE_RATE) + 1;
      attack_rate    = 1.0 / attack_samples;
   }

   //! Gate on
   void on()
   {
      phase   = DELAY;
      samples = delay_samples;
      level   = 0;
      rate    = 0;
   }

   //! Return next envelope sample
   Signal operator()()
   {
      level += rate;

      if (--samples == 0)
      {
         if (phase == DELAY)
         {
            phase   = ATTACK;
            samples = attack_samples;
            rate    = attack_rate;
         }
         else
         {
            phase = SUSTAIN;
            level = 1.0;
            rate  = 0;
         }
      }

      return level;
   }

private:
   enum EnvPhase { DELAY, ATTACK, SUSTAIN };

   EnvPhase phase{SUSTAIN};
   Signal   level{0.0};
   Signal   rate{0.0};
   unsigned samples{0};
   unsigned delay_samples{0};
   unsigned attack_samples{};
   Signal   attack_rate{0.0};
};

} // namespace SIG
