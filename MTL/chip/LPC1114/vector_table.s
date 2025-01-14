@------------------------------------------------------------------------------
@ Copyright (c) 2013 John D. Haughton
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
.align	2

.global vector_table

vector_table:
   .word  0x10001000        @ stack pointer (4k RAM)
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

   .weak  VEC_fault
   .weak  VEC_nmi
   .weak  VEC_svc
   .weak  VEC_pendSv
   .weak  VEC_sysTick

   .word  IRQ_PIO0_0+1     @ IRQ 0
   .word  IRQ_PIO0_1+1     @ IRQ 1
   .word  IRQ_PIO0_2+1     @ IRQ 2
   .word  IRQ_PIO0_3+1     @ IRQ 3
   .word  IRQ_PIO0_4+1     @ IRQ 4
   .word  IRQ_PIO0_5+1     @ IRQ 5
   .word  IRQ_PIO0_6+1     @ IRQ 6
   .word  IRQ_PIO0_7+1     @ IRQ 7
   .word  IRQ_PIO0_8+1     @ IRQ 8
   .word  IRQ_PIO0_9+1     @ IRQ 9
   .word  IRQ_PIO0_10+1    @ IRQ 10
   .word  IRQ_PIO0_11+1    @ IRQ 11
   .word  IRQ_PIO1_0+1     @ IRQ 12
   .word  IRQ_C_CAN        @ IRQ 13
   .word  IRQ_SSP1+1       @ IRQ 14
   .word  IRQ_I2C+1        @ IRQ 15
   .word  IRQ_CT16B0+1     @ IRQ 16
   .word  IRQ_CT16B1+1     @ IRQ 17
   .word  IRQ_CT32B0+1     @ IRQ 18
   .word  IRQ_CT32B1+1     @ IRQ 19
   .word  IRQ_SSP0+1       @ IRQ 20
   .word  IRQ_UART+1       @ IRQ 21
   .word  0                @ IRQ 22
   .word  0                @ IRQ 23
   .word  IRQ_ADC+1        @ IRQ 24
   .word  IRQ_WDT+1        @ IRQ 25
   .word  IRQ_BOD+1        @ IRQ 26
   .word  0                @ IRQ 27
   .word  IRQ_PIO_3+1      @ IRQ 28
   .word  IRQ_PIO_2+1      @ IRQ 29
   .word  IRQ_PIO_1+1      @ IRQ 30
   .word  IRQ_PIO_0+1      @ IRQ 31

   .weak IRQ_PIO0_0
   .weak IRQ_PIO0_1
   .weak IRQ_PIO0_2
   .weak IRQ_PIO0_3
   .weak IRQ_PIO0_4
   .weak IRQ_PIO0_5
   .weak IRQ_PIO0_6
   .weak IRQ_PIO0_7
   .weak IRQ_PIO0_9
   .weak IRQ_PIO0_10
   .weak IRQ_PIO0_11
   .weak IRQ_PIO1_0
   .weak IRQ_C_CAN
   .weak IRQ_SSP1
   .weak IRQ_I2C
   .weak IRQ_CT16B0
   .weak IRQ_CT16B1
   .weak IRQ_CT32B0
   .weak IRQ_CT32B1
   .weak IRQ_SSP0
   .weak IRQ_UART
   .weak IRQ_ADC
   .weak IRQ_WDT
   .weak IRQ_BOD
   .weak IRQ_PIO_3
   .weak IRQ_PIO_2
   .weak IRQ_PIO_1
   .weak IRQ_PIO_0

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
IRQ_PIO0_0:
IRQ_PIO0_1:
IRQ_PIO0_2:
IRQ_PIO0_3:
IRQ_PIO0_4:
IRQ_PIO0_5:
IRQ_PIO0_6:
IRQ_PIO0_7:
IRQ_PIO0_8:
IRQ_PIO0_9:
IRQ_PIO0_10:
IRQ_PIO0_11:
IRQ_PIO1_0:
IRQ_SSP1:
IRQ_I2C:
IRQ_CT16B0:
IRQ_CT16B1:
IRQ_CT32B0:
IRQ_CT32B1:
IRQ_SSP0:
IRQ_UART:
IRQ_ADC:
IRQ_BOD:
IRQ_WDT:
IRQ_PIO_3:
IRQ_PIO_2:
IRQ_PIO_1:
IRQ_PIO_0:
    bx   lr
