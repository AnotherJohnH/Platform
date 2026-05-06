//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Register logging helper

#pragma once

#include <cstdio>

inline void logReg(const char* name, volatile uint32_t& reg)
{
   printf("%08X: %08X  # %s\n", &reg, reg, name);
}
