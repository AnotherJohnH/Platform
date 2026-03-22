//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace HWR {

inline NOINLINE void testLed(TestPhase phase_)
{
   static HWR::Led led{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("LED: flash at 1Hz\n");
      break;

   case START: break;

   case RUN:
      led = not led;
      break;
   }
}

} // namespace HWR
