//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief Assembler for PIO state machines

#pragma once

#include <cassert>
#include <cstdint>

#include "PioAsmLbl.h"

namespace PIO {

//! Jump condition for use with the JMP
enum JmpCond
{
   ALWAYS     = 0b000, X_EQ_Z = 0b001, X_NE_Z_DEC = 0b010, Y_EQ_Z        = 0b011,
   Y_NE_Z_DEC = 0b100, X_NE_Y = 0b101, PIN        = 0b110, OSR_NOT_EMPTY = 0b111
};

//! Wait condition for use with the WFC
enum WaitCond
{
   GPIO_LO = 0b000, PIN_LO = 0b001, IRQ_LO = 0b010,
   GPIO_HI = 0b100, PIN_HI = 0b101, IRQ_HI = 0b110
};

//! Operand for use with IN, OUT, MOV and SET
enum Operand
{
//          000   001   010   011   100      101     110   111
// IN       PINS  X     Y     ZERO  -        -       ISR   OSR
// OUT      PINS  X     Y     IGN   PINDIRS  PC      ISR   EXEC_OUT
// MOV dst  PINS  X     Y     -     EXEC_MOV PC      ISR   OSR
// MOV src  PINS  X     Y     ZERO  -        STATUS  ISR   OSR
// SET      PINS  X     Y     -     PINDIRS  -       -     -

   PINS     = 0b000, X  = 0b001, Y   = 0b010, ZERO = 0b011,
   PINDIRS  = 0b100, PC = 0b101, ISR = 0b110, OSR  = 0b111,

   IGN      = 0b011,
   EXEC_OUT = 0b111,
   EXEC_MOV = 0b100,
   STATUS   = 0b101
};

//! Mode for use with PSH and PUL
enum FifoMode
{
   BLOCK             = 0b0100000,
   NO_BLOCK          = 0b0000000,
   IF_FULL_BLOCK     = 0b1100000,
   IF_FULL_NO_BLOCK  = 0b1000000,
   IF_EMPTY_BLOCK    = 0b1100000,
   IF_EMPTY_NO_BLOCK = 0b1000000
};

//! Mode for use with IRQ
enum Irq { SET = 0b00, WAIT = 0b01, CLR = 0b10 };


//! Base class for PIO assemblers
template <unsigned SIZE>
class AsmBase
{
public:
   AsmBase() = default;

   enum OpCode
   {
      OP_JMP = 0b000, OP_WFC = 0b001, OP_INP = 0b010, OP_OUT = 0b011,
      OP_STK = 0b100, OP_MOV = 0b101, OP_IRQ = 0b110, OP_SET = 0b111
   };

   //! Get number of side set bits
   unsigned getSideSetBits() const { return side_set_bits; }

   //! Get side set enable
   bool getSideSetEnable() const { return side_set_enable; }

   //! Set number of side set bits
   void side_set(unsigned side_set_bits_, bool enable_ = false)
   {
       assert(pc == 0);
       side_set_bits   = side_set_bits_;
       side_set_enable = enable_;
   }

   //! Jump to address if condition is true
   AsmBase& JMP(JmpCond cond, unsigned addr5)
   {
      return code(OP_JMP, (cond << 5) | addr5);
   }

   //! Jump to address
   AsmBase& JMP(unsigned addr5)
   {
      return JMP(ALWAYS, addr5);
   }

   //! Stall until a condition is met
   AsmBase& WFC(WaitCond cond, unsigned index5)
   {
      return code(OP_WFC, (cond << 5) | index5);
   }

   //! Shift bit_count bits from source into the ISR
   AsmBase& INP(Operand source, unsigned bit_count = 1)
   {
      return code(OP_INP, (source << 5) | (bit_count & 0x1F));
   }

   //! Shift bit_count bits from OSR into the destination
   AsmBase& OUT(Operand dest, unsigned bit_count = 1)
   {
      return code(OP_OUT, (dest << 5) | (bit_count & 0x1F));
   }

