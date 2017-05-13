//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef ZMACHINE_H
#define ZMACHINE_H

#include <stdint.h>
#include <cstring>

#include "ZWindow.h"
#include "ZMemory.h"
#include "ZStack.h"
#include "ZHeader.h"
#include "ZConfig.h"
#include "ZObject.h"
#include "ZText.h"
#include "ZParser.h"


#define TRACE if (1) ; else tprintf


class ZMachine
{
private:
   typedef void (ZMachine::*OpPtr)();

   static const unsigned STACK_SIZE   = 1024;
   static const unsigned MAX_OPERANDS = 8;

   enum OperandType
   {
      OP_LARGE_CONST = 0,
      OP_SMALL_CONST = 1,
      OP_VARIABLE    = 2,
      OP_NONE        = 3
   };

   ZMemory               memory;
   ZStack<STACK_SIZE>    stack;
   ZObject               object;
   ZText                 text;
   ZParser               parser;
   ZHeader*              header;
   uint16_t              initial_checksum;
   bool                  quit;
   uint32_t              pc;
   uint32_t              rand_state;
   unsigned              num_arg;
   union
   {
       uint16_t uarg[MAX_OPERANDS];
       int16_t  sarg[MAX_OPERANDS];
   };

   // Op-code decoders
   OpPtr  op0[0x10];
   OpPtr  op1[0x10];
   OpPtr  op2[0x20];
   OpPtr  opV[0x20];
   OpPtr  opE[0x20];

   void warning(const char* message)
   {
#if 0
      text.clear();
      text.attron(PLT::A_REVERSE);
      text.move(1, 1);
      text.clrtoeol();
      text.mvaddstr(1, 1, "ZIF WARNING");
      text.attroff(PLT::A_REVERSE);
      text.mvaddstr(3, 1, message);
      (void) text.getch();
#endif
   }

   void error(const char* message)
   {
      text.clear();
      text.attron(PLT::A_REVERSE);
      text.move(1, 1);
      text.clrtoeol();
      text.mvaddstr(1, 1, "ZIF ERROR");
      text.attroff(PLT::A_REVERSE);
      text.mvaddstr(3, 1, message);
      (void) text.getch();
      quit = true;
   }

   unsigned version() const { return header->version; }

   //! Returns true if the initial checksum matches the header checksum
   bool isChecksumOk()    const { return initial_checksum == header->checksum; }

   uint8_t  fetchByte()     { return memory.fetchByte(pc); }
   uint16_t fetchWord()     { return memory.fetchWord(pc); }

   uint32_t unpackAddr(uint16_t paddr, bool routine) const
   {
      return memory.unpackAddr(paddr, routine);
   }

   //! Read a variable
   uint16_t varRead(uint8_t index, bool peek = false)
   {
      if (index == 0)
      {
         return peek ? stack.peek() : stack.pop();
      }
      else if (index < 16)
      {
         return stack.frame(index - 1);
      }
      else
      {
         return memory.readGlobal(index - 16);
      }
   }

   //! Write a variable
   void varWrite(uint8_t index, uint16_t value, bool peek = false)
   {
      if (index == 0)
      {
         if (peek)
            stack.peek() = value;
         else
            stack.push(value);
      }
      else if (index < 16)
      {
         stack.frame(index - 1) = value;
      }
      else
      {
         memory.writeGlobal(index - 16, value);
      }

      TRACE(" [W%02X=%04X]", index, value);
   }

   //! Conditional branch
   void branch(bool cond)
   {
      uint8_t  type           = fetchByte();
      bool     branch_if_true = (type & (1<<7)) != 0;
      bool     long_branch    = (type & (1<<6)) == 0;
      int16_t  offset         = type & 0x3F;

      if (long_branch)
      {
         offset = (offset<<8) | fetchByte();
         // Sign extend
         offset = int16_t(offset << 3) >> 3;
      }

      TRACE(" B%c %04X", branch_if_true ? 'T' : 'F', offset & 0xFFFF);

      if (cond == branch_if_true)
      {
         if ((offset == 0) || (offset == 1))
         {
            subRet(offset);
         }
         else
         {
            pc += offset - 2;
         }
      }
   }

   //! Call a sub-routine
   void subCall(unsigned         call_type,
                uint16_t         packed_addr,
                unsigned         argc,
                const uint16_t*  argv)
   {
      stack.push(uint16_t(call_type));
      stack.push(pc);
      stack.pushFrame(argc);

      pc = unpackAddr(packed_addr, /* routine */ true);

      uint8_t num_locals = fetchByte();

      for(unsigned i=0; i<num_locals; ++i)
      {
         uint16_t value = 0;

         if (version() <= 4)
         {
            value = fetchWord();
         }

         if (i < argc)
         {
            value = argv[i];
         }

         stack.push(value);
      }

      TRACE("   // call %06x args=%d locals=%d",
            pc, argc, num_locals - argc);
   }

