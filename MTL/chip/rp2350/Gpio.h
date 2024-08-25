//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

// \brief RP2350 General Purpose I/O

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
         pads_bank.setOut(PIN + i, PadsBank::DRIVE_4MA);
         io_bank.setFunc( PIN + i, IoBank::SIO);
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
         pads_bank.setIn(PIN + i, PULL, SCHMITT_TRIGGER);
         io_bank.setFunc(PIN + i, IoBank::SIO);
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

} // namespace Gpio

} // namespace MTL
