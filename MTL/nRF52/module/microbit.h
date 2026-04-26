//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief micro::bit module pins

#pragma once

#include "MTL/nRF52/Pins.h"

namespace MTL {

// TODO shouldn't these be in the nRF52 header?
static const unsigned  PIN_UART_TX    = nRF52::PIN_1_8;
static const unsigned  PIN_UART_RX    = nRF52::PIN_0_6;
static const unsigned  PIN_SPI_MOSI   = nRF52::PIN_0_13;
static const unsigned  PIN_SPI_MISO   = nRF52::PIN_0_1;
static const unsigned  PIN_SPI_SCK    = nRF52::PIN_0_17;
static const unsigned  PIN_I2C_SCL    = nRF52::PIN_0_26;
static const unsigned  PIN_I2C_SDA    = nRF52::PIN_1_0;


static const unsigned  PIN_LED_COL1   = nRF52::PIN_0_28;
static const unsigned  PIN_LED_COL2   = nRF52::PIN_0_11;
static const unsigned  PIN_LED_COL3   = nRF52::PIN_0_31;
static const unsigned  PIN_LED_COL4   = nRF52::PIN_1_5;
static const unsigned  PIN_LED_COL5   = nRF52::PIN_0_30;

static const unsigned  PIN_LED_ROW1   = nRF52::PIN_0_21;
static const unsigned  PIN_LED_ROW2   = nRF52::PIN_0_22;
static const unsigned  PIN_LED_ROW3   = nRF52::PIN_0_15;
static const unsigned  PIN_LED_ROW4   = nRF52::PIN_0_24;
static const unsigned  PIN_LED_ROW5   = nRF52::PIN_0_19;

static const unsigned  PIN_BTN_A      = nRF52::PIN_0_14;
static const unsigned  PIN_BTN_B      = nRF52::PIN_0_23;

static const unsigned  PIN_PAD_0      = nRF52::PIN_0_2;
static const unsigned  PIN_PAD_1      = nRF52::PIN_0_3;
static const unsigned  PIN_PAD_2      = nRF52::PIN_0_4;
static const unsigned  PIN_PAD_3      = nRF52::PIN_0_31;
static const unsigned  PIN_PAD_4      = nRF52::PIN_0_28;
static const unsigned  PIN_PAD_5      = nRF52::PIN_0_14;
static const unsigned  PIN_PAD_6      = nRF52::PIN_1_5;
static const unsigned  PIN_PAD_7      = nRF52::PIN_0_11;
static const unsigned  PIN_PAD_8      = nRF52::PIN_0_10;
static const unsigned  PIN_PAD_9      = nRF52::PIN_0_9;
static const unsigned  PIN_PAD_10     = nRF52::PIN_0_30;
static const unsigned  PIN_PAD_11     = nRF52::PIN_0_23;
static const unsigned  PIN_PAD_12     = nRF52::PIN_0_12;
static const unsigned  PIN_PAD_13     = nRF52::PIN_0_17;
static const unsigned  PIN_PAD_14     = nRF52::PIN_0_1;
static const unsigned  PIN_PAD_15     = nRF52::PIN_0_13;
static const unsigned  PIN_PAD_16     = nRF52::PIN_1_2;
static const unsigned  PIN_PAD_19     = nRF52::PIN_0_26;
static const unsigned  PIN_PAD_20     = nRF52::PIN_1_0;

static const unsigned  PIN_LED1 = PIN_LED_COL1;

static const unsigned  PIN_NOT_CONNECTED = 0xFFFFFFFF;

} // namespace MTL