   //! Return from a sub-routine
   void subRet(uint16_t value)
   {
      stack.popFrame();
      stack.pop(pc);

      uint16_t call_type = stack.pop();
      switch(call_type)
      {
      case 0: varWrite(fetchByte(), value); break;
      case 1: /* throw return value away */ break;
      case 2: stack.push(value);            break;

      default: error("Bad call type");
      }

      TRACE("   // ret %06X", pc);
   }

   void showStatus()
   {
      TODO_IGNORE("show_status");
   }

   uint16_t random(int16_t arg)
   {
      if (arg <= 0)
      {
         arg = -arg;

         if (arg == 0)
         {
         }
         else if (arg < 1000)
         {
         }
         else
         {
            rand_state = arg;
         }

         return 0;
      }
      else
      {
         rand_state = 0x015A4E35 * rand_state + 1;
         uint16_t value = (rand_state >> 16) & 0x7FFF;
         return (value % arg) + 1;
      }
   }

   void ILLEGAL()  { error("Illegal operation"); }

   void TODO()     { error("Unimplemented operation"); }

   void TODO_IGNORE(const char* op)
   {
      warning(op);
   }

   void doOp0(uint8_t op_code)
   {
      (this->*op0[op_code & 0xF])();
   }

   void doOp1(uint8_t op_code)
   {
      (this->*op1[op_code & 0xF])();
   }

   void doOp2(uint8_t op_code)
   {
      (this->*op2[op_code & 0x1F])();
   }

   void doOpV(uint8_t op_code)
   {
      (this->*opV[op_code & 0x1F])();
   }

   void doOpE(uint8_t op_code)
   {
      (this->*opE[op_code & 0x1F])();
   }

   //============================================================================
   // Zero operand instructions

   //! rtrue - Return true (i.e. 1) from the current routine
   void op0_rtrue()        { subRet(1); }

   //! rfalse - Return false (i.e. 0) from the current routine
   void op0_rfalse()       { subRet(0); }

   //! print - Print the literal Z-encoded string
   void op0_print()        { pc = text.print(pc); }

   //! print_ret - Print the literal Z-encoded string, a new-line then return true
   void op0_print_ret()    { op0_print(); op0_new_line(); op0_rtrue(); }

   //! nop - Probably the offiical "nop"
   void op0_nop()          {}

   //! v1 save ?(label)
   void op0_save_v1()      { branch(false); } // TODO

   //! v4 save -> (result)
   void op0_save_v4()      { varWrite(fetchByte(), false); } // TODO

   //! v1 restore ?(label)
   void op0_restore_v1()   { branch(false); } // TODO

   //! v4 restore -> (result)
   void op0_restore_v4()   { varWrite(fetchByte(), false); } // TODO

   //! restart
   void op0_restart()      { TODO(); }

   //! ret_popped
   void op0_ret_popped()   { subRet(stack.pop()); }

   //! pop
   void op0_pop()          { stack.pop(); }

   //! catch -> (result)
   void op0_catch()        { varWrite(fetchByte(), stack.framePtr()); }

   //! quit
   void op0_quit()         { quit = true; }

   //! new_line
   void op0_new_line()     { text.writeChar('\n'); }

   //! show_status
   void op0_show_status()  { showStatus(); }

   //! verify ?(label)
   void op0_verify()       { branch(isChecksumOk()); }

   //! "extended opcodes"
   void op0_extend()
   {
       uint8_t ext_opcode = fetchByte();
       TRACE(" %02x", ext_opcode);

       fetchOperands(4);

       doOpE(ext_opcode);
   }

   //! piracy ?(label)
   void op0_piracy()       { branch(true); }

   //============================================================================
   // One operand instructions

   void op1_jz()            { branch(uarg[0] == 0); }

   void op1_get_sibling()
   {
      uint16_t obj = object.getSibling(uarg[0]);
      varWrite(fetchByte(), obj);
      branch(obj != 0);
   }

   void op1_get_parent()
   {
      uint16_t obj = object.getParent(uarg[0]);
      varWrite(fetchByte(), obj);
   }

   void op1_get_child()
   {
      uint16_t obj = object.getChild(uarg[0]);
      varWrite(fetchByte(), obj);
      branch(obj != 0);
   }

   void op1_get_prop_len()  { varWrite(fetchByte(), object.propSize(uarg[0])); }

   void op1_inc()           { varWrite(uarg[0], varRead(uarg[0]) + 1); }

   void op1_dec()           { varWrite(uarg[0], varRead(uarg[0]) - 1); }

   void op1_print_addr()    { text.print(uarg[0]); }

   void op1_call_1s()       { subCall(0, uarg[0], 0, 0); }

   void op1_remove_obj()    { object.remove(uarg[0]); }

