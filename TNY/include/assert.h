//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#if defined(NDEBUG)

#define assert(expr) ((void)0)

#else

#include <stdlib.h>
#include <stdio.h>

#define assert(expr) \
   do { \
      if ((expr) == 0) { \
         fprintf(stderr, \
                 "Assert fail: (" #expr ")" \
                 ", func %s, file %s, line %d\n", \
                 __func__, \
                 __FILE__, \
                 __LINE__); \
         abort(); \
      } \
   } while(1 == 0);

#endif // NDEBUG
