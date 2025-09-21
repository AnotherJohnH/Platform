//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "MTL/MTL.h"
#include "MTL/ConfigBase.h"

#define CLOCK_FREQ     133000000u //!<  System clock 133 MHz
#define RAM_SIZE       (256*1024) //!<  RAM          256 KiB
#define FLASH_SIZE  (2*1024*1024) //!<  FLASH          2 MiB

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
