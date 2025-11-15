//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040 PADS_BANK0 peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct PadsBankReg
{
   uint32_t voltage_select;
   uint32_t gpio[30];
   uint32_t swclk;
   uint32_t swd;
};

class PadsBank : public Periph<PadsBankReg, 0x4001C000>
{
public:
    static const uint8_t OD         = 0b10000000; //!< Output disable
    static const uint8_t IE         = 0b01000000; //!< Input enable
    static const uint8_t DRIVE_2MA  = 0;
    static const uint8_t DRIVE_4MA  = 0b000010000;
    static const uint8_t DRIVE_8MA  = 0b000100000;
    static const uint8_t DRIVE_12MA = 0b000110000;
    static const uint8_t PULL_NONE  = 0;
    static const uint8_t PULL_UP    = 0b000001000;
    static const uint8_t PULL_DOWN  = 0b000000100;
    static const uint8_t SCHMITT    = 0b00000010; //!< Enable schmitt trigger
    static const uint8_t SLEWFAST   = 0b00000001; //!< Slew rate control

    //! Set I/O pin as a digital output
    void setOut(unsigned io_pin,
                uint8_t  drive,
                bool     slew_fast = false)
    {
       uint32_t bits = IE | drive;

       if (slew_fast) bits |= SLEWFAST;

       reg->gpio[io_pin] = bits;
    }

    //! Set I/O pin as a digital input
    void setIn(unsigned io_pin,
               uint8_t  pull = PULL_UP,
               bool     schmitt_trigger = false)
    {
       uint32_t bits = OD | IE | pull;

       if (schmitt_trigger) bits |= SCHMITT;

       reg->gpio[io_pin] = bits;
    }

    //! Set I/O pin as a digital input and output
    void setInOut(unsigned io_pin,
                  uint8_t  drive,
                  bool     slew_fast = false,
                  uint8_t  pull = PULL_UP,
                  bool     schmitt_trigger = false)
    {
       uint32_t bits = IE | drive | pull;

       if (slew_fast)       bits |= SLEWFAST;
       if (schmitt_trigger) bits |= SCHMITT;

       reg->gpio[io_pin] = bits;
    }

    //! Set I/O pin as an analogue input
    void setAnalogIn(unsigned io_pin)
    {
       reg->gpio[io_pin] = OD;
    }

    //! Configure the pad
    void set(unsigned io_pin, uint8_t bits)
    {
       reg->gpio[io_pin] = (reg->gpio[io_pin] & 0xFFFFFF00) | bits;
    }
};

} // namespace MTL
