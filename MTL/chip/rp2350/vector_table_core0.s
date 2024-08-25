#------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#------------------------------------------------------------------------------

.cpu cortex-m33
.fpu softvfp

.section .vectors
.align 8

.global vector_table_core0

vector_table_core0:
   .word  0x20082000        @ stack pointer
   .word  VEC_reset+1
   .word  VEC_nmi+1
   .word  VEC_fault+1
   .word  invalid_excep+1
   .word  invalid_excep+1
   .word  invalid_excep+1
   .word  invalid_excep+1
   .word  invalid_excep+1
   .word  invalid_excep+1
   .word  invalid_excep+1
   .word  VEC_svc+1
   .word  invalid_excep+1
   .word  invalid_excep+1
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

.weak VEC_fault
.weak VEC_nmi
.weak VEC_svc
.weak VEC_pendSv
.weak VEC_sysTick

.weak IRQ_TIMER0_0
.weak IRQ_TIMER0_1
.weak IRQ_TIMER0_2
.weak IRQ_TIMER0_3
.weak IRQ_TIMER1_0
.weak IRQ_TIMER1_1
.weak IRQ_TIMER1_2
.weak IRQ_TIMER1_3
.weak IRQ_PWM_WRAP_0
.weak IRQ_PWM_WRAP_1
.weak IRQ_DMA_0
.weak IRQ_DMA_1
.weak IRQ_DMA_2
.weak IRQ_DMA_3
.weak IRQ_USBCTRL
.weak IRQ_PIO0_0
.weak IRQ_PIO0_1
.weak IRQ_PIO1_0
.weak IRQ_PIO1_1
.weak IRQ_PIO2_0
.weak IRQ_PIO2_1
.weak IRQ_IO_BANK0
.weak IRQ_IO_BANK0_NS
.weak IRQ_IO_QSPI
.weak IRQ_IO_QSPI_NS
.weak IRQ_SIO_FIFO
.weak IRQ_SIO_BELL
.weak IRQ_SIO_FIFO_NS
.weak IRQ_SIO_BELL_NS
.weak IRQ_SIO_MTIMECMP
.weak IRQ_CLOCKS
.weak IRQ_SPI0
.weak IRQ_SPI1
.weak IRQ_UART0
.weak IRQ_UART1
.weak IRQ_ADC_FIFO
.weak IRQ_I2C0
.weak IRQ_I2C1
.weak IRQ_OTP
.weak IRQ_TRNG
.weak IRQ_PROC0_CTI
.weak IRQ_PROC1_CTI
.weak IRQ_PLL_SYS
.weak IRQ_PLL_USB
.weak IRQ_POWMAN_POW
.weak IRQ_POWMAN_TIMER

invalid_excep:
   bkpt 0

#-------------------------------------------------------------------------------

.align 2
IMAGE_DEF:
   .word 0xffffded3
   .word 0x10210142
   .word 0x000001ff
   .word 0x00000000
   .word 0xab123579

#-------------------------------------------------------------------------------

VEC_reset:
#
# Check for CPU-1
#
    ldr   r0, =0xd0000000
    ldr   r0,[r0,#0]
    cbz   r0,start_cpu0
#
# Set the boot ROM VTABLE
    mov     r0, #0
    ldr     r1, =0xe000ed08
    str     r0, [r1, #0]
#
# Enter the boot ROM via the reset vector
    ldmia   r0, {r0, r1}
    msr     MSP, r0
    bx      r1

start_cpu0:
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
IRQ_TIMER0_0:
IRQ_TIMER0_1:
IRQ_TIMER0_2:
IRQ_TIMER0_3:
IRQ_TIMER1_0:
IRQ_TIMER1_1:
IRQ_TIMER1_2:
IRQ_TIMER1_3:
IRQ_PWM_WRAP_0:
IRQ_PWM_WRAP_1:
IRQ_DMA_0:
IRQ_DMA_1:
IRQ_DMA_2:
IRQ_DMA_3:
IRQ_USBCTRL:
IRQ_PIO0_0:
IRQ_PIO0_1:
IRQ_PIO1_0:
IRQ_PIO1_1:
IRQ_PIO2_0:
IRQ_PIO2_1:
IRQ_IO_BANK0:
IRQ_IO_BANK0_NS:
IRQ_IO_QSPI:
IRQ_IO_QSPI_NS:
IRQ_SIO_FIFO:
IRQ_SIO_BELL:
IRQ_SIO_FIFO_NS:
IRQ_SIO_BELL_NS:
IRQ_SIO_MTIMECMP:
IRQ_CLOCKS:
IRQ_SPI0:
IRQ_SPI1:
IRQ_UART0:
IRQ_UART1:
IRQ_ADC_FIFO:
IRQ_I2C0:
IRQ_I2C1:
IRQ_OTP:
IRQ_TRNG:
IRQ_PROC0_CTI:
IRQ_PROC1_CTI:
IRQ_PLL_SYS:
IRQ_PLL_USB:
IRQ_POWMAN_POW:
IRQ_POWMAN_TIMER:
    bx   lr

