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
   .word  VEC_reset+1
   .word  VEC_nmi+1
   .word  VEC_hard_fault+1
   .word  VEC_mem_fault+1
   .word  VEC_bus_fault+1
   .word  VEC_usage_fault+1
   .word  0
   .word  0
   .word  0
   .word  0
   .word  VEC_svc+1
   .word  0
   .word  0
   .word  VEC_pendSv+1
   .word  VEC_sysTick+1

   .word  IRQ_WDT+1       @ IRQ 0
   .word  IRQ_Timer0+1    @ IRQ 1
   .word  IRQ_Timer1+1    @ IRQ 2
   .word  IRQ_Timer2+1    @ IRQ 3
   .word  IRQ_Timer3+1    @ IRQ 4
   .word  IRQ_UART0+1     @ IRQ 5
   .word  IRQ_UART1+1     @ IRQ 6
   .word  IRQ_UART2+1     @ IRQ 7
   .word  IRQ_UART3+1     @ IRQ 8
   .word  IRQ_PWM1+1      @ IRQ 9
   .word  IRQ_I2C0+1      @ IRQ 10
   .word  IRQ_I2C1+1      @ IRQ 11
   .word  IRQ_I2C2+1      @ IRQ 12
   .word  IRQ_SPI+1       @ IRQ 13
   .word  IRQ_SSP0+1      @ IRQ 14
   .word  IRQ_SSP1+1      @ IRQ 15
   .word  IRQ_PLL0+1      @ IRQ 16
   .word  IRQ_RTC+1       @ IRQ 17
   .word  IRQ_Ext0+1      @ IRQ 18
   .word  IRQ_Ext1+1      @ IRQ 19
   .word  IRQ_Ext2+1      @ IRQ 20
   .word  IRQ_Ext3+1      @ IRQ 21
   .word  IRQ_ADC+1       @ IRQ 22
   .word  IRQ_BOD+1       @ IRQ 23
   .word  IRQ_USB+1       @ IRQ 24
   .word  IRQ_CAN+1       @ IRQ 25
   .word  IRQ_GPDMA+1     @ IRQ 26
   .word  IRQ_I2S+1       @ IRQ 27
   .word  IRQ_Eth+1       @ IRQ 28
   .word  IRQ_RepTimer+1  @ IRQ 29
   .word  IRQ_MCPWM+1     @ IRQ 30
   .word  IRQ_QuadEnc+1   @ IRQ 31
   .word  IRQ_PLL1+1      @ IRQ 32
   .word  IRQ_USBAct+1    @ IRQ 33
   .word  IRQ_CANAct+1    @ IRQ 34

   .weak  VEC_nmi
   .weak  VEC_hard_fault
   .weak  VEC_mem_fault
   .weak  VEC_svc
   .weak  VEC_pendSv
   .weak  VEC_sysTick

   .weak  IRQ_WDT
   .weak  IRQ_Timer0
   .weak  IRQ_Timer1
   .weak  IRQ_Timer2
   .weak  IRQ_Timer3
   .weak  IRQ_UART0
   .weak  IRQ_UART1
   .weak  IRQ_UART2
   .weak  IRQ_UART3
   .weak  IRQ_PWM1
   .weak  IRQ_I2C0
   .weak  IRQ_I2C1
   .weak  IRQ_I2C2
   .weak  IRQ_SPI
   .weak  IRQ_SSP0
   .weak  IRQ_SSP1
   .weak  IRQ_PLL0
   .weak  IRQ_RTC
   .weak  IRQ_Ext0
   .weak  IRQ_Ext1
   .weak  IRQ_Ext2
   .weak  IRQ_Ext3
   .weak  IRQ_ADC
   .weak  IRQ_BOD
   .weak  IRQ_USB
   .weak  IRQ_CAN
   .weak  IRQ_GPDMA
   .weak  IRQ_I2S
   .weak  IRQ_Eth
   .weak  IRQ_RepTimer
   .weak  IRQ_MCPWM
   .weak  IRQ_QuadEnc
   .weak  IRQ_PLL1
   .weak  IRQ_USBAct
   .weak  IRQ_CANAct

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
VEC_hard_fault:
VEC_mem_fault:
    bl   MTL_halt

# Empty handlers
VEC_nmi:
VEC_svc:
VEC_pendSv:
VEC_sysTick:

IRQ_WDT:
IRQ_Timer0:
IRQ_Timer1:
IRQ_Timer2:
IRQ_Timer3:
IRQ_UART0:
IRQ_UART1:
IRQ_UART2:
IRQ_UART3:
IRQ_PWM1:
IRQ_I2C0:
IRQ_I2C1:
IRQ_I2C2:
IRQ_SPI:
IRQ_SSP0:
IRQ_SSP1:
IRQ_PLL0:
IRQ_RTC:
IRQ_Ext0:
IRQ_Ext1:
IRQ_Ext2:
IRQ_Ext3:
IRQ_ADC:
IRQ_BOD:
IRQ_USB:
IRQ_CAN:
IRQ_GPDMA:
IRQ_I2S:
IRQ_Eth:
IRQ_RepTimer:
IRQ_MCPWM:
IRQ_QuadEnc:
IRQ_PLL1:
IRQ_USBAct:
IRQ_CANAct:
    bx   lr
