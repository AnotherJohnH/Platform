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
   .word  0x20040000        @ stack pointer (256k RAM)
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

   .word  irq0+1           @ IRQ  0  PIO0_0
   .word  irq1+1           @ IRQ  1  PIO0_1
   .word  irq2+1           @ IRQ  2  PIO0_2
   .word  irq3+1           @ IRQ  3  PIO0_3
   .word  irq4+1           @ IRQ  4  PIO0_4
   .word  irq5+1           @ IRQ  5  PIO0_5
   .word  irq6+1           @ IRQ  6  PIO0_6
   .word  irq7+1           @ IRQ  7  PIO0_7
   .word  irq8+1           @ IRQ  8  PIO0_8
   .word  irq9+1           @ IRQ  9  PIO0_9
   .word  irq10+1          @ IRQ 10  PIO0_10
   .word  irq11+1          @ IRQ 11  PIO0_11
   .word  irq12+1          @ IRQ 12  PIO1_0
   .word  irq13+1          @ IRQ 13  C_CAN
   .word  irq14+1          @ IRQ 14  SSP/SPI 1
   .word  irq15+1          @ IRQ 15  I2C
   .word  Timer16_0_IRQ+1  @ IRQ 16  CT16B0
   .word  Timer16_1_IRQ+1  @ IRQ 17  CT16B1
   .word  irq18+1          @ IRQ 18
   .word  irq19+1          @ IRQ 19
   .word  Spi_0_IRQ+1      @ IRQ 20  SSP/SSP 0
   .word  irq21+1          @ IRQ 21
   .word  irq22+1          @ IRQ 22
   .word  irq23+1          @ IRQ 23
   .word  irq24+1          @ IRQ 24
   .word  irq25+1          @ IRQ 25
   .word  irq26+1          @ IRQ 26
   .word  irq27+1          @ IRQ 27
   .word  irq28+1          @ IRQ 28
   .word  irq29+1          @ IRQ 29
   .word  irq30+1          @ IRQ 30
   .word  irq31+1          @ IRQ 31

   .weak  VEC_nmi
   .weak  VEC_fault
   .weak  VEC_svc
   .weak  VEC_pendSv
   .weak  VEC_sysTick

   .weak irq0
   .weak irq1
   .weak irq2
   .weak irq3
   .weak irq4
   .weak irq5
   .weak irq6
   .weak irq7
   .weak irq8
   .weak irq9
   .weak irq10
   .weak irq11
   .weak irq12
   .weak irq13
   .weak irq14
   .weak irq15
   .weak Timer16_0_IRQ
   .weak Timer16_1_IRQ
   .weak irq18
   .weak irq19
   .weak Spi_0_IRQ
   .weak irq21
   .weak irq22
   .weak irq23
   .weak irq24
   .weak irq25
   .weak irq26
   .weak irq27
   .weak irq28
   .weak irq29
   .weak irq30
   .weak irq31

.text
.align 2

VEC_reset:
#
    bl      platform_init
#
# Initialise C/C++ runtime
#
    bl      tinyc_init
#
# Call application entry point
#
    bl      mtlMain
#
# Fall through to unhandled exception
#
unhandled_exception:

VEC_nmi:
VEC_fault:
VEC_svc:
VEC_pendSv:
VEC_sysTick:

loop:
    b       loop

irq0:
irq1:
irq2:
irq3:
irq4:
irq5:
irq6:
irq7:
irq8:
irq9:
irq10:
irq11:
irq12:
irq13:
irq14:
irq15:
Timer16_0_IRQ:
Timer16_1_IRQ:
irq18:
irq19:
Spi_0_IRQ:
irq21:
irq22:
irq23:
irq24:
irq25:
irq26:
irq27:
irq28:
irq29:
irq30:
irq31:
    mrs     r0,ipsr
    sub     r0,#16
    b       platform_irq
