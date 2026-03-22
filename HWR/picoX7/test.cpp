//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "HWR/picoX7/Config.h"

#include "Table_sine.h"

#include "HWR/Device/Test/TestLed.h"
#include "HWR/Device/Test/TestLcd.h"
#include "HWR/Device/Test/TestPhysMidi.h"
#include "HWR/Device/Test/TestUsbFileMidi.h"
#include "HWR/Device/Test/TestLed7Seg.h"
#include "HWR/Device/Test/TestAudio.h"
#include "HWR/Device/Test/TestButtons.h"

static void test(HWR::TestPhase phase_)
{
   if (1) HWR::testLed(phase_);
   if (1) HWR::testLcd(phase_);
   if (1) HWR::testPhysMidi(phase_);
   if (1) HWR::testUsbFileMidi(phase_);
   if (1) HWR::testLed7Seg(phase_);
   if (1) HWR::testAudio(phase_);
   if (1) HWR::testButtons(phase_);
}

static void consoleReport()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("HARDWARE TEST\n");
   printf("\n");
   printf("Program  : Test picoX7 (%s)\n", HW_DESCR);
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
