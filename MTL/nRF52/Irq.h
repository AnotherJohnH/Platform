//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// nRF52

#pragma once

namespace MTL {

enum Irq
{
   IRQ_POWER_CLOCK =  0,
   IRQ_RADIO       =  1,
   IRQ_UART        =  2,
   IRQ_SPI_TWI_0   =  3,
   IRQ_SPI_TWI_1   =  4,
   IRQ_5           =  5,
   IRQ_GPIO_TE     =  6,
   IRQ_ADC         =  7,
   IRQ_TIMER_0     =  8,
   IRQ_TIMER_1     =  9,
   IRQ_TIMER_2     = 10,
   IRQ_RTC_0       = 11,
   IRQ_TEMP        = 12,
   IRQ_RNG         = 13,
   IRQ_ECB         = 14,
   IRQ_CCM_AAR     = 15,
   IRQ_WDT         = 16,
   IRQ_RTC         = 17,
   IRQ_QDEC        = 18,
   IRQ_LPCOMP      = 19,
   IRQ_SWI_0       = 20,
   IRQ_SWI_1       = 21,
   IRQ_SWI_2       = 22,
   IRQ_SWI_3       = 23,
   IRQ_SWI_4       = 24,
   IRQ_SWI_5       = 25,
   IRQ_TIMER_3     = 26,
   IRQ_TIMER_4     = 27,
   IRQ_PWM_0       = 28,
   IRQ_PDM         = 29,

   IRQ_MWU         = 32,
   IRQ_PWM_1       = 33,
   IRQ_PWM_2       = 34,
   IRQ_SPI_2       = 35,
   IRQ_RTC_2       = 36,
   IRQ_I2S         = 37,
   IRQ_FPU         = 38,
   IRQ_USBD        = 39,
   IRQ_UARTE1      = 40,

   IRQ_PWM_3       = 45,

   IRQ_SPIM3       = 47
};

} // namespace MTL

