//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdint>

#include "MTL/MTL.h"

//! Initialise data section and clear BSS
void MTL_data_and_bss()
{
   extern uint8_t __etext[];
   extern uint8_t __data_start__[];
   extern uint8_t __data_end__;
   extern uint8_t __bss_start__[];
   extern uint8_t __bss_end__;

   for(signed i = 0; i < &__data_end__ - &__data_start__[0]; ++i)
   {
      __data_start__[i] = __etext[i];
   }

   for(signed i = 0; i < &__bss_end__ - &__bss_start__[0]; ++i)
   {
      __bss_start__[i] = 0;
   }
}

//! Call global constructors
void MTL_global_construction()
{
   extern void (*__init_array_start)();
   extern void (*__init_array_end)();

   for(void (**func)() = &__init_array_start; func < &__init_array_end; ++func)
   {
      (**func)();
   }
}

