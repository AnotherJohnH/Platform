//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \file  Yield.h
//! \brief Real-time scheduling

#pragma once

#include <cstdint>

//! Platform abstraction layer
namespace PLT {

//!
class Yield
{
public:
   Yield(unsigned freq_hz)
      : period_ticks(TICKS_PER_SEC / freq_hz)
   {
      next_event_ticks = getTicks() + period_ticks;
   }

   //! Idle until next tick event
   void yield()
   {
      while(true)
      {
         int32_t delta_ticks = next_event_ticks - getTicks();
         if (delta_ticks <= 0)
         {
            next_event_ticks += period_ticks;
            break;
         }

         yieldForTicks(delta_ticks);
      }
   }

private:
   static const int32_t TICKS_PER_SEC;
   int32_t period_ticks;
   int32_t next_event_ticks;

   static int32_t getTicks();
   static void    yieldForTicks(int32_t ticks);
};

} // namespace PLT

