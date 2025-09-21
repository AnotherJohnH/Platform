//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Record of hardware pin allocations

#include <cstdint>

#include "MTL/Config.h"

MTL::Config::Config()
{
   MTL::Config& self = *this;

   pin(1, "(Debug) TX UART");

   for(unsigned p = 3; p < NUM_PINS; p += 5)
   {
      pin(p, "gnd");
   }

   pin(30, "run");
   pin(33, "agnd");
   pin(35, "adc-vref");
   pin(36, "3v3");
   pin(37, "3v3-en");
   pin(39, "vsys");
   pin(40, "vbus +5v");
}

unsigned MTL::Config::gpioToPin(unsigned gpio_)
{
   static const uint8_t map[32] = 
   {
       1,  2,  4,  5,  6,  7,  9, 10, 11, 12, 14, 15, 16, 17, 19, 20,
      21, 22, 24, 25, 26, 27, 29,  0,  0,  0, 31, 32, 34,  0,  0,  0
   };

   return gpio_ < 32 ? map[gpio_] : 0;
}

MTL::Config MTL::config{};
