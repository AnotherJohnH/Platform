//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "MIDI/Const.h"

namespace Akai {

namespace MPKmini {

inline constexpr unsigned BEND_VERT = 1;

inline constexpr unsigned K1 = 2;
inline constexpr unsigned K2 = 3;
inline constexpr unsigned K3 = 4;
inline constexpr unsigned K4 = 5;
inline constexpr unsigned K5 = 6;
inline constexpr unsigned K6 = 7;
inline constexpr unsigned K7 = 8;
inline constexpr unsigned K8 = 9;

extern const uint8_t map[10];

} // MPKmini

} // Akai
