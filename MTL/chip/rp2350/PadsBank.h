//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2350 PADS_BANK0 peripheral

#pragma once

#include "MTL/Periph.h"

namespace MTL {

struct PadsBankReg
{
   uint32_t voltage_select;
   uint32_t gpio[48];
   uint32_t swclk;
   uint32_t swd;
};

class PadsBank : public Periph<PadsBankReg, 0x40038000>
{
public:
    static const uint8_t OD         = 0b010000000; //!< Output disable
    static const uint8_t IE         = 0b001000000; //!< Input enable
    static const uint8_t DRIVE_2MA  = 0;
    static const uint8_t DRIVE_4MA  = 0b000010000;
    static const uint8_t DRIVE_8MA  = 0b000100000;
    static const uint8_t DRIVE_12MA = 0b000110000;
    static const uint8_t PULL_NONE  = 0;
    static const uint8_t PULL_UP    = 0b000001000;
    static const uint8_t PULL_DOWN  = 0b000000100;
    static const uint8_t SCHMITT    = 0b000000010; //!< Enable schmitt trigger
    static const uint8_t SLEWFAST   = 0b000000001; //!< Slew rate control

    //! Set I/O pin as a digital output
    void setOut(unsigned io_pin,
                uint8_t  drive,
                bool     slew_fast = false)
    {
       uint8_t bits = IE | drive;

       if (slew_fast) bits |= SLEWFAST;

       set(io_pin, bits);
    }

    //! Set I/O pin as a digital input
    void setIn(unsigned io_pin,
               uint8_t  pull = PULL_UP,
               bool     schmitt_trigger = false)
    {
       uint8_t bits = OD | IE | pull;

       if (schmitt_trigger) bits |= SCHMITT;

       set(io_pin, bits);
    }

    //! Set I/O pin as a digital input and output
    void setInOut(unsigned io_pin,
                  uint8_t  drive,
                  bool     slew_fast = false,
                  uint8_t  pull = PULL_NONE,
                  bool     schmitt_trigger = false)
    {
       uint8_t bits = IE | drive | pull;

       if (slew_fast)       bits |= SLEWFAST;
       if (schmitt_trigger) bits |= SCHMITT;

       set(io_pin, bits);
    }

    //! Set I/O pin as an analogue input
    void setAnalogIn(unsigned io_pin)
    {
       set(io_pin, OD);
    }

    //! Configure the pad
    void set(unsigned io_pin, uint8_t bits)
    {
       reg->gpio[io_pin] = (reg->gpio[io_pin] & 0xFFFFFF00) | bits;
    }

    void isolate(unsigned io_pin)
    {
       reg->gpio[io_pin] |= ISO;
    }

    void unisolate(unsigned io_pin)
    {
       reg->gpio[io_pin] &= ~ISO;
    }

private:
    static const uint32_t ISO = 1 << 8; //!< Isolate
};

} // namespace MTL
