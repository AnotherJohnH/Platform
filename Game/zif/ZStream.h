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

#ifndef Z_STREAM_H
#define Z_STREAM_H

#include <cassert>

#include "ZConsole.h"
#include "ZMemory.h"

//! Stream handling
class ZStream : public ZConsole
{
private:
   static const unsigned NUM_STREAM      = 4;
   static const unsigned MAX_WORD_LENGTH = 16;

   enum Stream
   {
      SCREEN  = 0,
      PRINTER = 1,
      MEMORY  = 2,
      SNOOP   = 3
   };

   //! Unbuffered write of an output character
   void send(uint16_t zscii)
   {
      if (zscii == '\0') return;

      if (zscii == '\n')
         col = 1;
      else
         col++;

      if (enable[SCREEN])  addch(zscii);
      if (enable[PRINTER]) print(zscii);
   }

   void flushOutput()
   {
      for(unsigned i=0; i<buffer_size; i++)
      {
         send(buffer[i]);
      }

      buffer_size = 0;
   }

   //! Buffered write of an output character
   void sendBuffered(uint16_t zscii)
   {
      if ((zscii == ' ') || (zscii == '\n') || (buffer_size == MAX_WORD_LENGTH))
      {
         if ((col + buffer_size) > getAttr(ZConsole::COLS))
         {
            send('\n');
         }

         flushOutput();

         send(zscii);
      }
      else
      {
         buffer[buffer_size++] = zscii;
      }
   }

public:
   ZStream(PLT::Device* device_, ZMemory& memory_)
      : ZConsole(device_)
      , col(1)
      , buffer_enable(false)
      , buffer_size(0)
      , print_input(false)
      , memory(&memory_)
   {
      for(unsigned i=0; i<NUM_STREAM; i++)
      {
         enable[i] = false;
      }
   }

   void initStreams(uint8_t version)
   {
      enable[SCREEN]  = true;
      enable[PRINTER] = true;
      enable[SNOOP]   = true;
      buffer_enable   = true;
      buffer_size     = 0;
      print_input     = version <= 5;
   }

   void setCol(unsigned col_)
   {
      col = col_;
   }

   //! Control automatic line breaking
   bool setBuffering(bool buffer_enable_)
   {
      bool prev = buffer_enable;
      buffer_enable = buffer_enable_;
      return prev;
   }

   void flush()
   {
      if (buffer_enable)  flushOutput();
   }

   //! Control state of output streams
   void enableStream(unsigned index, bool enable_ = true)
   {
      assert((index >= 1) && (index <= 4));
      enable[index - 1] = enable_;
   }

   void enableMemoryStream(uint32_t table_, int16_t width_)
   {
      table     = table_;
      table_ptr = table + 2;
      width     = width_;

      enable[MEMORY] = true;

      memory->writeWord(table, 0);
   }

   //! Read ZSCII character
   bool readChar(uint16_t& zscii, unsigned timeout)
   {
      flushOutput();

      bool ok = read(zscii, timeout);
      if (ok)
      {
         // Echo input to enabled output streams
         if (enable[SCREEN])                 addch(zscii);
         if (enable[PRINTER] && print_input) print(zscii);
         if (enable[SNOOP])                  snoop(zscii);

         if (zscii == '\n') col = 1;
      }

      return ok;
   }

   //! Write ZSCII character
   void writeChar(uint16_t zscii)
   {
      if (enable[MEMORY])
      {
         memory->writeWord(table, memory->readWord(table) + 1);
         memory->writeByte(table_ptr++, zscii);
      }
      else if (buffer_enable)
      {
         sendBuffered(zscii);
      }
      else
      {
         send(zscii);
      }
   }

   //! Write signed integer value
   void writeNumber(int32_t value)
   {
      if (value < 0)
      {
         writeChar('-');
         value = -value;
      }

      int place = 10000;

      while((place > 1) && (value < place))
      {
         place = place / 10;
      }

      for(; place; place = place / 10)
      {
         unsigned digit = value / place;

         writeChar('0' + digit);

         value -= place * digit;
      }
   }

   //! Write raw string with no buffering
   void writeRaw(const char* s)
   {
      for(; *s; s++)
      {
         send(*s);
      }
   }

private:
   unsigned   col;
   bool       buffer_enable;
   uint8_t    buffer_size;
   uint8_t    buffer[MAX_WORD_LENGTH];
   bool       print_input;
   bool       enable[4];
   ZMemory*   memory;
   uint32_t   table;
   uint32_t   table_ptr;
   int16_t    width;
};

#endif
