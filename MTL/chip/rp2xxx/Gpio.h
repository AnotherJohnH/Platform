//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief RP2040/RP2350 General Purpose I/O

#pragma once

#include "MTL/Periph.h"

#include "IoBank.h"
#include "PadsBank.h"
#include "Sio.h"

namespace MTL {

namespace Gpio {


template <unsigned WIDTH, unsigned PIN>
class Out
{
public:
   Out()
   {
      PadsBank pads_bank;
      IoBank   io_bank;

      sio.reg->gpio_oe_clr  = MASK << PIN;
      sio.reg->gpio_out_clr = MASK << PIN;

      for(unsigned i = 0; i < WIDTH; ++i)
      {
         io_bank.setFunc( PIN + i, IoBank::SIO);

         pads_bank.setOut(PIN + i, PadsBank::DRIVE_4MA);
      }

      sio.reg->gpio_oe_set = MASK << PIN;
   }

   operator uint32_t() const
   {
      return (sio.reg->gpio_out >> PIN) & MASK;
   }

   uint32_t operator=(uint32_t data)
   {
      sio.reg->gpio_out = (sio.reg->gpio_out & ~(MASK << PIN)) | (data << PIN);
      return data;
   }

   void set(uint32_t data)
   {
      sio.reg->gpio_out_set = data << PIN;
   }

   void clr(uint32_t data)
   {
      sio.reg->gpio_out_clr = data << PIN;
   }

private:
   static const uint32_t MASK = (1 << WIDTH) - 1;

   Sio sio;
};


template <unsigned WIDTH,
          unsigned PIN,
          uint8_t  PULL            = PadsBank::PULL_UP,
          bool     SCHMITT_TRIGGER = false>
class In
{
public:
   In()
   {
      PadsBank pads_bank;
      IoBank   io_bank;

      sio.reg->gpio_oe_clr = MASK << PIN;

      for(unsigned i = 0; i < WIDTH; ++i)
      {
         io_bank.setFunc(PIN + i, IoBank::SIO);

         pads_bank.setIn(PIN + i, PULL, SCHMITT_TRIGGER);
      }
   }

   operator uint32_t() const
   {
      return (sio.reg->gpio_in >> PIN) & MASK;
   }

private:
   static const uint32_t MASK = (1 << WIDTH) - 1;

   Sio sio;
};



template <unsigned WIDTH,
          unsigned PIN,
          uint8_t  PULL            = PadsBank::PULL_DOWN,
          bool     SCHMITT_TRIGGER = false>
class InOut
{
public:
   InOut()
   {
      PadsBank pads_bank;
      IoBank   io_bank;

      sio.reg->gpio_oe_clr = MASK << PIN;

      for(unsigned i = 0; i < WIDTH; ++i)
      {
         io_bank.setFunc(PIN + i, IoBank::SIO);

         pads_bank.setInOut(PIN + i,
                            PadsBank::DRIVE_2MA,
                            /* fast_slew */ false,
                            PULL,
                            SCHMITT_TRIGGER);
      }
   }

   operator uint32_t() const
   {
      return (sio.reg->gpio_in >> PIN) & MASK;
   }

   uint32_t operator=(uint32_t data)
   {
      sio.reg->gpio_out = (sio.reg->gpio_out & ~(MASK << PIN)) | (data << PIN);
      return data;
   }

   void set(uint32_t data)
   {
      sio.reg->gpio_out_set = data << PIN;
   }

   void clr(uint32_t data)
   {
      sio.reg->gpio_out_clr = data << PIN;
   }

   void setOut() { sio.reg->gpio_oe_set = MASK << PIN; }
   void setIn()  { sio.reg->gpio_oe_clr = MASK << PIN; }
   void setHiZ() { setIn(); }

private:
   static const uint32_t MASK = (1 << WIDTH) - 1;

   Sio sio;
};

} // namespace Gpio

} // namespace MTL
