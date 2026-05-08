#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
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
   .word  0x20080000        @ stack pointer
   .word  0                 @ reset will go to main vector table
   .word  VEC_nmi+1
   .word  VEC_hard_fault+1
   .word  VEC_mem_fault+1
   .word  VEC_bus_fault+1
   .word  VEC_usage_fault+1
   .word  VEC_secure_fault+1
   .word  0
   .word  0
   .word  0
   .word  VEC_svc+1
   .word  VEC_dbg_mon+1
   .word  0
   .word  VEC_pendSv+1
   .word  VEC_sysTick+1

   .word  IRQ_TIMER0_0+1     @ IRQ  0
   .word  IRQ_TIMER0_1+1     @ IRQ  1
   .word  IRQ_TIMER0_2+1     @ IRQ  2
   .word  IRQ_TIMER0_3+1     @ IRQ  3
   .word  IRQ_TIMER1_0+1     @ IRQ  4
   .word  IRQ_TIMER1_1+1     @ IRQ  5
   .word  IRQ_TIMER1_2+1     @ IRQ  6
   .word  IRQ_TIMER1_3+1     @ IRQ  7
   .word  IRQ_PWM_WRAP_0+1   @ IRQ  8
   .word  IRQ_PWM_WRAP_1+1   @ IRQ  9
   .word  IRQ_DMA_0+1        @ IRQ 10
   .word  IRQ_DMA_1+1        @ IRQ 11
   .word  IRQ_DMA_2+1        @ IRQ 12
   .word  IRQ_DMA_3+1        @ IRQ 13
   .word  IRQ_USBCTRL+1      @ IRQ 14
   .word  IRQ_PIO0_0+1       @ IRQ 15
   .word  IRQ_PIO0_1+1       @ IRQ 16
   .word  IRQ_PIO1_0+1       @ IRQ 17
   .word  IRQ_PIO1_1+1       @ IRQ 18
   .word  IRQ_PIO2_0+1       @ IRQ 19
   .word  IRQ_PIO2_1+1       @ IRQ 20
   .word  IRQ_IO_BANK0+1     @ IRQ 21
   .word  IRQ_IO_BANK0_NS+1  @ IRQ 22
   .word  IRQ_IO_QSPI+1      @ IRQ 23
   .word  IRQ_IO_QSPI_NS+1   @ IRQ 24
   .word  IRQ_SIO_FIFO+1     @ IRQ 25
   .word  IRQ_SIO_BELL+1     @ IRQ 26
   .word  IRQ_SIO_FIFO_NS+1  @ IRQ 27
   .word  IRQ_SIO_BELL_NS+1  @ IRQ 28
   .word  IRQ_SIO_MTIMECMP+1 @ IRQ 29
   .word  IRQ_CLOCKS+1       @ IRQ 30
   .word  IRQ_SPI0+1         @ IRQ 31
   .word  IRQ_SPI1+1         @ IRQ 32
   .word  IRQ_UART0+1        @ IRQ 33
   .word  IRQ_UART1+1        @ IRQ 34
   .word  IRQ_ADC_FIFO+1     @ IRQ 35
   .word  IRQ_I2C0+1         @ IRQ 36
   .word  IRQ_I2C1+1         @ IRQ 37
   .word  IRQ_OTP+1          @ IRQ 38
   .word  IRQ_TRNG+1         @ IRQ 39
   .word  IRQ_PROC0_CTI+1    @ IRQ 40
   .word  IRQ_PROC1_CTI+1    @ IRQ 41
   .word  IRQ_PLL_SYS+1      @ IRQ 42
   .word  IRQ_PLL_USB+1      @ IRQ 43
   .word  IRQ_POWMAN_POW+1   @ IRQ 44
   .word  IRQ_POWMAN_TIMER+1 @ IRQ 45
   .word  0                  @ IRQ 46
   .word  0                  @ IRQ 47
   .word  0                  @ IRQ 48
   .word  0                  @ IRQ 49
   .word  0                  @ IRQ 50 
   .word  0                  @ IRQ 51