   void op1_print_obj()     { text.print(object.getName(uarg[0])); }

   void op1_ret()           { subRet(uarg[0]); }

   void op1_jump()          { pc += sarg[0] - 2; }

   void op1_print_paddr()   { text.print(unpackAddr(uarg[0], /* routine */false)); }

   void op1_load()          { varWrite(fetchByte(), varRead(uarg[0], true)); }

   void op1_not()           { varWrite(uarg[0], ~uarg[0]); }

   void op1_call_1n()       { subCall(1, uarg[0], 0, 0); }

   //============================================================================
   // Two operand instructions
   void op2_je()
   {
      branch(((num_arg > 1) && (uarg[0] == uarg[1])) ||
             ((num_arg > 2) && (uarg[0] == uarg[2])) ||
             ((num_arg > 3) && (uarg[0] == uarg[3])));
   }

   void op2_jl()            { branch(sarg[0] < sarg[1]); }
   void op2_jg()            { branch(sarg[0] > sarg[1]); }

   void op2_dec_chk()
   {
      int16_t value = varRead(uarg[0]) - 1;
      varWrite(uarg[0], value);
      branch(value < sarg[1]);
   }

   void op2_inc_chk()
   {
      int16_t value = varRead(uarg[0]) + 1;
      varWrite(uarg[0], value);
      branch(value > sarg[1]);
   }

   void op2_jin()           { branch(object.getParent(uarg[0]) == uarg[1]); }
   void op2_test_bitmap()   { branch((uarg[0] & uarg[1]) == uarg[1]); }
   void op2_or()            { varWrite(fetchByte(), uarg[0] | uarg[1]); }
   void op2_and()           { varWrite(fetchByte(), uarg[0] & uarg[1]); }
   void op2_test_attr()     { branch(object.getAttr(uarg[0], uarg[1])); }
   void op2_set_attr()      { object.setAttr(uarg[0], uarg[1], true); }
   void op2_clear_attr()    { object.setAttr(uarg[0], uarg[1], false); }
   void op2_store()         { varWrite(uarg[0], uarg[1], true); }
   void op2_insert_obj()    { object.insert(uarg[0], uarg[1]); }

   //! 2OP:15 0F loadw array word_index -> (result)
   void op2_loadw()
   {
      varWrite(fetchByte(), memory.readWord(uarg[0]+2*uarg[1]));
   }

   //! 2OP:16 10 loadb array byte_index -> (result)
   //  Stores array->byte_index (i.e., the byte at address array+byte_index,
   //  which must lie in static or dynamic memory)
   void op2_loadb()
   {
      varWrite(fetchByte(), memory.readByte(uarg[0] + uarg[1]));
   }

   void op2_get_prop()      { varWrite(fetchByte(), object.getProp(uarg[0], uarg[1])); }
   void op2_get_prop_addr() { varWrite(fetchByte(), object.getPropAddr(uarg[0], uarg[1])); }
   void op2_get_next_prop() { varWrite(fetchByte(), object.getPropNext(uarg[0], uarg[1])); }
   void op2_add()           { varWrite(fetchByte(), sarg[0] + sarg[1]); }
   void op2_sub()           { varWrite(fetchByte(), sarg[0] - sarg[1]); }
   void op2_mul()           { varWrite(fetchByte(), sarg[0] * sarg[1]); }

   void op2_div()
   {
      if (sarg[1] == 0) error("Division by zero");
      varWrite(fetchByte(), sarg[0] / sarg[1]);
   }

   void op2_mod()
   {
      if (sarg[1] == 0) error("Division by zero");
      varWrite(fetchByte(), sarg[0] % sarg[1]);
   }

   void op2_call_2s()           { subCall(0, uarg[0], 1, &uarg[1]); }
   void op2_call_2n()           { subCall(1, uarg[0], 1, &uarg[1]); }
   void op2_set_colour()        { text.colourset(uarg[0], uarg[1]); /* TODO v6 window */ }
   void op2_throw()             { TODO(); }

   //============================================================================
   // Variable operand instructions
   void opV_call()
   {
      if (uarg[0] == 0)
         varWrite(fetchByte(), 0);
      else
         subCall(0, uarg[0], num_arg-1, &uarg[1]);
   }

   void opV_call_vs()
   {
      opV_call(); // TODO decode call_vs() here
   }

   void opV_not()            { varWrite(fetchByte(), ~uarg[0]); }
   void opV_call_vn()        { subCall(1, uarg[0], num_arg-1, &uarg[1]); }
   void opV_call_vn2()       { opV_call_vn(); /* TODO decode call_vn2() here */ }
   void opV_storew()         { memory.writeWord(uarg[0] + 2*uarg[1], uarg[2]); }
   void opV_storeb()         { memory.writeByte(uarg[0] + uarg[1], uarg[2]); }
   void opV_put_prop()       { object.setProp(uarg[0], uarg[1], uarg[2]); }

