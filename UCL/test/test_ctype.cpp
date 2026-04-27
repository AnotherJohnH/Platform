//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "ctype.h"

#include "test.h"

int test_ctype()
{
   EXPECT_TRUE(isdigit('0'));

   return 0;
}
