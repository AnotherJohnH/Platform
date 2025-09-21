//-------------------------------------------------------------------------------
// Copyright (c) 2014 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "MTL/MTL.h"

#include "Clocks.h"
#include "Resets.h"

//! Default system clock freqency 132 MHz
//  declared weak so that applications may override
namespace MTL {
Clocks::SysFreq __attribute__((weak)) clocks_sys_freq = Clocks::SYS_FREQ_132_00_MHZ;
}

void MTL_init()
{
   // Resets for essential peripherals
   const uint32_t ESSENTIAL_RESETS = MTL::Resets::IO_QSPI   |
                                     MTL::Resets::PADS_QSPI |
                                     MTL::Resets::PLL_USB   |
                                     MTL::Resets::USBCTRL   |
                                     MTL::Resets::SYSCFG    |
                                     MTL::Resets::PLL_SYS;

   const uint32_t CLOCKED_RESETS = MTL::Resets::USBCTRL |
                                   MTL::Resets::UART1   |
                                   MTL::Resets::UART0   |
                                   MTL::Resets::SPI1    |
                                   MTL::Resets::SPI0    |
                                   MTL::Resets::RTC     |
                                   MTL::Resets::ADC;

   MTL::Resets resets;

   // reset everything except essentials
   resets.setReset(MTL::Resets::ALL & ~ESSENTIAL_RESETS);

   // Clear reset for all except some clocked peropherals
   resets.clrReset(MTL::Resets::ALL & ~CLOCKED_RESETS);

   // Start XOSC, PLLs and all the clock generators
   MTL::Clocks().start();

   // Clear resets for everything
   resets.clrReset(MTL::Resets::ALL);
}