   //! V1 sread text parse
   //! V4 sread text parse timeout routine
   template <bool TIMER, bool SHOW_STATUS>
   void opV_sread()
   {
      uint16_t buffer  = uarg[0];
      uint16_t parse   = uarg[1];
      uint16_t timeout = TIMER ? uarg[2] : 0;
      uint16_t routine = TIMER ? uarg[3] : 0;

      if (SHOW_STATUS) showStatus();

      uint8_t  max = memory.readByte(buffer++) - 1;
      uint16_t start = buffer;

      for(uint8_t len=0; len<max; len++)
      {
          uint16_t ch;

          if (!text.readChar(ch, timeout))
          {
             // TODO branch to routine
             (void) routine;
             break;
          }

          if (ch == '\b')
          {
             // => delete
             if (buffer > start)
             {
                text.writeRaw(" \b");
                --buffer;
                --len;
             }
          }
          else if (ch == '\n')
          {
             memory.writeByte(buffer, '\0');
             break;
          }
          else
          {
             memory.writeByte(buffer++, tolower(ch));
          }
      }

      parser.tokenise(memory, parse, start, header->dict, false);
   }

   //! aread text parse timeout routine -> (result)
   void opV_aread()
   {
      uint16_t buffer  = uarg[0];
      uint16_t parse   = uarg[1];
      uint16_t timeout = uarg[2];
      uint16_t routine = uarg[3];

      uint8_t  max = memory.readByte(buffer++);
      uint8_t  len = memory.readByte(buffer++);

      uint16_t start = buffer;

      for(; len<max; len++)
      {
          uint16_t ch;

          if (!text.readChar(ch, timeout))
          {
             // TODO branch to routine
             (void) routine;
             break;
          }

          if (ch == '\b')
          {
             // => delete
             if (buffer > start)
             {
                text.writeRaw(" \b");
                --buffer;
                --len;
             }
          }
          else if (ch == '\n')
          {
             memory.writeByte(buffer, '\0');
             memory.writeByte(start - 1, len);
             varWrite(fetchByte(), ch);
             break;
          }
          else
          {
             memory.writeByte(buffer++, ch);
          }
      }

      if (parse != 0)
      {
         parser.tokenise(memory, parse, start, header->dict, false);
      }
   }

   void opV_print_char()     { text.writeChar(uarg[0]); }
   void opV_print_num()      { text.writeNumber(sarg[0]); }
   void opV_random()         { varWrite(fetchByte(), random(sarg[0])); }
   void opV_push()           { stack.push(uarg[0]); }

   void opV_pull_v1()
   {
      varWrite(uarg[0], stack.pop(), true);
   }

   void opV_pull_v6()
   {
      uint16_t  value;
      if (num_arg == 1)
      {
         // User stack
         uint16_t st  = uarg[0];
         uint16_t ptr = memory.readWord(st);
         memory.writeWord(st, ++ptr);
         value = memory.readWord(ptr + 2*ptr);
      }
      else
      {
         value = stack.pop();
      }

      varWrite(fetchByte(), value, true);
   }

   void opV_split_window()   { text.split(uarg[0]); }
   void opV_set_window()     { text.select(uarg[0]); }
   void opV_call_vs2()       { subCall(0, uarg[0], num_arg-1, &uarg[1]); }
   void opV_erase_window()   { text.eraseWindow(uarg[0]); }
   void opV_erase_line_v4()  { TODO(); }
   void opV_erase_line_v6()  { TODO(); }
   void opV_set_cursor_v4()  { text.move(uarg[0], uarg[1]); }
   void opV_set_cursor_v6()  { TODO_IGNORE("set_cursor_v6"); }
   void opV_get_cursor()     { TODO(); }

   void opV_set_text_style()
   {
      text.flush();

      unsigned attr = 0;
      if (uarg[0] & (1<<0)) attr |= PLT::A_REVERSE;;
      if (uarg[0] & (1<<1)) attr |= PLT::A_BOLD;;
      if (uarg[0] & (1<<2)) attr |= PLT::A_ITALIC;;
      if (uarg[0] & (1<<3)) attr |= PLT::A_FIXED;;
      text.attrset(attr);
   }

   void opV_buffer_mode()
   {
      text.setBuffering(uarg[0] != 0);
   }

   void opV_output_stream()
   {
      int16_t number = sarg[0];

      if (number == 3)
      {
         uint32_t  table  = num_arg >= 2 ? uarg[1] : 0;
         int16_t   width  = num_arg == 3 ? sarg[2] : 0;

         text.enableMemoryStream(table, width);
      }
      else if (number > 0)
      {
         text.enableStream(number, true);
      }
      else if (number < 0)
      {
         text.enableStream(-number, false);
      }
   }

   void opV_input_stream()
   {
      TODO();
   }

