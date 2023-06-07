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

// \brief Code writer for PIO peripheral

#pragma once

#include <cassert>
#include <cstdint>

#include "PioAsmLbl.h"

namespace PIO {

enum JmpCond
{
   ALWAYS     = 0b000, X_EQ_Z = 0b001, X_NE_Z_DEC = 0b010, Y_EQ_Z        = 0b011,
   Y_NE_Z_DEC = 0b100, X_NE_Y = 0b101, PIN        = 0b110, OSR_NOT_EMPTY = 0b111
};

enum WaitCond
{
   GPIO_LO = 0b000, PIN_LO = 0b001, IRQ_LO = 0b010,
   GPIO_HI = 0b100, PIN_HI = 0b101, IRQ_HI = 0b110
};

enum Operand
{
//          000   001   010   011   100      101     110   111
// IN       PINS  X     Y     ZERO  -        -       ISR   OSR
// OUT      PINS  X     Y     IGN   PINDIRS  PC      ISR   EXEC_OUT
// MOV dst  PINS  X     Y     -     EXEC_MOV PC      ISR   OSR
// MOV src  PINS  X     Y     ZERO  -        STATUS  ISR   OSR
// SET      PINS  X     Y     -     PINDIRS  -       -     -

   PINS    = 0b000, X = 0b001,  Y = 0b010,   ZERO = 0b011,
   PINDIRS = 0b100, PC = 0b101, ISR = 0b110, OSR  = 0b111,

   IGN      = 0b011,
   EXEC_OUT = 0b111,
   EXEC_MOV = 0b100,
   STATUS   = 0b101
};

enum FifoMode
{
   BLOCK             = 0b0100000,
   NO_BLOCK          = 0b0000000,
   IF_FULL_BLOCK     = 0b1100000,
   IF_FULL_NO_BLOCK  = 0b1000000,
   IF_EMPTY_BLOCK    = 0b1100000,
   IF_EMPTY_NO_BLOCK = 0b1000000
};

enum Irq { SET = 0b00, WAIT = 0b01, CLR = 0b10 };


//! Assembler for PIO code
class Asm
{
public:
   enum OpCode
   {
      OP_JMP = 0b000, OP_WFC = 0b001
   };

   Asm(unsigned side_set_bits_ = 0)
       : side_set_bits(side_set_bits_)
   {
   }

   //! Number of side set bits
   unsigned sideSetBits() const { return side_set_bits; }

   //! Number of instructions encoded
   unsigned size() const { return pc; }

   uint8_t getEntry() const { return lbl_entry; }

   uint8_t getWrap() const { return lbl_wrap; }

   uint8_t getWrapTarget() const { return lbl_wrap_target; }

   //! Read program
   uint16_t operator[](unsigned addr) const { return prog[addr]; }

   void side_set(unsigned side_set_bits_)
   {
      side_set_bits = side_set_bits_;
   }

   //! Fix a label at the current location
   void lbl(Lbl& label) { label.resolve(pc, prog); }

   //! Fix entry label at the current location
   void entry()       { lbl(lbl_entry);       }

   //! Fix wrap label at the current location
   void wrap()        { lbl(lbl_wrap);        }

   //! Fix wrap target label at the current location
   void wrap_target() { lbl(lbl_wrap_target); }

   //! Jump to address if condition is true
   Asm& JMP(JmpCond cond, Lbl& label)
   {
       return code(OP_JMP, (cond << 5) | label.reference(pc));
   }

   //! Jump to address
   Asm& JMP(Lbl& label)
   {
       return code(OP_JMP, (ALWAYS << 5) | label.reference(pc));
   }

   //! Stall until a condition is met
   Asm& WFC(WaitCond cond, unsigned index5)
   {
       return code(OP_WFC, (cond << 5) | index5);
   }

   //! Shift bit_count bits from source into the ISR
   Asm& INP(Operand source, unsigned bit_count = 1)
   {
       return code(0b010, (source << 5) | (bit_count & 0x1F));
   }

   //! Shift bit_count bits from OSR into the destination
   Asm& OUT(Operand dest, unsigned bit_count = 1)
   {
       return code(0b011, (dest << 5) | (bit_count & 0x1F));
   }

   //! Push contents of the ISR into the RX FIFO
   Asm& PSH(FifoMode mode = BLOCK)
   {
       return code(0b100, mode);
   }

   //! Pull contents of the TX FIFO into the OSR
   Asm& POP(FifoMode mode = BLOCK)
   {
       return code(0b100, (1 << 7) | mode);
   }

   //! Copy data from source to destination
   Asm& MOV(Operand dest, Operand source)
   {
       return code(0b101, (dest << 5) | (0b00 << 3) | source);
   }

   //! Invert data from source to destination
   Asm& INV(Operand dest, Operand source)
   {
       return code(0b101, (dest << 5) | (0b01 << 3) | source);
   }

   //! Bit reverese data from source to destination
   Asm& REV(Operand dest, Operand source)
   {
       return code(0b101, (dest << 5) | (0b10 << 3) | source);
   }

   //! Set or clear an IRQ
   Asm& IRQ(Irq mode, unsigned irq_num)
   {
       return code(0b110, (mode << 5) | irq_num);
   }

   //! Write immediate value to destination
   Asm& SET(Operand dest, unsigned imm5)
   {
       return code(0b111, (dest << 5) | imm5);
   }

   //! No operation
   Asm& NOP()
   {
      return MOV(Y, Y);
   }

   //! Set delay in the instruction just written
   Asm& delay(unsigned cycles)
   {
      assert(cycles < (1 << (5 - side_set_bits)));

      uint16_t& inst = prog[pc - 1];

      inst = inst | (cycles << 8);
      return *this;
   }

   //! Set side set bits in the instruction just written
   Asm& side(unsigned bits)
   {
      assert(bits < (1 << side_set_bits));

      uint16_t& inst = prog[pc - 1];
      inst = inst | (bits << (13 - side_set_bits));

      return *this;
   }

   //! Get the instruction just encoded
   uint16_t op() const { return prog[pc - 1]; }

private:
   Asm& code(unsigned op_, uint16_t inst_)
   {
       prog[pc++] = (op_ << 13) | inst_;

       return *this;
   }

   static const unsigned MAX_INST = 32;

   uint8_t  side_set_bits{0};
   uint8_t  pc{0};
   uint16_t prog[MAX_INST];
   Lbl      lbl_entry{0x00};
   Lbl      lbl_wrap{0x1F};
   Lbl      lbl_wrap_target{0x00};
};

} // namespace PIO
