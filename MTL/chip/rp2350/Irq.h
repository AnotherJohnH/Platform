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

// rp2350 IRQ numbers

#pragma once

namespace MTL {

enum Irq
{
   IRQ_TIMER0_0     =  0,
   IRQ_TIMER0_1     =  1,
   IRQ_TIMER0_2     =  2,
   IRQ_TIMER0_3     =  3,
   IRQ_TIMER1_0     =  4,
   IRQ_TIMER1_1     =  5,
   IRQ_TIMER1_2     =  6,
   IRQ_TIMER1_3     =  7,
   IRQ_PWM_WRAP_0   =  8,
   IRQ_PWM_WRAP_1   =  9,
   IRQ_DMA_0        = 10,
   IRQ_DMA_1        = 11,
   IRQ_DMA_2        = 12,
   IRQ_DMA_3        = 13,
   IRQ_USBCTRL      = 14,
   IRQ_PIO0_0       = 15,
   IRQ_PIO0_1       = 16,
   IRQ_PIO1_0       = 17,
   IRQ_PIO1_1       = 18,
   IRQ_PIO2_0       = 19,
   IRQ_PIO2_1       = 20,
   IRQ_IO_BANK0     = 21,
   IRQ_IO_BANK0_NS  = 22,
   IRQ_IO_QSPI      = 23,
   IRQ_IO_QSPI_NS   = 24,
   IRQ_SIO_FIFO     = 25,
   IRQ_SIO_BELL     = 26,
   IRQ_SIO_FIFO_NS  = 27,
   IRQ_SIO_BELL_NS  = 28,
   IRQ_SIO_MTIMECMP = 29,
   IRQ_CLOCKS       = 30,
   IRQ_SPI0         = 31,
   IRQ_SPI1         = 32,
   IRQ_UART0        = 33,
   IRQ_UART1        = 34,
   IRQ_ADC_FIFO     = 35,
   IRQ_I2C0         = 36,
   IRQ_I2C1         = 37,
   IRQ_OTP          = 38,
   IRQ_TRNG         = 39,
   IRQ_PROC0_CTI    = 40,
   IRQ_PROC1_CTI    = 41,
   IRQ_PLL_SYS      = 42,
   IRQ_PLL_USB      = 43,
   IRQ_POWMAN_POW   = 44,
   IRQ_POWMAN_TIMER = 45
};

} // namespace MTL
