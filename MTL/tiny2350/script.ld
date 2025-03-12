/*------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

MEMORY
{
   FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 4M
   RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 512K
}

ENTRY(vector_table_core0)

SECTIONS
{
   .text :
   {
      __text_start__ = .;
      KEEP(*(.vectors))
      KEEP(*(.entry))
      *(.text*)
      __init_array_start = ALIGN(4);
      KEEP(*(.init_array))
      __init_array_end = .;
      *(.rodata*)
      __text_end__ = .;

   } > FLASH

   .ARM.exidx :
   {
      __exidx_start = .;
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
      __exidx_end = .;

   } > FLASH

   __etext = .;

   .data : AT (__etext)
   {
      __data_start__ = .;
      *(.data*)
      __data_end__ = .;

   } > RAM

   .bss :
   {
      __bss_start__ = .;
      *(.bss*)
      __bss_end__ = .;

   } > RAM
}
