/*------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//----------------------------------------------------------------------------*/

MEMORY
{
   FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 8K
   RAM   (rwx) : ORIGIN = 0x10000000, LENGTH = 512
}

ENTRY(vector_table)

SECTIONS
{
   .text :
   {
      KEEP(*(.vectors))
      *(.text*)
      __ctors_start = .;
      KEEP(*(.init_array))
      __ctors_end = .;
      *(.rodata*)

   } > FLASH

   __etext = .;

   .data : AT (__etext)
   {
      __data_load_start = .;
      __data_start = .;
      *(.data*)
      __data_end = .;

   } > RAM

   .bss :
   {
      __bss_start = .;
      *(.bss*)
      __bss_end = .;

   } > RAM
}
