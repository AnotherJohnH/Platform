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
   .word  0x10000400        @ stack pointer (4k RAM)
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

   .word  IRQ_SPI0+1       @ IRQ 0
   .word  IRQ_SPI1+1       @ IRQ 1
   .word  0                @ IRQ 2
   .word  IRQ_UART0+1      @ IRQ 3
   .word  IRQ_UART1+1      @ IRQ 4
   .word  IRQ_UART2+1      @ IRQ 5
   .word  0                @ IRQ 6
   .word  0                @ IRQ 7
   .word  IRQ_I2C0+1       @ IRQ 8
   .word  IRQ_SCT+1        @ IRQ 9
   .word  IRQ_MRT+1        @ IRQ 10
   .word  IRQ_CMP+1        @ IRQ 11
   .word  IRQ_WDT+1        @ IRQ 12
   .word  IRQ_BOD+1        @ IRQ 13
   .word  0                @ IRQ 14
   .word  IRQ_WKT+1        @ IRQ 15
   .word  0                @ IRQ 16
   .word  0                @ IRQ 17
   .word  0                @ IRQ 18
   .word  0                @ IRQ 19
   .word  0                @ IRQ 20
   .word  0                @ IRQ 21
   .word  0                @ IRQ 22
   .word  0                @ IRQ 23
   .word  IRQ_PININT0+1    @ IRQ 24
   .word  IRQ_PININT1+1    @ IRQ 25
   .word  IRQ_PININT2+1    @ IRQ 26
   .word  IRQ_PININT3+1    @ IRQ 27
   .word  IRQ_PININT4+1    @ IRQ 28
   .word  IRQ_PININT5+1    @ IRQ 29
   .word  IRQ_PININT6+1    @ IRQ 30
   .word  IRQ_PININT7+1    @ IRQ 31

   .weak IRQ_SPI0
   .weak IRQ_SPI1
   .weak IRQ_UART0
   .weak IRQ_UART1
   .weak IRQ_UART2
   .weak IRQ_I2C0
   .weak IRQ_SCT
   .weak IRQ_MRT
   .weak IRQ_CMP
   .weak IRQ_WDT
   .weak IRQ_BOD
   .weak IRQ_WKT
   .weak IRQ_PININT0
   .weak IRQ_PININT1
   .weak IRQ_PININT2
   .weak IRQ_PININT3
   .weak IRQ_PININT4
   .weak IRQ_PININT5
   .weak IRQ_PININT6
   .weak IRQ_PININT7

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
IRQ_SPI0:
IRQ_SPI1:
IRQ_UART0:
IRQ_UART1:
IRQ_UART2:
IRQ_I2C0:
IRQ_SCT:
IRQ_MRT:
IRQ_CMP:
IRQ_WDT:
IRQ_BOD:
IRQ_WKT:
IRQ_PININT0:
IRQ_PININT1:
IRQ_PININT2:
IRQ_PININT3:
IRQ_PININT4:
IRQ_PININT5:
IRQ_PININT6:
IRQ_PININT7:
    bx   lr
