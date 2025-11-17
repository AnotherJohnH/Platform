//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC1768 Additional RAM
//
// Data source NXP document "LPC17xx User Manual UM10360"

#pragma once

#include <cstddef>

namespace MTL {

class RAM
{
public:
   static uint8_t* data() { return (uint8_t*)0x2007C000; }
   static size_t   size() { return 32 * 1024; }
};

} // namespace MTL
