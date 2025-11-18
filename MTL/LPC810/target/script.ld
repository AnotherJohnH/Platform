/*------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//----------------------------------------------------------------------------*/

MEMORY
{
   FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 32K
   RAM   (rwx) : ORIGIN = 0x10000000, LENGTH = 1K
}

ENTRY(vector_table)

SECTIONS
{
   .text :
   {
      KEEP(*(.vectors))
      *(.text*)
      __init_array_start = ALIGN(4);
      KEEP(*(.init_array))
      __init_array_end = .;
      *(.rodata*)

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
