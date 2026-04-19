//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "HWR/picoGame/Config.h"

#include "HWR/Device/Test/TestLed.h"
#include "HWR/Device/Test/TestDisplay.h"
#include "HWR/Device/Test/TestUsbFile.h"
#include "HWR/Device/Test/TestButtons.h"

static void test(HWR::TestPhase phase_)
{
   if (1) HWR::testLed(phase_);
   if (1) HWR::testDisplay(phase_);
   if (1) HWR::testUsbFile(phase_);
   if (1) HWR::testButtons(phase_);
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
   printf("Version  : %s\n", PDK_VERSION);
   printf("Commit   : %s\n", PDK_COMMIT);
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
   test(HWR::DECL);

   consoleReport();

   printf("INFO\n\n");

   test(HWR::INFO);

   printf("\nSTART\n");

   test(HWR::START);

   printf("RUN\n\n");

   while(true)
   {
      test(HWR::RUN);

      usleep(500000);
   }

   return 0;
}
