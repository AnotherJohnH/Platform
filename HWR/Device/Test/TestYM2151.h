//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

inline NOINLINE void testYM2151(TestPhase phase_)
{
#if !defined(HW_NATIVE)
   static hw::YM2151<> ym2151{};
#endif

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("YM2151: Only build and declare for now\n");
      break;

   case START:
      break;

   case RUN:
      break;
   }
}

} // namespace hw
