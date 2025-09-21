//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//
// \brief Piromoni badger 2040

#pragma once

#include "MTL/rp2040/Pins.h"
#include "MTL/rp2040/Adc.h"
#include "MTL/rp2040/Pwm.h"
#include "MTL/rp2040/Gpio.h"
#include "MTL/rp2040/Spi.h"
#include "MTL/rp2040/I2C.h"

#include "MTL/EPaper_UC8151.h"

namespace MTL {

namespace badger2040 {

static const unsigned PIN_ENABLE_3V3 = rp2040::IO_PIN_10;
static const unsigned PIN_SW_DN      = rp2040::IO_PIN_11;
static const unsigned PIN_SW_A       = rp2040::IO_PIN_12;
static const unsigned PIN_SW_B       = rp2040::IO_PIN_13;
static const unsigned PIN_SW_C       = rp2040::IO_PIN_14;
static const unsigned PIN_SW_UP      = rp2040::IO_PIN_15;
static const unsigned PIN_SW_USR     = rp2040::IO_PIN_23;
static const unsigned PIN_USER_LED   = rp2040::IO_PIN_25;
static const unsigned PIN_VREF_PWR   = rp2040::IO_PIN_27;

static const unsigned ADC_CHAN_VREF  = 2;
static const unsigned ADC_CHAN_VBAT  = 3;

static const unsigned VREF_MILLIVOLT = 1240;
static const unsigned VBAT_SCALE     = 3;

using EPaper = EPaper_UC8151</* WIDTH  */ 296,
                             /* HEIGHT */ 128,
                             /* DC  */ rp2040::IO_PIN_20,
                             /* CS  */ rp2040::IO_PIN_17,
                             /* RST */ rp2040::IO_PIN_21,
                             /* BSY */ rp2040::IO_PIN_26,
                             Spi0_ALT2>;

using LedUser    = Gpio::Out<1, PIN_USER_LED>;
using Enable3V3  = Gpio::Out<1, PIN_ENABLE_3V3>;
using EnableVRef = Gpio::Out<1, PIN_VREF_PWR>;
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

} // namespace badger2040

} // namespace MTL
