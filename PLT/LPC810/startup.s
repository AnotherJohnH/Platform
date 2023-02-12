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
   .word  0x10000400        @ stack pointer (1k RAM)
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

   .word  0           @ IRQ 0
   .word  0           @ IRQ 1
   .word  0           @ IRQ 2
   .word  0           @ IRQ 3
   .word  0           @ IRQ 4
   .word  0           @ IRQ 5
   .word  0           @ IRQ 6
   .word  0           @ IRQ 7
   .word  0           @ IRQ 8
   .word  0           @ IRQ 9
   .word  0           @ IRQ 10
   .word  0           @ IRQ 11
   .word  0           @ IRQ 12
   .word  0           @ IRQ 13
   .word  0           @ IRQ 14
   .word  0           @ IRQ 15

   .weak  VEC_nmi
   .weak  VEC_fault
   .weak  VEC_svc
   .weak  VEC_pendSv
   .weak  VEC_sysTick

.text
.align 2

VEC_reset:
#
    bl      MTL_init
#
# Initialise C/C++ runtime
#
    bl      TNY_init
#
# Call application entry point
#
    bl      MTL_main
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
