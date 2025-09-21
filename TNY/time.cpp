//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <time.h>

time_t time(time_t* t_ptr)
{
   time_t t = clock() / CLOCKS_PER_SEC;
   if (t_ptr != nullptr)
   {
      *t_ptr = t;
   }
   return t;
}
