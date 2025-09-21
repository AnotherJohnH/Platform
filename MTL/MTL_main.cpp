//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

__attribute__((weak))
int MTL_main()
{
   extern int main(int, const char*[]);

   const char* argv[1] = {"metal"};

   return main(1, argv);
}
