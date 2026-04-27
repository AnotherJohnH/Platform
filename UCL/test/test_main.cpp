//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "test.h"

extern int test_ctype();

bool TST::pass;

int main()
{
   int status{0};

   status += test_ctype();

   return status;
}
