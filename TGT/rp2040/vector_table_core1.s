#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

.syntax unified
.thumb

#===============================================================================
# Vector table for core-1 when manually started

.section .vectors.core1
.align 8

.global vector_table_core1
vector_table_core1:
   .word  0x20040000        @ stack pointer
   .word  0                 @ reset will go to main vector table
   .word  VEC_nmi+1
   .word  VEC_fault+1
   .word  0
   .word  0
   .word  0
   .word  0
   .word  0
   .word  0
   .word  0
   .word  VEC_svc+1
   .word  0
   .word  0
   .word  VEC_pendSv+1
   .word  VEC_sysTick+1

   .word  IRQ_TIMER_0+1    @ IRQ  0
   .word  IRQ_TIMER_1+1    @ IRQ  1
   .word  IRQ_TIMER_2+1    @ IRQ  2
   .word  IRQ_TIMER_3+1    @ IRQ  3
   .word  IRQ_PWM_WRAP+1   @ IRQ  4
   .word  IRQ_USBCTRL+1    @ IRQ  5
   .word  IRQ_XIP+1        @ IRQ  6
   .word  IRQ_PIO0_0+1     @ IRQ  7
   .word  IRQ_PIO0_1+1     @ IRQ  8
   .word  IRQ_PIO1_0+1     @ IRQ  9
   .word  IRQ_PIO1_1+1     @ IRQ 10
   .word  IRQ_DMA_0+1      @ IRQ 11
   .word  IRQ_DMA_1+1      @ IRQ 12
   .word  IRQ_IO_BANK0+1   @ IRQ 13
   .word  IRQ_IO_QSPI+1    @ IRQ 14
   .word  IRQ_SIO_PROC0+1  @ IRQ 15
   .word  IRQ_SIO_PROC1+1  @ IRQ 16
   .word  IRQ_CLOCKS+1     @ IRQ 17
   .word  IRQ_SPI0+1       @ IRQ 18
   .word  IRQ_SPI1+1       @ IRQ 19
   .word  IRQ_UART0+1      @ IRQ 20
   .word  IRQ_UART1+1      @ IRQ 21
   .word  IRQ_ADC_FIFO+1   @ IRQ 22
   .word  IRQ_I2C0+1       @ IRQ 23
   .word  IRQ_I2C1+1       @ IRQ 24
   .word  IRQ_RTC+1        @ IRQ 25
   .word  0                @ IRQ 26
   .word  0                @ IRQ 27
   .word  0                @ IRQ 28
   .word  0                @ IRQ 29
   .word  0                @ IRQ 30
   .word  0                @ IRQ 31
