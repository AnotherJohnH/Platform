//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040/RP2350 UART peripheral

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

static MTL::Uart0_P1_P2 uart0 {};
static MTL::Uart1_P6_P7 uart1 {};

extern "C" void IRQ_UART0() { uart0.irq(); }
extern "C" void IRQ_UART1() { uart1.irq(); }
