//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

inline NOINLINE void testPhysMidi(TestPhase phase_)
{
   static hw::PhysMidi phys_midi{};
   
   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("MIDI: Laggy log of MIDI messages on the console\n" );
      break;
   
   case START:
      phys_midi.setDebug(true);
      break;

   case RUN:
      phys_midi.tick();
      break;
   }
}

} // namespace hw
