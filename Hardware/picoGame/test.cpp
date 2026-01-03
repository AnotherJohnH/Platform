//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoGame/Config.h"

#include "Hardware/Device/Test/TestLed.h"
#include "Hardware/Device/Test/TestDisplay.h"
#include "Hardware/Device/Test/TestUsbFile.h"
#include "Hardware/Device/Test/TestButtons.h"

static void test(hw::TestPhase phase_)
{
   if (1) hw::testLed(phase_);
   if (1) hw::testDisplay(phase_);
   if (1) hw::testUsbFile(phase_);
   if (1) hw::testButtons(phase_);
}

static void consoleReport()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("HARDWARE TEST\n");
   printf("\n");
   printf("Program  : Test picoGame (%s)\n", HW_DESCR);
   printf("Author   : Copyright (c) 2025 John D. Haughton\n");
   printf("License  : MIT\n");
   printf("Version  : %s\n", PLT_VERSION);
   printf("Commit   : %s\n", PLT_COMMIT);
   printf("Built    : %s %s\n", __TIME__, __DATE__);
   printf("Compiler : %s\n", __VERSION__);

#if !defined(HW_NATIVE)
   char config[1024];
   MTL::config.format(config, sizeof(config));
   puts(config);
#endif
}

int main()
{
   test(hw::DECL);

   consoleReport();

   printf("INFO\n\n");

   test(hw::INFO);

   printf("\nSTART\n");

   test(hw::START);

   printf("RUN\n\n");

   while(true)
   {
      test(hw::RUN);

      usleep(500000);
   }

   return 0;
}
