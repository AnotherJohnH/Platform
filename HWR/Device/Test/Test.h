//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdio>

#define NOINLINE __attribute__((noinline))

namespace HWR {

enum TestPhase { DECL, INFO, START, RUN };

}
