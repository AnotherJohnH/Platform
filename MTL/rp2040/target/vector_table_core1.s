#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

.cpu cortex-m0
.fpu softvfp

.section .vectors
.align 8

.global vector_table_core1

vector_table_core1:
   .word  0x20040000        @ stack pointer
   .word  VEC1_reset+1
   .word  VEC1_nmi+1
   .word  VEC1_fault+1
   .word  0
   .word  0
   .word  0
   .word  0
   .word  0
   .word  0
   .word  0
   .word  VEC1_svc+1
   .word  0
   .word  0
   .word  VEC1_pendSv+1
   .word  VEC1_sysTick+1

   .word  IRQ1_TIMER_0+1    @ IRQ  0
   .word  IRQ1_TIMER_1+1    @ IRQ  1
   .word  IRQ1_TIMER_2+1    @ IRQ  2
   .word  IRQ1_TIMER_3+1    @ IRQ  3
   .word  IRQ1_PWM_WRAP+1   @ IRQ  4
   .word  IRQ1_USBCTRL+1    @ IRQ  5
   .word  IRQ1_XIP+1        @ IRQ  6
   .word  IRQ1_PIO0_0+1     @ IRQ  7
   .word  IRQ1_PIO0_1+1     @ IRQ  8
   .word  IRQ1_PIO1_0+1     @ IRQ  9
   .word  IRQ1_PIO1_1+1     @ IRQ 10
   .word  IRQ1_DMA_0+1      @ IRQ 11
   .word  IRQ1_DMA_1+1      @ IRQ 12
   .word  IRQ1_IO_BANK0+1   @ IRQ 13
   .word  IRQ1_IO_QSPI+1    @ IRQ 14
   .word  IRQ1_SIO_PROC0+1  @ IRQ 15
   .word  IRQ1_SIO_PROC1+1  @ IRQ 16
   .word  IRQ1_CLOCKS+1     @ IRQ 17
   .word  IRQ1_SPI0+1       @ IRQ 18
   .word  IRQ1_SPI1+1       @ IRQ 19
   .word  IRQ1_UART0+1      @ IRQ 20
   .word  IRQ1_UART1+1      @ IRQ 21
   .word  IRQ1_ADC_FIFO+1   @ IRQ 22
   .word  IRQ1_I2C0+1       @ IRQ 23
   .word  IRQ1_I2C1+1       @ IRQ 24
   .word  IRQ1_RTC+1        @ IRQ 25
   .word  0                 @ IRQ 26
   .word  0                 @ IRQ 27
   .word  0                 @ IRQ 28
   .word  0                 @ IRQ 29
   .word  0                 @ IRQ 30
   .word  0                 @ IRQ 31

.weak VEC1_fault
.weak VEC1_nmi
.weak VEC1_svc
.weak VEC1_pendSv
.weak VEC1_sysTick

.weak IRQ1_TIMER_0
.weak IRQ1_TIMER_1
.weak IRQ1_TIMER_2
.weak IRQ1_TIMER_3
.weak IRQ1_PWM_WRAP
.weak IRQ1_USBCTRL
.weak IRQ1_XIP
.weak IRQ1_PIO0_0
.weak IRQ1_PIO0_1
.weak IRQ1_PIO1_0
.weak IRQ1_PIO1_1
.weak IRQ1_DMA_0
.weak IRQ1_DMA_1
.weak IRQ1_IO_BANK0
.weak IRQ1_IO_QSPI
.weak IRQ1_SIO_PROC0
.weak IRQ1_SIO_PROC1
.weak IRQ1_CLOCKS
.weak IRQ1_SPI0
.weak IRQ1_SPI1
.weak IRQ1_UART0
.weak IRQ1_UART1
.weak IRQ1_ADC_FIFO
.weak IRQ1_I2C0
.weak IRQ1_I2C1
.weak IRQ1_RTC

.text
.align 2

VEC1_reset:
VEC1_fault:
VEC1_nmi:
VEC1_svc:
VEC1_pendSv:
VEC1_sysTick:
IRQ1_TIMER_0:
IRQ1_TIMER_1:
IRQ1_TIMER_2:
IRQ1_TIMER_3:
IRQ1_PWM_WRAP:
IRQ1_USBCTRL:
IRQ1_XIP:
IRQ1_PIO0_0:
IRQ1_PIO0_1:
IRQ1_PIO1_0:
IRQ1_PIO1_1:
IRQ1_DMA_0:
IRQ1_DMA_1:
IRQ1_IO_BANK0:
IRQ1_IO_QSPI:
IRQ1_SIO_PROC0:
IRQ1_SIO_PROC1:
IRQ1_CLOCKS:
IRQ1_SPI0:
IRQ1_SPI1:
IRQ1_UART0:
IRQ1_UART1:
IRQ1_ADC_FIFO:
IRQ1_I2C0:
IRQ1_I2C1:
IRQ1_RTC:
loop:
    b   loop
