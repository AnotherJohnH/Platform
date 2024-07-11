@------------------------------------------------------------------------------
@ Copyright (c) 2023 John D. Haughton
@
@ Permission is hereby granted, free of charge, to any person obtaining a copy
@ of this software and associated documentation files (the "Software"), to deal
@ in the Software without restriction, including without limitation the rights
@ to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
@ copies of the Software, and to permit persons to whom the Software is
@ furnished to do so, subject to the following conditions:
@
@ The above copyright notice and this permission notice shall be included in
@ all copies or substantial portions of the Software.
@
@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
@ IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
@ FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
@ AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
@ LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
@ OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
@ SOFTWARE.
@------------------------------------------------------------------------------

.cpu cortex-m0
.fpu softvfp

.section .vectors
.align 8

.global vector_table_core0

vector_table_core0:
   .word  0x20042000        @ stack pointer
   .word  VEC_reset+1
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

.weak VEC_fault
.weak VEC_nmi
.weak VEC_svc
.weak VEC_pendSv
.weak VEC_sysTick

.weak IRQ_TIMER_0
.weak IRQ_TIMER_1
.weak IRQ_TIMER_2
.weak IRQ_TIMER_3
.weak IRQ_PWM_WRAP
.weak IRQ_USBCTRL
.weak IRQ_XIP
.weak IRQ_PIO0_0
.weak IRQ_PIO0_1
.weak IRQ_PIO1_0
.weak IRQ_PIO1_1
.weak IRQ_DMA_0
.weak IRQ_DMA_1
.weak IRQ_IO_BANK0
.weak IRQ_IO_QSPI
.weak IRQ_SIO_PROC0
.weak IRQ_SIO_PROC1
.weak IRQ_CLOCKS
.weak IRQ_SPI0
.weak IRQ_SPI1
.weak IRQ_UART0
.weak IRQ_UART1
.weak IRQ_ADC_FIFO
.weak IRQ_I2C0
.weak IRQ_I2C1
.weak IRQ_RTC

.text
.align 2

VEC_reset:
#
# Prepare image to run
#
    bl   MTL_data_and_bss
#
# Initialise platform
# XXX Must not use global constructors
#     as not initialised yet
#
    bl   MTL_init
#
# Construct global objects
#
    bl   MTL_global_construction
#
# Call application entry point
#
    bl   MTL_main
#
# Fall through to unhandled exception
#
VEC_fault:
    bl   MTL_halt

# Empty handlers
VEC_nmi:
VEC_svc:
VEC_pendSv:
VEC_sysTick:
IRQ_TIMER_0:
IRQ_TIMER_1:
IRQ_TIMER_2:
IRQ_TIMER_3:
IRQ_PWM_WRAP:
IRQ_USBCTRL:
IRQ_XIP:
IRQ_PIO0_0:
IRQ_PIO0_1:
IRQ_PIO1_0:
IRQ_PIO1_1:
IRQ_DMA_0:
IRQ_DMA_1:
IRQ_IO_BANK0:
IRQ_IO_QSPI:
IRQ_SIO_PROC0:
IRQ_SIO_PROC1:
IRQ_CLOCKS:
IRQ_SPI0:
IRQ_SPI1:
IRQ_UART0:
IRQ_UART1:
IRQ_ADC_FIFO:
IRQ_I2C0:
IRQ_I2C1:
IRQ_RTC:
    bx   lr