   void opV_sound_effect()
   {
      TODO();
   }

   void opV_read_char()
   {
      uint16_t timeout = uarg[0];
      uint16_t routine = uarg[1];

      uint16_t ch;
      if (!text.readChar(ch, timeout))
      {
         pc = routine;
      }

      varWrite(fetchByte(), ch);
   }

   void opV_scan_table()
   {
      uint16_t x      = uarg[0];
      uint16_t table  = uarg[1];
      uint16_t len    = uarg[2];
      uint16_t form   = num_arg == 4 ? uarg[3] : 0x82;
      uint16_t result = 0;

      for(uint16_t i=0; i<len; ++i)
      {
         uint16_t v = (form & 0x80) ? memory.readWord(table)
                                    : memory.readByte(table);

         if (v == x)
         {
            result = table;
            break;
         }

         table += form & 0x7F;
      }

      varWrite(fetchByte(), result);

      branch(result != 0);
   }

   void opV_tokenise()
   {
      uint16_t text  = uarg[0];
      uint16_t parse = uarg[1];
      uint16_t dict  = num_arg >= 3 ? uarg[2] : uint16_t(header->dict);
      bool     flag  = num_arg == 4 ? uarg[3] != 0 : false;

      parser.tokenise(memory, parse, text + 1, dict, flag);
   }

   void opV_encode_text()
   {
      TODO();
   }

   void opV_copy_table()
   {
      uint16_t from = uarg[0];
      uint16_t to   = uarg[1];
      int16_t  size = sarg[2];

      if (to == 0)
      {
         memory.clear(from, size);
      }
      else if ((size < 0) || (from < to))
      {
         memory.copyForward(from, to, abs(size));
      }
      else
      {
         memory.copyBackward(from, to, size);
      }
   }

   void opV_print_table()
   {
      uint16_t addr   = uarg[0];
      uint16_t width  = uarg[1];
      uint16_t height = num_arg >= 3 ? uarg[2] : 1;
      int16_t  skip   = num_arg == 4 ? uarg[3] : 0;

      text.printTable(addr, width, height, skip);
   }

   void opV_check_arg_count()
   {
      branch(uarg[0] <= stack.frameArgs());
   }

   //============================================================================
   // Extended operand instructions

   void opE_save_table()
   {
      TODO();
   }

   void opE_restore_table()
   {
      TODO();
   }

   void opE_log_shift()
   {
      if (sarg[1] < 0)
         varWrite(fetchByte(), uarg[0] >> -sarg[1]);
      else
         varWrite(fetchByte(), uarg[0] << sarg[1]);
   }

   void opE_art_shift()
   {
      if (sarg[1] < 0)
         varWrite(fetchByte(), sarg[0] >> -sarg[1]);
      else
         varWrite(fetchByte(), sarg[0] << sarg[1]);
   }

   void opE_save_undo()
   {
      varWrite(fetchByte(), 1);
   }

   void opE_restore_undo()
   {
      TODO();
   }

   void opE_print_unicode()
   {
      TODO();
   }

   void opE_check_unicode()
   {
      TODO();
   }

   //! EXT:4
   //  set_font font
   void opE_set_font()
   {
      // text.setFont(uarg[0]); TODO
      varWrite(fetchByte(), 0);
   }

   void opE_move_window()
   {
      uint16_t wind = uarg[0];
      uint16_t y    = uarg[1];
      uint16_t x    = uarg[2];

      (void) wind;
      (void) y;
      (void) x;

      TODO_IGNORE("move_window");
   }

   void opE_window_size()
   {
      uint16_t wind = uarg[0];
      uint16_t y    = uarg[1];
      uint16_t x    = uarg[2];

      (void) wind;
      (void) y;
      (void) x;

      TODO_IGNORE("window_size");
   }

   void opE_window_style()
   {
      uint16_t wind      = uarg[0];
      uint16_t flags     = uarg[1];
      uint16_t operation = uarg[2];

      (void) wind;
      (void) flags;
      (void) operation;

      TODO_IGNORE("set_wind_style");
   }

   void opE_get_wind_prop()
   {
      uint16_t wind = uarg[0];
      uint16_t prop = uarg[1];

      varWrite(fetchByte(), text.getWindowProp(wind, prop));
   }

