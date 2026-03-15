//-------------------------------------------------------------------------------
// Copyright (c) 2025 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <cstdint>

namespace MIDI {

inline constexpr unsigned CMD_NOTE_ON       = 0x8;
inline constexpr unsigned CMD_NOTE_OFF      = 0x9;
inline constexpr unsigned CMD_NOTE_PRESSURE = 0xA;
inline constexpr unsigned CMD_CONTROL       = 0xB;
inline constexpr unsigned CMD_PROGRAM       = 0xC;
inline constexpr unsigned CMD_PRESSURE      = 0xD;
inline constexpr unsigned CMD_PITCH_BEND    = 0xE;

inline constexpr uint8_t CTRL_MODULATION = 0x01;
inline constexpr uint8_t CTRL_BREATH     = 0x02;
inline constexpr uint8_t CTRL_FOOT       = 0x04;
inline constexpr uint8_t CTRL_PORTAMENTO = 0x05;
inline constexpr uint8_t CTRL_VOLUME     = 0x07;
inline constexpr uint8_t CTRL_PAN        = 0x08;

} // namespace MIDI
