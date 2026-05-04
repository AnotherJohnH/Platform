//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "UCL/stdlib.h"

#include "stdint.h"

static uint32_t rand_state = 1;

void srand(unsigned seed_)
{
   rand_state = seed_;
}

int rand(void)
{
   // use xorshift, it's fast and simple
   rand_state ^= rand_state << 13;
   rand_state ^= rand_state >> 17;
   rand_state ^= rand_state << 5;

   return rand_state & RAND_MAX;
}
