//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"
#include "module/microbit.h"
#include "Uart.h"

static MTL::nRF51::Uart<MTL::PIN_UART_RX,MTL::PIN_UART_TX> uart{MTL::nRF51::UART_BAUD_115200};

void MTL_putch(uint8_t ch)
{
   if (ch == '\n')
      uart.tx('\r');
   uart.tx(ch);
}

int MTL_getch()
{
   return uart.rx();
}

void MTL_nobuff()
{
}
