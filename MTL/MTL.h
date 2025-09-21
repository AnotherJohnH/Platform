//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief C API

#pragma once

#include <stdint.h>

using Handler = void (*)(uint32_t);

enum Exception
{
   EXC_BUS,
   EXC_UND,
   EXC_FPE,
   NUM_EXC
};

extern "C"
{
   //! Intialise the platform
   void MTL_init();

   //! Initialise image
   void MTL_data_and_bss();

   //! Construct global objects
   void MTL_global_construction();

   //! Intialise the application
   void MTL_load();

   //! Application entry point
   int MTL_main();

   //! Halt platform
   [[ noreturn ]] void MTL_halt(uint32_t status);

   //! Get current 100 Hz tick count
   uint32_t MTL_clock();

   //! Get current microsecond tick count
   uint32_t MTL_us_clock();

   //! Send character to console
   void MTL_putch(uint8_t ch);

   //! Get character from the console
   int MTL_getch();

   //! Check if character avialable from the console
   bool MTL_getch_empty();

   //! Disable console buffering (avoids IRQs)
   void MTL_nobuff();

   //! Fault handler
   [[ noreturn ]] void MTL_fault(unsigned excep_num, uint32_t* fault_stack_ptr);

   //! Exception handling
   void MTL_excep(Exception signal, Handler handler, uint32_t data);

   //! Start another core
   bool MTL_start_core(unsigned index, void (*func)());
}
