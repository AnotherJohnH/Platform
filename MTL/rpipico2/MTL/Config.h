//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Record of hardware pin allocations

#pragma once

#include "MTL/MTL.h"
#include "MTL/ConfigBase.h"

#define CLOCK_FREQ     150000000u //!<  System clock 150 MHz
#define RAM_SIZE       (512*1024) //!<  RAM          512 KiB
#define FLASH_SIZE  (4*1024*1024) //!<  FLASH          4 MiB

namespace MTL {

class Config : public ConfigBase</* PINS */ 40>
{
public:
   Config();

protected:
   unsigned gpioToPin(unsigned gpio_) override;
};

extern Config config;

}
