//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "STB/Vector2.h"

namespace GUI {

using Vector    = STB::Vector2<int32_t>;
using Position  = Vector;
using Size      = STB::Vector2<uint32_t>;
using SmallSize = STB::Vector2<uint8_t>;

} // namespace GUI
