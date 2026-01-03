//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <unistd.h>

#include "Hardware/blinky/Config.h"

static hw::Led led;

int main()
{
   led = true;

   while(true)
   {
      led = not led;

      usleep(500000);
   }

   return 0;
}
