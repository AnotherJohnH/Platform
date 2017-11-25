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

.cpu cortex-m3
.fpu softvfp

.section .vectors
.align	2

.global vector_table

vector_table:
   .word  0x10008000        @ stack pointer (32k RAM)
   .word  reset+1
   .word  nmi+1
   .word  hard_fault+1
   .word  mem_fault+1
   .word  bus_fault+1
   .word  usage_fault+1
   .word  0
   .word  0
   .word  0
   .word  0
   .word  svc+1
   .word  0
   .word  0
   .word  pendSv+1
   .word  sysTick+1

   .word  irqWDT+1       @ IRQ 0
   .word  irqTimer0+1    @ IRQ 1
   .word  irqTimer1+1    @ IRQ 2
   .word  irqTimer2+1    @ IRQ 3
   .word  irqTimer3+1    @ IRQ 4
   .word  irqUART0+1     @ IRQ 5
   .word  irqUART1+1     @ IRQ 6
   .word  irqUART2+1     @ IRQ 7
   .word  irqUART3+1     @ IRQ 8
   .word  irqPWM1+1      @ IRQ 9
   .word  irqI2C0+1      @ IRQ 10
   .word  irqI2C1+1      @ IRQ 11
   .word  irqI2C2+1      @ IRQ 12
   .word  irqSPI+1       @ IRQ 13
   .word  irqSSP0+1      @ IRQ 14
   .word  irqSSP1+1      @ IRQ 15
   .word  irqPLL0+1      @ IRQ 16
   .word  irqRTC+1       @ IRQ 17
   .word  irqExt0+1      @ IRQ 18
   .word  irqExt1+1      @ IRQ 19
   .word  irqExt2+1      @ IRQ 20
   .word  irqExt3+1      @ IRQ 21
   .word  irqADC+1       @ IRQ 22
   .word  irqBOD+1       @ IRQ 23
   .word  irqUSB+1       @ IRQ 24
   .word  irqCAN+1       @ IRQ 25
   .word  irqGPDMA+1     @ IRQ 26
   .word  irqI2S+1       @ IRQ 27
   .word  irqEth+1       @ IRQ 28
   .word  irqRepTimer+1  @ IRQ 29
   .word  irqMCPWM+1     @ IRQ 30
   .word  irqQuadEnc+1   @ IRQ 31
   .word  irqPLL1+1      @ IRQ 32
   .word  irqUSBAct+1    @ IRQ 33
   .word  irqCANAct+1    @ IRQ 34

   .weak  reset
   .weak  nmi
   .weak  hard_fault
   .weak  mem_fault
   .weak  bus_fault
   .weak  usage_fault
   .weak  svc
   .weak  pendSv
   .weak  sysTick

   .weak  irqWDT
   .weak  irqTimer0
   .weak  irqTimer1
   .weak  irqTimer2
   .weak  irqTimer3
   .weak  irqUART0
   .weak  irqUART1
   .weak  irqUART2
   .weak  irqUART3
   .weak  irqPWM1
   .weak  irqI2C0
   .weak  irqI2C1
   .weak  irqI2C2
   .weak  irqSPI
   .weak  irqSSP0
   .weak  irqSSP1
   .weak  irqPLL0
   .weak  irqRTC
   .weak  irqExt0
   .weak  irqExt1
   .weak  irqExt2
   .weak  irqExt3
   .weak  irqADC
   .weak  irqBOD
   .weak  irqUSB
   .weak  irqCAN
   .weak  irqGPDMA
   .weak  irqI2S
   .weak  irqEth
   .weak  irqRepTimer
   .weak  irqMCPWM
   .weak  irqQuadEnc
   .weak  irqPLL1
   .weak  irqUSBAct
   .weak  irqCANAct

.text
.align	2

reset:
#
    bl      platform_init 
#
# Initialise C/C++ runtime
#
    bl      tinyc_init
#
# Call application entry point
#
    mov     r0, #0   @ ensure argc is zero
    bl      main
    bl      exit

#
# unhandled exceptions
#
unhandled_exception:
nmi:
hard_fault:
mem_fault:
bus_fault:
usage_fault:
svc:
pendSv:
sysTick:
irqWDT:
irqTimer0:
irqTimer1:
irqTimer2:
irqTimer3:
irqUART0:
irqUART1:
irqUART2:
irqUART3:
irqPWM1:
irqI2C0:
irqI2C1:
irqI2C2:
irqSPI:
irqSSP0:
irqSSP1:
irqPLL0:
irqRTC:
irqExt0:
irqExt1:
irqExt2:
irqExt3:
irqADC:
irqBOD:
irqUSB:
irqCAN:
irqGPDMA:
irqI2S:
irqEth:
irqRepTimer:
irqMCPWM:
irqQuadEnc:
irqPLL1:
irqUSBAct:
irqCANAct:
    ldr     r0,=platform_fault
    bx      r0
