#------------------------------------------------------------------------------
# Copyright (c) 2020 John D. Haughton
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#------------------------------------------------------------------------------

.cpu cortex-m4
.fpu vfp

.section .vectors
.align	2

.global vector_table

vector_table:
   .word  0x20020000        @ stack pointer
   .word  VEC_reset+1
   .word  VEC_nmi+1
   .word  VEC_fault+1
   .word  VEC_memMan+1
   .word  VEC_busFault+1
   .word  VEC_usageFault+1
   .word  0
   .word  0
   .word  0
   .word  0
   .word  VEC_svc+1
   .word  VEC_dbg+1
   .word  0
   .word  VEC_pendSv+1
   .word  VEC_sysTick+1

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
   .word  Timer_3_IRQ+1     @ IRQ 26
   .word  Timer_4_IRQ+1     @ IRQ 27
   .word  PWM0_IRQ+1        @ IRQ 28
   .word  PDM_IRQ+1         @ IRQ 29
   .word  0
   .word  0
   .word  MWU_IRQ+1         @ IRQ 32
   .word  PWM1_IRQ+1        @ IRQ 33
   .word  PWM2_IRQ+1        @ IRQ 34
   .word  SPI2_IRQ+1        @ IRQ 35
   .word  RTC2_IRQ+1        @ IRQ 36
   .word  I2S_IRQ+1         @ IRQ 37
   .word  FPU_IRQ+1         @ IRQ 38
   .word  USBD_IRQ+1        @ IRQ 39
   .word  UARTE1_IRQ+1      @ IRQ 40
   .word  0
   .word  0
   .word  0
   .word  0
   .word  PWM3_IRQ+1        @ IRQ 45
   .word  0
   .word  SPIM3_IRQ+1       @ IRQ 47

   .weak  VEC_nmi
   .weak  VEC_fault
   .weak  VEC_memMan
   .weak  VEC_busFault
   .weak  VEC_usageFault
   .weak  VEC_svc
   .weak  VEC_dbg
   .weak  VEC_pendSv
   .weak  VEC_sysTick

   .weak  PowerClock_IRQ
   .weak  Radio_IRQ
   .weak  Uart_IRQ
   .weak  UartE_IRQ
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
   .weak  Timer_3_IRQ
   .weak  Timer_4_IRQ
   .weak  PWM0_IRQ
   .weak  PDM_IRQ
   .weak  MWU_IRQ
   .weak  PWM1_IRQ
   .weak  PWM2_IRQ
   .weak  SPI2_IRQ
   .weak  RTC2_IRQ
   .weak  I2S_IRQ
   .weak  FPU_IRQ
   .weak  USBD_IRQ
   .weak  UARTE1_IRQ
   .weak  PWM3_IRQ
   .weak  SPIM3_IRQ

.text
.align	2

VEC_reset:
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
VEC_memMan:
VEC_busFault:
VEC_usageFault:
VEC_svc:
VEC_dbg:
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
Timer_3_IRQ:
Timer_4_IRQ:
Rtc_0_IRQ:
Rtc_1_IRQ:
Rtc_2_IRQ:
Temp_IRQ:
Rng_IRQ:
Ecb_IRQ:
CcmAar_IRQ:
Wdt_IRQ:
Qdec_IRQ:
LpComp_IRQ:
Swi_0_IRQ:
Swi_1_IRQ:
Swi_2_IRQ:
Swi_3_IRQ:
Swi_4_IRQ:
Swi_5_IRQ:
Timer_3_IRQ:
Timer_4_IRQ:
PWM0_IRQ:
PDM_IRQ:
MWU_IRQ:
PWM1_IRQ:
PWM2_IRQ:
SPI2_IRQ:
RTC2_IRQ:
I2S_IRQ:
FPU_IRQ:
USBD_IRQ:
UARTE1_IRQ:
PWM3_IRQ:
SPIM3_IRQ:
loop:
    b       loop