   void initDecoder()
   {
      // Zero operand instructions
      op0[0x0] =                  &ZMachine::op0_rtrue;
      op0[0x1] =                  &ZMachine::op0_rfalse;
      op0[0x2] =                  &ZMachine::op0_print;
      op0[0x3] =                  &ZMachine::op0_print_ret;
      op0[0x4] =                  &ZMachine::op0_nop;
      op0[0x5] = version() <= 3 ? &ZMachine::op0_save_v1
               : version() == 4 ? &ZMachine::op0_save_v4
                                : &ZMachine::ILLEGAL;
      op0[0x6] = version() <= 3 ? &ZMachine::op0_restore_v1
               : version() == 4 ? &ZMachine::op0_restore_v4
                                : &ZMachine::ILLEGAL;
      op0[0x7] =                  &ZMachine::op0_restart;
      op0[0x8] =                  &ZMachine::op0_ret_popped;
      op0[0x9] = version() <= 4 ? &ZMachine::op0_pop
                                : &ZMachine::op0_catch;
      op0[0xA] =                  &ZMachine::op0_quit;
      op0[0xB] =                  &ZMachine::op0_new_line;
      op0[0xC] = version() <= 2 ? &ZMachine::ILLEGAL
               : version() == 3 ? &ZMachine::op0_show_status
                                : &ZMachine::op0_nop;
      op0[0xD] = version() >= 3 ? &ZMachine::op0_verify
                                : &ZMachine::ILLEGAL;
      op0[0xE] = version() >= 5 ? &ZMachine::op0_extend
                                : &ZMachine::ILLEGAL;
      op0[0xF] = version() >= 5 ? &ZMachine::op0_piracy
                                : &ZMachine::ILLEGAL;

      // One operand instructions
      op1[0x0] =                  &ZMachine::op1_jz;
      op1[0x1] =                  &ZMachine::op1_get_sibling;
      op1[0x2] =                  &ZMachine::op1_get_child;
      op1[0x3] =                  &ZMachine::op1_get_parent;
      op1[0x4] =                  &ZMachine::op1_get_prop_len;
      op1[0x5] =                  &ZMachine::op1_inc;
      op1[0x6] =                  &ZMachine::op1_dec;
      op1[0x7] =                  &ZMachine::op1_print_addr;
      op1[0x8] = version() >= 4 ? &ZMachine::op1_call_1s
                                : &ZMachine::ILLEGAL;
      op1[0x9] =                  &ZMachine::op1_remove_obj;
      op1[0xA] =                  &ZMachine::op1_print_obj;
      op1[0xB] =                  &ZMachine::op1_ret;
      op1[0xC] =                  &ZMachine::op1_jump;
      op1[0xD] =                  &ZMachine::op1_print_paddr;
      op1[0xE] =                  &ZMachine::op1_load;
      op1[0xF] = version() <= 4 ? &ZMachine::op1_not
                                : &ZMachine::op1_call_1n;

      // Two operand instructions
      op2[0x00] =                  &ZMachine::ILLEGAL;
      op2[0x01] =                  &ZMachine::op2_je;
      op2[0x02] =                  &ZMachine::op2_jl;
      op2[0x03] =                  &ZMachine::op2_jg;
      op2[0x04] =                  &ZMachine::op2_dec_chk;
      op2[0x05] =                  &ZMachine::op2_inc_chk;
      op2[0x06] =                  &ZMachine::op2_jin;
      op2[0x07] =                  &ZMachine::op2_test_bitmap;
      op2[0x08] =                  &ZMachine::op2_or;
      op2[0x09] =                  &ZMachine::op2_and;
      op2[0x0A] =                  &ZMachine::op2_test_attr;
      op2[0x0B] =                  &ZMachine::op2_set_attr;
      op2[0x0C] =                  &ZMachine::op2_clear_attr;
      op2[0x0D] =                  &ZMachine::op2_store;
      op2[0x0E] =                  &ZMachine::op2_insert_obj;
      op2[0x0F] =                  &ZMachine::op2_loadw;
      op2[0x10] =                  &ZMachine::op2_loadb;
      op2[0x11] =                  &ZMachine::op2_get_prop;
      op2[0x12] =                  &ZMachine::op2_get_prop_addr;
      op2[0x13] =                  &ZMachine::op2_get_next_prop;
      op2[0x14] =                  &ZMachine::op2_add;
      op2[0x15] =                  &ZMachine::op2_sub;
      op2[0x16] =                  &ZMachine::op2_mul;
      op2[0x17] =                  &ZMachine::op2_div;
      op2[0x18] =                  &ZMachine::op2_mod;
      op2[0x19] = version() >= 4 ? &ZMachine::op2_call_2s
                                 : &ZMachine::ILLEGAL;
      op2[0x1A] = version() >= 5 ? &ZMachine::op2_call_2n
                                 : &ZMachine::ILLEGAL;
      op2[0x1B] = version() >= 5 ? &ZMachine::op2_set_colour
                                 : &ZMachine::ILLEGAL;
      op2[0x1C] = version() >= 5 ? &ZMachine::op2_throw
                                 : &ZMachine::ILLEGAL;
      op2[0x1D] =                  &ZMachine::ILLEGAL;
      op2[0x1E] =                  &ZMachine::ILLEGAL;
      op2[0x1F] =                  &ZMachine::ILLEGAL;

      // Variable operand instructions
      opV[0x00] = version() <= 3 ? &ZMachine::opV_call
                                 : &ZMachine::opV_call_vs;
      opV[0x01] =                  &ZMachine::opV_storew;
      opV[0x02] =                  &ZMachine::opV_storeb;
      opV[0x03] =                  &ZMachine::opV_put_prop;
      opV[0x04] = version() <= 3 ? &ZMachine::opV_sread<false,true>
                : version() == 4 ? &ZMachine::opV_sread<true,false>
                                 : &ZMachine::opV_aread;
      opV[0x05] =                  &ZMachine::opV_print_char;
      opV[0x06] =                  &ZMachine::opV_print_num;
      opV[0x07] =                  &ZMachine::opV_random;
      opV[0x08] =                  &ZMachine::opV_push;
      opV[0x09] = version() == 6 ? &ZMachine::opV_pull_v6
                                 : &ZMachine::opV_pull_v1;
      opV[0x0A] = version() >= 3 ? &ZMachine::opV_split_window
                                 : &ZMachine::ILLEGAL;
      opV[0x0B] = version() >= 3 ? &ZMachine::opV_set_window
                                 : &ZMachine::ILLEGAL;
      opV[0x0C] = version() >= 4 ? &ZMachine::opV_call_vs2
                                 : &ZMachine::ILLEGAL;
      opV[0x0D] = version() >= 4 ? &ZMachine::opV_erase_window
                                 : &ZMachine::ILLEGAL;
      opV[0x0E] = version() >= 4 ? &ZMachine::opV_erase_line_v4
                : version() >= 6 ? &ZMachine::opV_erase_line_v6
                                 : &ZMachine::ILLEGAL;
      opV[0x0F] = version() >= 4 ? &ZMachine::opV_set_cursor_v4
                : version() >= 6 ? &ZMachine::opV_set_cursor_v6
                                 : &ZMachine::ILLEGAL;
      opV[0x10] = version() >= 4 ? &ZMachine::opV_get_cursor      : &ZMachine::ILLEGAL;
      opV[0x11] = version() >= 4 ? &ZMachine::opV_set_text_style  : &ZMachine::ILLEGAL;
      opV[0x12] = version() >= 4 ? &ZMachine::opV_buffer_mode     : &ZMachine::ILLEGAL;
      opV[0x13] = version() >= 3 ? &ZMachine::opV_output_stream   : &ZMachine::ILLEGAL;
      opV[0x14] = version() >= 3 ? &ZMachine::opV_input_stream    : &ZMachine::ILLEGAL;
      opV[0x15] = version() >= 5 ? &ZMachine::opV_sound_effect    : &ZMachine::ILLEGAL;
      opV[0x16] = version() >= 4 ? &ZMachine::opV_read_char       : &ZMachine::ILLEGAL;
      opV[0x17] = version() >= 4 ? &ZMachine::opV_scan_table      : &ZMachine::ILLEGAL;
      opV[0x18] = version() >= 5 ? &ZMachine::opV_not             : &ZMachine::ILLEGAL;
      opV[0x19] = version() >= 5 ? &ZMachine::opV_call_vn         : &ZMachine::ILLEGAL;
      opV[0x1A] = version() >= 5 ? &ZMachine::opV_call_vn2        : &ZMachine::ILLEGAL;
      opV[0x1B] = version() >= 5 ? &ZMachine::opV_tokenise        : &ZMachine::ILLEGAL;
      opV[0x1C] = version() >= 5 ? &ZMachine::opV_encode_text     : &ZMachine::ILLEGAL;
      opV[0x1D] = version() >= 5 ? &ZMachine::opV_copy_table      : &ZMachine::ILLEGAL;
      opV[0x1E] = version() >= 5 ? &ZMachine::opV_print_table     : &ZMachine::ILLEGAL;
      opV[0x1F] = version() >= 5 ? &ZMachine::opV_check_arg_count : &ZMachine::ILLEGAL;

      // Externded instructions
      for(unsigned i=0; i<=0x1F; i++)
      {
         opE[i] = &ZMachine::ILLEGAL;
      }

      if (version() < 5) return;

      opE[0x00] = &ZMachine::opE_save_table;
      opE[0x01] = &ZMachine::opE_restore_table;
      opE[0x02] = &ZMachine::opE_log_shift;
      opE[0x03] = &ZMachine::opE_art_shift;
      opE[0x04] = &ZMachine::opE_set_font;
      opE[0x09] = &ZMachine::opE_save_undo;
      opE[0x0A] = &ZMachine::opE_restore_undo;
      opE[0x0B] = &ZMachine::opE_print_unicode;
      opE[0x0C] = &ZMachine::opE_check_unicode;

      if (version() < 6) return;

      opE[0x05] = &ZMachine::TODO;
      opE[0x06] = &ZMachine::TODO;
      opE[0x07] = &ZMachine::TODO;
      opE[0x08] = &ZMachine::TODO;

      opE[0x10] = &ZMachine::opE_move_window;
      opE[0x11] = &ZMachine::opE_window_size;
      opE[0x12] = &ZMachine::opE_window_style;
      opE[0x13] = &ZMachine::opE_get_wind_prop;
      opE[0x14] = &ZMachine::TODO;
      opE[0x15] = &ZMachine::TODO;
      opE[0x16] = &ZMachine::TODO;
      opE[0x17] = &ZMachine::TODO;
      opE[0x18] = &ZMachine::TODO;
      opE[0x19] = &ZMachine::TODO;
      opE[0x1A] = &ZMachine::TODO;
      opE[0x1B] = &ZMachine::TODO;
      opE[0x1C] = &ZMachine::TODO;
   }

