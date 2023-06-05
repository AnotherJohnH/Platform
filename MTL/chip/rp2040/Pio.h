//------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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

// \brief RP2040 PIO peripheral

#pragma once

#include "MTL/Periph.h"

#include "PioAsm.h"

#include "PadsBank.h"
#include "IoBank.h"

namespace MTL {

struct PioReg
{
   uint32_t ctrl;
   uint32_t fstat;
   uint32_t fdebug;
   uint32_t flevel;
   uint32_t txf[4];
   uint32_t rxf[4];
   uint32_t irq;
   uint32_t irq_force;
   uint32_t input_sync_bypass;
   uint32_t dbg_padout;
   uint32_t dbg_padoe;
   uint32_t dbg_cfginfo;
   uint32_t instr_mem[32];

   struct StateMachine
   {
      uint32_t clkdiv;
      uint32_t execctrl;
      uint32_t shiftctrl;
      uint32_t addr;
      uint32_t instr;
      uint32_t pinctrl;
   };

   StateMachine sm[4];
   uint32_t     intr;
   uint32_t     irq0_inte;
   uint32_t     irq0_intf;
   uint32_t     irq0_ints;
   uint32_t     irq1_inte;
   uint32_t     irq1_intf;
   uint32_t     irq1_ints;
};

struct Label
{
    bool     resolved{false};
    uint32_t addr{0};
    uint32_t unresolved{0};
};

enum ShiftDir { SHIFT_RIGHT = 0, SHIFT_LEFT = 1 };

enum Auto     { MANUAL = 0, AUTO_PULL = 1, AUTO_PUSH = 1 };

template <unsigned INDEX, uint32_t BASE_ADDRESS>
class PioBase : public Periph<PioReg, BASE_ADDRESS>
{
public:
   void program(PIO::Asm& code)
   {
      for(unsigned pc = 0; pc < code.size(); pc++)
      {
         this->reg->instr_mem[pc] = code[pc];
      }
   }

   //! Set state machine OUT pins
   void SM_PinOUT(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      this->setField(this->reg->sm[sd].pinctrl,  4,  0, pin);
      this->setField(this->reg->sm[sd].pinctrl, 25, 20, n);
   }

   //! Set state machine SET pins
   void SM_PinSET(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      this->setField(this->reg->sm[sd].pinctrl,  9,  5, pin);
      this->setField(this->reg->sm[sd].pinctrl, 28, 26, n);
   }

   //! Set state machine side set pins
   void SM_PinSDE(unsigned sd, unsigned pin, unsigned n = 1)
   {
      configOut(pin, n);

      this->setField(this->reg->sm[sd].pinctrl, 14, 10, pin);
      this->setField(this->reg->sm[sd].pinctrl, 31, 29, n);
   }

   //! Set state machine INP pin
   void SM_PinINP(unsigned sd, unsigned pin)
   {
      configIn(pin, 1);

      this->setField(this->reg->sm[sd].pinctrl, 19, 15, pin);
   }

   //! Set state machine clock divider
   void SM_ClkDiv(unsigned sd, uint32_t clkdiv8)
   {
      this->reg->sm[sd].clkdiv = clkdiv8 << 8;
   }

   //! Set state machine wrap points
   void SM_wrap(unsigned sd, unsigned btm, unsigned top)
   {
      this->setField(this->reg->sm[sd].execctrl, 16, 7, ((top - 1) << 5) | btm);
   }

   //! Configure output shift register
   void SM_configOSR(unsigned sd, unsigned bits, ShiftDir dir, Auto autopull)
   {
      bits &= 0x1F;

      this->reg->sm[sd].shiftctrl = (   1     << 30) |    // Combine TX + RX FIFO
                                    (bits     << 25) |
                                    (dir      << 19) |
                                    (autopull << 17);
   }

   //! Configure input shift register
   void SM_configISR(unsigned sd, unsigned bits, ShiftDir dir, Auto autopush)
   {
      bits &= 0x1F;

      this->reg->sm[sd].shiftctrl = (   1     << 31) |   // Combine RX & TX FIFO
                                    (bits     << 20) |
                                    (dir      << 18) |
                                    (autopush << 16);
   }

   //! Write to TX FIFO
   void SM_write(unsigned sd, uint32_t data)
   {
      this->reg->txf[sd] = data;
   }

   //! Read from TX FIFO
   uint32_t SM_read(unsigned sd)
   {
      return this->reg->rxf[sd];
   }

   //! Start state machines
   void start(unsigned sd_mask)
   {
      this->setField(this->reg->ctrl, 3, 0, sd_mask);
   }

   //! Stop state machines
   void stop(unsigned sd_mask)
   {
      this->setField(this->reg->ctrl, 3, 0, this->reg->ctrl & ~sd_mask);
   }

private:
   void configOut(unsigned pin, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
      {
         pads_bank.setOut(pin + i, PadsBank::DRIVE_2MA, /* slew_fast */ true);
         io_bank.setFunc( pin + i, INDEX == 0 ? IoBank::PIO0 : IoBank::PIO1);
      }
   }

   void configIn(unsigned pin, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
      {
         pads_bank.setIn(pin + i);
         io_bank.setFunc(pin + i, INDEX == 0 ? IoBank::PIO0 : IoBank::PIO1);
      }
   }

   PadsBank pads_bank{};
   IoBank   io_bank{};
};

class Pio0 : public PioBase<0,0x50200000> {};

class Pio1 : public PioBase<1,0x50300000> {};

} // namespace MTL
