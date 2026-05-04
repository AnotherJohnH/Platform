//-------------------------------------------------------------------------------
// Copyright (c) YEAR AUTHOR
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cunistd>

#include "HWR/blinky/Config.h"

static HWR::Led led;

int main()
{
   led = false;

   while(true)
   {
      led = not led;

      usleep(500000);
   }

   return 0;
}