   //============================================================================

   void clearOperands()
   {
      num_arg = 0;
   }

   void fetchOperand(uint8_t type)
   {
      uint16_t operand{};

      switch(type)
      {
      case OP_LARGE_CONST: operand = fetchWord();          break;
      case OP_SMALL_CONST: operand = fetchByte();          break;
      case OP_VARIABLE:    operand = varRead(fetchByte()); break;

      case OP_NONE: return;
      }

      TRACE(" (%X)", operand);
      uarg[num_arg++] = operand;
   }

   void fetchOperands(unsigned n)
   {
      uint16_t type = 0;

      if (n == 4)
      {
         type = fetchByte() << 8;
         TRACE(" t%02X", type >> 8);
      }
      else if (n == 8)
      {
         type = fetchWord();
         TRACE(" t%04X", type);
      }

      for(unsigned i=0; i<n; ++i)
      {
         fetchOperand(type >> 14);
         type <<= 2;
      }
   }

   void fetchDecodeExecute()
   {
      static unsigned tick = 0;

      TRACE("%4d %06X:", tick++, pc);

      clearOperands();

      uint8_t opcode = fetchByte();

      if (opcode < 0x80)
      {
         TRACE(" %02X 2OP:%02X    ", opcode, opcode & 0x1F);

         fetchOperand(opcode & (1<<6) ? OP_VARIABLE : OP_SMALL_CONST);
         fetchOperand(opcode & (1<<5) ? OP_VARIABLE : OP_SMALL_CONST);

         doOp2(opcode);
      }
      else if (opcode < 0xB0)
      {
         TRACE(" %02X 1OP:%1X     ", opcode, opcode & 0xF);

         fetchOperand((opcode >> 4) & 3);

         doOp1(opcode);
      }
      else if (opcode < 0xC0)
      {
         TRACE(" %02X 0OP:%1X     ", opcode, opcode & 0xF);

         doOp0(opcode);
      }
      else if (opcode < 0xE0)
      {
         TRACE(" %02X 2OP:%02X", opcode, opcode & 0x1F);

         fetchOperands(4);

         doOp2(opcode);
      }
      else
      {
         TRACE(" %02X VAR:%02X", opcode, opcode & 0x1F);

         if ((opcode == 0xEC) || (opcode == 0xFA))
            fetchOperands(8);
         else
            fetchOperands(4);

         doOpV(opcode);
      }

      TRACE("\n");
   }

