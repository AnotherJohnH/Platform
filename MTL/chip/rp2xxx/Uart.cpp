//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief RP2040 UART peripheral

#include "Uart.h"

#include "MTL/Digital.h"

template <>
MTL::UartFifo MTL::UartBuffers<0>::rx_buffer {};

template <>
MTL::UartFifo MTL::UartBuffers<0>::tx_buffer {};

template <>
MTL::UartFifo MTL::UartBuffers<1>::rx_buffer {};

template <>
MTL::UartFifo MTL::UartBuffers<1>::tx_buffer {};

static MTL::Uart0 uart0 {};
static MTL::Uart0 uart1 {};

extern "C" void IRQ_UART0() { uart0.irq(); }
extern "C" void IRQ_UART1() { uart1.irq(); }
