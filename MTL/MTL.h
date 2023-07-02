//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

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
   [[ noreturn ]] extern void MTL_halt(uint32_t status);

   //! Get current millisecond tick count
   uint32_t MTL_clock();

   //! Send character to console
   void MTL_putch(uint8_t ch);

   //! Get character from the console
   int MTL_getch();

   //! Check if character avialable from the console
   bool MTL_getch_empty();

   //! Exception handling
   void MTL_excep(Exception signal, Handler handler, uint32_t data);
}
