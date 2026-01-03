//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>
#include <unistd.h>

#include "Hardware/picoX21H/Config.h"

#include "Table_sine.h"

#include "Hardware/Device/Test/TestLed.h"
#include "Hardware/Device/Test/TestLcd.h"
#include "Hardware/Device/Test/TestPhysMidi.h"
#include "Hardware/Device/Test/TestUsbFileMidi.h"
#include "Hardware/Device/Test/TestDac.h"
#include "Hardware/Device/Test/TestYM2151.h"

static void test(hw::TestPhase phase_)
{
   if (1) hw::testLed(phase_);
   if (1) hw::testLcd(phase_);
   if (1) hw::testPhysMidi(phase_);
   if (1) hw::testUsbFileMidi(phase_);
   if (1) hw::testDac(phase_);
   if (1) hw::testYM2151(phase_);
}

static void consoleReport()
{
   // Clear screen and cursor to home
   printf("\033[2J\033[H");

   printf("HARDWARE TEST\n");
   printf("\n");
   printf("Program  : Test picoX21H (%s)\n", HW_DESCR);
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