   //! Push contents of the ISR into the RX FIFO
   AsmBase& PSH(FifoMode mode = BLOCK)
   {
      return code(OP_STK, mode);
   }

   //! Pull contents of the TX FIFO into the OSR
   AsmBase& POP(FifoMode mode = BLOCK)
   {
      return code(OP_STK, (1 << 7) | mode);
   }

   //! Copy data from source to destination
   AsmBase& MOV(Operand dest, Operand source)
   {
      return code(OP_MOV, (dest << 5) | (0b00 << 3) | source);
   }

   //! Invert data from source to destination
   AsmBase& INV(Operand dest, Operand source)
   {
      return code(OP_MOV, (dest << 5) | (0b01 << 3) | source);
   }

   //! Bit reverese data from source to destination
   AsmBase& REV(Operand dest, Operand source)
   {
      return code(OP_MOV, (dest << 5) | (0b10 << 3) | source);
   }

   //! Set or clear an IRQ
   AsmBase& IRQ(Irq mode, unsigned irq_num)
   {
      return code(OP_IRQ, (mode << 5) | irq_num);
   }

   //! Write immediate value to destination
   AsmBase& SET(Operand dest, unsigned imm5)
   {
      return code(OP_SET, (dest << 5) | imm5);
   }


   //! Jump to label if condition is true
   AsmBase& JMP(JmpCond cond, Lbl& label) { return JMP(cond, label.reference(pc)); }

   //! Jump to label
   AsmBase& JMP(Lbl& label) { return JMP(label.reference(pc)); }

   //! No operation
   AsmBase& NOP() { return MOV(Y, Y); }


   //! Set delay in the instruction just encoded
   AsmBase& delay(unsigned cycles)
   {
      assert(cycles < (1 << (5 - side_set_bits)));

      uint16_t& inst = prog[pc - 1];
      inst = inst | (cycles << 8);

      return *this;
   }

   //! Set side set bits in the instruction just encoded
   AsmBase& side(unsigned bits)
   {
      assert(bits < (1 << side_set_bits));
      uint16_t& inst = prog[pc - 1];

      if (side_set_enable)
      {
         inst = inst | (1 << 12) | (bits << (12 - side_set_bits));
      }
      else
      {
         inst = inst | (bits << (13 - side_set_bits));
      }

      return *this;
   }

   //! Get the instruction just encoded
   uint16_t op() const { return prog[pc - 1]; }

protected:
   AsmBase& code(unsigned op_, uint16_t inst_)
   {
       // XXX programs that are too big will overwrite themselves
       if (pc == SIZE)
       {
          overwrite = true;
          pc        = 0;
       }

       prog[pc++] = (op_ << 13) | inst_;

       return *this;
   }

   uint8_t  pc{0};
   uint8_t  side_set_bits{0};
   bool     side_set_enable{false};
   bool     overwrite{false};
   uint16_t prog[SIZE];
};

//! Assembler for writing PIO programs
class Asm : public AsmBase<32>
{
public:
   //! Get entry address
   uint8_t getEntry() const { return lbl_entry; }

   //! Get wrap block start address
   uint8_t getWrapTarget() const { return lbl_wrap_target; }

   //! Get wrap block end address
   uint8_t getWrap() const { return lbl_wrap; }

   //! Declare entry at the current location
   void entry() { lbl(lbl_entry); }

   //! Declare wrap block start at the current location
   void wrap_target() { lbl(lbl_wrap_target); }

   //! Declare wrap block end at the previous location
   void wrap() { lbl(lbl_wrap); }

   //! Fix a label at the current location
   void lbl(Lbl& label) { label.resolve(pc, prog); }

   //! Number of instructions encoded
   unsigned size() const { return pc; }

   //! Read program
   uint16_t operator[](unsigned addr) const { return prog[addr]; }

private:
   Lbl lbl_entry{0x00};
   Lbl lbl_wrap{0x1F};
   Lbl lbl_wrap_target{0x00};
};

} // namespace PIO
