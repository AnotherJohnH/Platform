//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "Test.h"

#include <cstdio>

namespace hw {

static hw::Buttons* buttons_ptr{nullptr};

extern "C" void IRQ_IO_BANK0()
{
   if (buttons_ptr != nullptr)
   {
      buttons_ptr->irq();
   }
}

inline NOINLINE void testButtons(TestPhase phase_)
{
   static hw::Buttons buttons{};

   switch(phase_)
   {
   case DECL:
      buttons_ptr = &buttons;
      break;

   case INFO:
      printf("Buttons: Debug trace of buttons pressed\n");
      break;

   case START:
      break;

   case RUN:
      {
         unsigned index{};
         bool     pressed{};

         if (buttons.popEvent(index, pressed))
         {
            printf("BTN%u %s\n", index + 1, pressed ? "DN" : "UP");
         }
      }
      break;
   }
}

} // namespace hw
