/*------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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
//----------------------------------------------------------------------------*/

//! \brief tiny C library implementation

#include <stdint.h>
#include <string.h>

void TNY_init()
{
   // Initialise data section
   extern uint8_t __etext;
   extern uint8_t __data_start__;
   extern uint8_t __data_end__;
   memcpy(&__data_start__, &__etext, &__data_end__ - &__data_start__);

   // Clear BSS
   extern uint8_t __bss_start__;
   extern uint8_t __bss_end__;
   memset(&__bss_start__, 0, &__bss_end__ - &__bss_start__);

   // Call global constructors
   extern void(*__init_array_start)();
   extern void(*__init_array_end)();
   void (**func)();
   for(func = &__init_array_start; func < &__init_array_end; ++func)
   {
      (**func)();
   }
}
