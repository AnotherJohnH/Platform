;------------------------------------------------------------------------------
; Copyright (c) 2013 John D. Haughton
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
; SOFTWARE.
;------------------------------------------------------------------------------

.section .vectors
.align  2

.macro vector name
.weak  \name
.set   \name, irq
    rjmp    \ name
.endm

.global vector_table

vector_table:
    rjmp    reset
    vector  __vector_1
    vector  __vector_2
    vector  __vector_3
    vector  __vector_4
    vector  __vector_5
    vector  __vector_6
    vector  __vector_7
    vector  __vector_8
    vector  __vector_9
    vector  __vector_10
    vector  __vector_11
    vector  __vector_12
    vector  __vector_13
    vector  __vector_14

.text
.align  2

reset:
    eor     r1,r1
    out     0x3f,r1

    ldi     r28,0x5f
    out     0x3d,r28
    ldi     r29,0x02
    out     0x3e,r29
#
# Initialise C/C++ runtime
#
    rcall   tinyc_init
#
# Call application entry point
#
    rcall   mtlMain

irq:
loop:
    rjmp    loop
