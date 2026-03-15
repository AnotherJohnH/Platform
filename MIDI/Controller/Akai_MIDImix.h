//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

#include "MIDI/Const.h"

namespace Akai {

namespace MIDImix {

inline constexpr unsigned TOP1 = 16;
inline constexpr unsigned MID1 = 17;
inline constexpr unsigned BTM1 = 18;
inline constexpr unsigned LVL1 = 19;

inline constexpr unsigned TOP2 = 20;
inline constexpr unsigned MID2 = 21;
inline constexpr unsigned BTM2 = 22;
inline constexpr unsigned LVL2 = 23;

inline constexpr unsigned TOP3 = 24;
inline constexpr unsigned MID3 = 25;
inline constexpr unsigned BTM3 = 26;
inline constexpr unsigned LVL3 = 27;

inline constexpr unsigned TOP4 = 28;
inline constexpr unsigned MID4 = 29;
inline constexpr unsigned BTM4 = 30;
inline constexpr unsigned LVL4 = 31;

inline constexpr unsigned TOP5 = 46;
inline constexpr unsigned MID5 = 47;
inline constexpr unsigned BTM5 = 48;
inline constexpr unsigned LVL5 = 49;

inline constexpr unsigned TOP6 = 50;
inline constexpr unsigned MID6 = 51;
inline constexpr unsigned BTM6 = 52;
inline constexpr unsigned LVL6 = 53;

inline constexpr unsigned TOP7 = 54;
inline constexpr unsigned MID7 = 55;
inline constexpr unsigned BTM7 = 56;
inline constexpr unsigned LVL7 = 57;

inline constexpr unsigned TOP8 = 58;
inline constexpr unsigned MID8 = 59;
inline constexpr unsigned BTM8 = 60;
inline constexpr unsigned LVL8 = 61;

inline constexpr unsigned MASTER  = 62;

extern const uint8_t map[38];

} // MIDImix

} // Akai
