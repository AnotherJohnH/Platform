//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

inline NOINLINE void testDisplay(TestPhase phase_)
{
   static const STB::Colour WHITE = STB::RGB(0xC0, 0xC0, 0xC0);
   static const STB::Colour BLACK = STB::RGB(0x00, 0x00, 0x00);

   static hw::Display::Canvas canvas{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("Display: \n");
      break;

   case START:
      canvas.clear(WHITE);
      canvas.drawLine(BLACK, 0, 0, canvas.getWidth(), canvas.getHeight());
      canvas.drawLine(BLACK, 0, canvas.getHeight(), canvas.getWidth(), 0);
      canvas.refresh();
      break;

   case RUN:
      break;
   }
}

} // namespace hw
