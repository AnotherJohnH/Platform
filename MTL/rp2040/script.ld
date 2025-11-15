/*------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//----------------------------------------------------------------------------*/

MEMORY
{
   FLASH (rx)  : ORIGIN = 0x10000000, LENGTH = 2M
   RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
}

ENTRY(boot2)

SECTIONS
{
   .text :
   {
      __text_start__ = .;
      KEEP(*(.boot2))
      KEEP(*(.vectors))
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
