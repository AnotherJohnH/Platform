//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

inline NOINLINE void testLcd(TestPhase phase_)
{
   static hw::Lcd lcd{};

   switch(phase_)
   {
   case DECL:
      break;

   case INFO:
      printf("LCD: display \"Hello World!\", an incrementing value and an up arrow\n");
      break;

   case START:
      {
         uint8_t ch[8] =
         {
            0b00000,
            0b00100,
            0b01110,
            0b10101,
            0b00100,
            0b00100,
            0b00100,
            0b00000
         };

         lcd.progChar(0, ch);
      }
      break;

   case RUN:
      {
         static unsigned n{0};

         char text[16];
         snprintf(text, sizeof(text), "Hello %u", n++);

         lcd.move(0, 0);
         lcd.print(text);

         lcd.move(0, 1);
         lcd.print("World!");

         lcd.move(15, 1);
         lcd.putchar('\0');
      }
      break;
   }
}

} // namespace hw
