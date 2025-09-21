//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "STB/FAT/FAT16.h"

#include "STB/Test.h"

#include <cstdio>

TEST(STB_FAT16, construct)
{
   STB::FAT16<128> fat16{"picoChippy"};
   uint8_t         block[512];

   fat16.read(0, 0, sizeof(block), block);

   for(unsigned i = 0; i < 64; ++i)
   {
      printf(" %02X", block[i]);

      if ((i % 16) == 15) putchar('\n');
   }
}
