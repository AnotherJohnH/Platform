//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "Vector.h"

struct Vector
{
   Handler  handler;
   uint32_t data;
};

static Vector exc_table = {MTL_halt, 1};

void VEC_fault()
{
   (*exc_table.handler)(exc_table.data);
}

void MTL_excep(Exception exc, Handler handler, uint32_t data)
{
   if (exc < NUM_EXC)
   {
      exc_table.handler = handler;
      exc_table.data    = data;
   }
}
