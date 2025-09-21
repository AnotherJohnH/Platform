//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief NXP LPC11U24 Additional RAM

#pragma once

#include <cstddef>

namespace MTL {

class RAM
{
public:
   static uint8_t* data() { return (uint8_t*)0x20004000; }
   static size_t   size() { return 2 * 1024; }
};

} // namespace MTL
