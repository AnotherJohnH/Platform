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

static Vector exc_table[NUM_EXC] =
{
   {MTL_halt, 1},
   {MTL_halt, 2},
   {MTL_halt, 3}
};

void VEC_bus_fault()
{
   (*exc_table[EXC_BUS].handler)(exc_table[EXC_BUS].data);
}

void VEC_usage_fault()
{
   (*exc_table[EXC_UND].handler)(exc_table[EXC_UND].data);
}

void MTL_excep(Exception exc, Handler handler, uint32_t data)
{
   if (exc < NUM_EXC)
   {
      exc_table[exc].handler = handler;
      exc_table[exc].data    = data;
   }
}
