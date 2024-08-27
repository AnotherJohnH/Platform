//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// RP2040 IRQ numbers

#pragma once

namespace MTL {

enum Irq
{
   IRQ_TIMER_0     =  0,
   IRQ_TIMER_1     =  1,
   IRQ_TIMER_2     =  2,
   IRQ_TIMER_3     =  3,
   IRQ_PWM_WRAP    =  4,
   IRQ_USBCTRL     =  5,
   IRQ_XIP         =  6,
   IRQ_PIO0_0      =  7,
   IRQ_PIO0_1      =  8,
   IRQ_PIO1_0      =  9,
   IRQ_PIO1_1      = 10,
   IRQ_DMA_0       = 11,
   IRQ_DMA_1       = 12,
   IRQ_IO_BANK0    = 13,
   IRQ_IO_QSPI     = 14,
   IRQ_SIO_PROC0   = 15,
   IRQ_SIO_PROC1   = 16,
   IRQ_CLOCKS      = 17,
   IRQ_SPI0        = 18,
   IRQ_SPI1        = 19,
   IRQ_UART0       = 20,
   IRQ_UART1       = 21,
   IRQ_ADC_FIFO    = 22,
   IRQ_I2C0        = 23,
   IRQ_I2C1        = 24,
   IRQ_RTC         = 25
};

} // namespace MTL
