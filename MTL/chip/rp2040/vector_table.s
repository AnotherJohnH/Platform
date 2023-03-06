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
.align	2

.global vector_table

vector_table:
   .word  0x20040000        @ stack pointer (25k RAM)
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

   .word  IRQ_GPIO_0+1     @ IRQ 0
   .word  IRQ_GPIO_1+1     @ IRQ 1
   .word  IRQ_GPIO_2+1     @ IRQ 2
   .word  IRQ_GPIO_3+1     @ IRQ 3
   .word  IRQ_GPIO_4+1     @ IRQ 4
   .word  IRQ_GPIO_5+1     @ IRQ 5
   .word  IRQ_GPIO_6+1     @ IRQ 6
   .word  IRQ_GPIO_7+1     @ IRQ 7
   .word  IRQ_GINT0+1      @ IRQ 8
   .word  IRQ_GINT1+1      @ IRQ 9
   .word  0                @ IRQ 10
   .word  0                @ IRQ 11
   .word  0                @ IRQ 12
   .word  0                @ IRQ 13
   .word  IRQ_SSP1+1       @ IRQ 14
   .word  IRQ_I2C+1        @ IRQ 15
   .word  IRQ_CT16B0+1     @ IRQ 16
   .word  IRQ_CT16B1+1     @ IRQ 17
   .word  IRQ_CT32B0+1     @ IRQ 18
   .word  IRQ_CT32B1+1     @ IRQ 19
   .word  IRQ_SSP0+1       @ IRQ 20
   .word  IRQ_USART+1      @ IRQ 21
   .word  IRQ_USB_IRQ+1    @ IRQ 22
   .word  IRQ_USB_FIQ+1    @ IRQ 23
   .word  IRQ_ADC+1        @ IRQ 24
   .word  IRQ_BOD+1        @ IRQ 25
   .word  IRQ_WWDT+1       @ IRQ 26
   .word  IRQ_FLASH+1      @ IRQ 27
   .word  0                @ IRQ 28
   .word  0                @ IRQ 29
   .word  IRQ_USB_WAKEUP+1 @ IRQ 30
   .word  IRQ_IOH+1        @ IRQ 31

   .weak IRQ_GPIO_0
   .weak IRQ_GPIO_1
   .weak IRQ_GPIO_2
   .weak IRQ_GPIO_3
   .weak IRQ_GPIO_4
   .weak IRQ_GPIO_5
   .weak IRQ_GPIO_6
   .weak IRQ_GPIO_7
   .weak IRQ_GINT0
   .weak IRQ_GINT1
   .weak IRQ_SSP1
   .weak IRQ_I2C
   .weak IRQ_CT16B0
   .weak IRQ_CT16B1
   .weak IRQ_CT32B0
   .weak IRQ_CT32B1
   .weak IRQ_SSP0
   .weak IRQ_USART
   .weak IRQ_USB_IRQ
   .weak IRQ_USB_FIQ
   .weak IRQ_ADC
   .weak IRQ_WWDT
   .weak IRQ_BOD
   .weak IRQ_FLASH
   .weak IRQ_USB_WAKEUP
   .weak IRQ_IOH

.text
.align 2

VEC_reset:
#
# Initialise platform
#
    bl   MTL_init
#
# Initialise C/C++ runtime
#
    bl   MTL_load
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
IRQ_GPIO_0:
IRQ_GPIO_1:
IRQ_GPIO_2:
IRQ_GPIO_3:
IRQ_GPIO_4:
IRQ_GPIO_5:
IRQ_GPIO_6:
IRQ_GPIO_7:
IRQ_GINT0:
IRQ_GINT1:
IRQ_SSP1:
IRQ_I2C:
IRQ_CT16B0:
IRQ_CT16B1:
IRQ_CT32B0:
IRQ_CT32B1:
IRQ_SSP0:
IRQ_USART:
IRQ_USB_IRQ:
IRQ_USB_FIQ:
IRQ_ADC:
IRQ_WWDT:
IRQ_BOD:
IRQ_FLASH:
IRQ_USB_WAKEUP:
IRQ_IOH:
    bx   lr
