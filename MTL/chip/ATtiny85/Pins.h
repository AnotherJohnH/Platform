//-------------------------------------------------------------------------------
// Copyright (c) 2020 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \file Pins.h
// \brief ATMEL ATtiny85 pins
//
// Data source Atmel document "doc2586 8-bit AVR Microcontroller ATtiny85"

#ifndef ATTINY85_PINS_H
#define ATTINY85_PINS_H

namespace MTL {

// Pin constants encode [bit:3][reg:8]

static const unsigned  PIN_1 = 0x516; // PB5
static const unsigned  PIN_2 = 0x316; // PB3
static const unsigned  PIN_3 = 0x416; // PB4
static const unsigned  PIN_5 = 0x016; // PB0
static const unsigned  PIN_6 = 0x116; // PB1
static const unsigned  PIN_7 = 0x216; // PB2

static const unsigned  PIN_LED1 = PIN_1;

} // namespace MTL

#endif // ATTINY85_PINS_H
