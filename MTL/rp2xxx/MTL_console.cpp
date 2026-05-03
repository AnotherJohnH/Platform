//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "MTL/rp2xxx/Uart.h"

namespace MTL {
//  declared weak so that applications may override
extern const unsigned __attribute__((weak)) console_baud = 115200;
}

static MTL::Uart0_P1_P2 uart{MTL::console_baud, 8, MTL::UART::NONE, 1};

void __attribute__((weak)) MTL_putch(uint8_t ch)
{
   uart.tx(ch);
}

int MTL_getch()
{
   return uart.rx();
}

bool MTL_getch_empty()
{
   return uart.empty();
}

void MTL_nobuff()
{
   uart.disableTxBuffer();
}
