//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#include <stdlib.h>
#include <stdint.h>

static uint32_t rand_state = 1;

int rand(void)
{
   // use xorshift, it's fast and simple
   rand_state ^= rand_state << 13;
   rand_state ^= rand_state >> 17;
   rand_state ^= rand_state << 5;

   return rand_state & RAND_MAX;
}