   bool load(const char* filename)
   {
      FILE* fp = fopen(filename, "r");
      if (fp == 0)
      {
         error("Failed to open story z-file");
         return false;
      }

      // Read header
      header = memory.getHeader();
      if (fread(header, sizeof(ZHeader), 1, fp) != 1)
      {
         error("Z-file header read failed");
         return false;
      }

      if (!header->isVersionValid())
      {
         error("Unexpected version");
         return false;
      }

      // Read story
      if (fread(header + 1, header->getStorySize() - sizeof(ZHeader), 1, fp) != 1)
      {
         error("Z-file read error");
         return false;
      }

      fclose(fp);

      initial_checksum = memory.getChecksum();

      if (!isChecksumOk())
      {
         warning("checksum fail");
      }

      return true;
   }

public:
   ZMachine(PLT::Device* device_)
      : object(&memory)
      , text(device_, memory)
      , quit(false)
      , rand_state(1)
   {}

   //! Play a Z file
   void open(const char* filename)
   {
      if (!load(filename)) return;

      ZConfig  config;

      config.interp_major_version = 1;
      config.interp_minor_version = 0;

      header->init(text, config);

      memory.init();
      stack.init();
      text.init(header->version, header->abbr);
      parser.init(header->version);
      object.init(header->obj, header->version);

      TRACE("version=%d\n", header->version);

      initDecoder();

      if (header->version != 6)
         pc = header->init_pc;
      else
         pc = unpackAddr(header->init_pc, /* routine */ true) + 1;

      rand_state = 1;

      quit = false;

      while(!quit)
      {
         fetchDecodeExecute();
      }
   }
};


#endif
