//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

inline NOINLINE void testLed7Seg(TestPhase phase_)
{
   static hw::Led7Seg led{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("LED-7SEG: an incrementing value\n");
      break;

   case START:
      break;

   case RUN:
      {
         static unsigned n{0};

         led.printDec(n, n >= 100 ? 0 : 3);

         n++;
      }
      break;
   }
}

} // namespace hw
