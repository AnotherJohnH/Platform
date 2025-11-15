//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// \brief Piromoni tufty 2040

#pragma once

#include "MTL/rp2040/Pins.h"
#include "MTL/rp2040/Adc.h"
#include "MTL/rp2040/Pwm.h"
#include "MTL/rp2040/Gpio.h"
#include "MTL/rp2040/I2C.h"

#include "MTL/Lcd_ST7789V.h"

namespace MTL {

namespace tufty2040 {

static const unsigned PIN_SW_DN      = IO_PIN_6;
static const unsigned PIN_SW_A       = IO_PIN_7;
static const unsigned PIN_SW_B       = IO_PIN_8;
static const unsigned PIN_SW_C       = IO_PIN_9;
static const unsigned PIN_SW_UP      = IO_PIN_22;
static const unsigned PIN_SW_USR     = IO_PIN_23;
static const unsigned PIN_USER_LED   = IO_PIN_25;
static const unsigned PIN_SENSOR_PWR = IO_PIN_27;

static const unsigned ADC_CHAN_LUX  = 0;
static const unsigned ADC_CHAN_VREF = 2;
static const unsigned ADC_CHAN_VBAT = 3;

using Lcd = Lcd_ST7789V</* CS */ IO_PIN_10,
                        /* DC */ IO_PIN_11,
                        /* WR */ IO_PIN_12,
                        /* RD */ IO_PIN_13,
                        /* DB */ IO_PIN_14,
                        /* BL */ IO_PIN_2>;

using LedUser    = Gpio::Out<1, PIN_USER_LED>;
using PwrLux     = Gpio::Out<1, PIN_SENSOR_PWR>;
using PwmLedUser = Pwm<PIN_USER_LED>;

using SwitchUp  = Gpio::In<1, PIN_SW_UP,  PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchDn  = Gpio::In<1, PIN_SW_DN,  PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchA   = Gpio::In<1, PIN_SW_A,   PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchB   = Gpio::In<1, PIN_SW_B,   PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchC   = Gpio::In<1, PIN_SW_C,   PadsBank::PULL_DOWN, /* schmitt trigger */ true>;
using SwitchUsr = Gpio::In<1, PIN_SW_USR, PadsBank::PULL_DOWN, /* schmitt trigger */ true>;

using AdcVRef = Adc<ADC_CHAN_VREF>;
using AdcVBat = Adc<ADC_CHAN_VBAT>;

using I2C_QwSt = I2C0_P6_P7;

} // namespace tufty2040

} // namespace MTL
