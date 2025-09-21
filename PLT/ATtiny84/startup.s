#-------------------------------------------------------------------------------
# ; Copyright (c) 2013 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

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
    rcall   TNY_init
#
# Call application entry point
#
    rcall   MTL_main

irq:
loop:
    rjmp    loop
