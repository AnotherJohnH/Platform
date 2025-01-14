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
   .word  0x20004000        @ stack pointer (16k RAM)
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

   .word  PowerClock_IRQ+1  @ IRQ  0
   .word  Radio_IRQ+1       @ IRQ  1
   .word  Uart_IRQ+1        @ IRQ  2
   .word  SpiTwi_0_IRQ+1    @ IRQ  3
   .word  SpiTwi_1_IRQ+1    @ IRQ  4
   .word  irq5+1            @ IRQ  5
   .word  GpioTE_IRQ+1      @ IRQ  6
   .word  Adc_IRQ+1         @ IRQ  7
   .word  Timer_0_IRQ+1     @ IRQ  8
   .word  Timer_1_IRQ+1     @ IRQ  9
   .word  Timer_2_IRQ+1     @ IRQ 10
   .word  Rtc_0_IRQ+1       @ IRQ 11
   .word  Temp_IRQ+1        @ IRQ 12
   .word  Rng_IRQ+1         @ IRQ 13
   .word  Ecb_IRQ+1         @ IRQ 14
   .word  CcmAar_IRQ+1      @ IRQ 15
   .word  Wdt_IRQ+1         @ IRQ 16
   .word  Rtc_1_IRQ+1       @ IRQ 17
   .word  Qdec_IRQ+1        @ IRQ 18
   .word  LpComp_IRQ+1      @ IRQ 19
   .word  Swi_0_IRQ+1       @ IRQ 20
   .word  Swi_1_IRQ+1       @ IRQ 21
   .word  Swi_2_IRQ+1       @ IRQ 22
   .word  Swi_3_IRQ+1       @ IRQ 23
   .word  Swi_4_IRQ+1       @ IRQ 24
   .word  Swi_5_IRQ+1       @ IRQ 25

   .weak  PowerClock_IRQ
   .weak  Radio_IRQ
   .weak  Uart_IRQ
   .weak  SpiTwi_0_IRQ
   .weak  SpiTwi_1_IRQ
   .weak  irq5
   .weak  GpioTE_IRQ
   .weak  Adc_IRQ
   .weak  Timer_0_IRQ
   .weak  Timer_1_IRQ
   .weak  Timer_2_IRQ
   .weak  Rtc_0_IRQ
   .weak  Temp_IRQ
   .weak  Rng_IRQ
   .weak  Ecb_IRQ
   .weak  CcmAar_IRQ
   .weak  Wdt_IRQ
   .weak  Rtc_1_IRQ
   .weak  Qdec_IRQ
   .weak  LpComp_IRQ
   .weak  Swi_0_IRQ
   .weak  Swi_1_IRQ
   .weak  Swi_2_IRQ
   .weak  Swi_3_IRQ
   .weak  Swi_4_IRQ
   .weak  Swi_5_IRQ

.text
.align 2

VEC_reset:
#
# Make sure all RAM banks are powered on
#
    ldr     r0,ramon
    movs    r1,#3
    str     r1,[r0]
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
PowerClock_IRQ:
Radio_IRQ:
Uart_IRQ:
SpiTwi_0_IRQ:
SpiTwi_1_IRQ:
irq5:
GpioTE_IRQ:
Adc_IRQ:
Timer_0_IRQ:
Timer_1_IRQ:
Timer_2_IRQ:
Rtc_0_IRQ:
Temp_IRQ:
Rng_IRQ:
Ecb_IRQ:
CcmAar_IRQ:
Wdt_IRQ:
Rtc_1_IRQ:
Qdec_IRQ:
LpComp_IRQ:
Swi_0_IRQ:
Swi_1_IRQ:
Swi_2_IRQ:
Swi_3_IRQ:
Swi_4_IRQ:
Swi_5_IRQ:
    bx   lr

.align 2
ramon:
   .word 0x40000524
