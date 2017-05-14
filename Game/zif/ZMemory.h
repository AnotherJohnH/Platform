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

#ifndef ZMEMORY_H
#define ZMEMORY_H

#include  <cassert>
#include  <cstdint>

#include "ZHeader.h"

#include "ZLog.h"

//! Memory for ZMachine
class ZMemory
{
private:
   static const unsigned MAX_SIZE = 512 * 1024;

   uint8_t   memory[MAX_SIZE];

   uint32_t getLimit() const
   {
      const ZHeader* header = getHeader();

      switch(header->version)
      {
      case 1:
      case 2:
      case 3:
         return 128*1024;

      case 4:
      case 5:
         return 256*1024;

      case 7:
         return 320*1024;

      case 6:
      case 8:
         return 512*1024;

      default:
         assert(!"unexpected version");
         return 0;
      }
   }

public:
   ZMemory()
   {
      memset(memory, 0, MAX_SIZE);
   }

         ZHeader* getHeader()       { return (ZHeader*)memory; }
   const ZHeader* getHeader() const { return (ZHeader*)memory; }

   void init()
   {
      assert(getHeader()->getStorySize() < getLimit());
   }

   //! 
   uint32_t unpackAddr(uint16_t packed_address, bool routine) const
   {
      const ZHeader* header = getHeader();

      switch(header->version)
      {
      case 1:
      case 2:
      case 3:
         return packed_address<<1;

      case 4:
      case 5:
         return packed_address<<2;

      case 6:
      case 7:
         return (packed_address<<2) + (routine ? header->routines<<3
                                               : header->static_strings<<3);

      case 8:
         return packed_address<<3;

      default:
         assert(!"unexpected version");
         return 0;
      }
   }

   uint16_t getChecksum() const
   {
      uint16_t sum = 0;

      for(unsigned i=sizeof(ZHeader); i<getHeader()->getStorySize(); ++i)
      {
         sum += memory[i];
      }

      return sum;
   }

   uint8_t readByte(uint32_t addr) const
   {
      assert(addr < getLimit());
      //tprintf(" {%04X=>%02X}", addr, memory[addr]);
      return memory[addr];
   }

   void writeByte(uint32_t addr, uint8_t value)
   {
      assert(addr < getLimit());
      //tprintf(" {%04X<=%02X}", addr, value);
      memory[addr] = value;
   }

   uint16_t readWord(uint32_t addr) const
   {
      uint16_t word = readByte(addr);
      return (word << 8) | readByte(addr + 1);
   }

   void writeWord(uint32_t addr, uint16_t value)
   {
      writeByte(addr,     value >> 8);
      writeByte(addr + 1, value & 0xFF);
   }

   uint8_t fetchByte(uint32_t& addr) const
   {
      assert(addr < getLimit());
      return memory[addr++];
   }

   uint16_t fetchWord(uint32_t& addr) const
   {
      uint16_t value = fetchByte(addr);
      return (value<<8) | fetchByte(addr);
   }

   uint16_t readGlobal(unsigned index)
   {
      return readWord(getHeader()->glob + index*2);
   }

   void writeGlobal(unsigned index, uint16_t value)
   {
      writeWord(getHeader()->glob + index*2, value);
   }

   void clear(uint32_t addr, uint16_t size)
   {
      for(size_t i=0; i<size; i++)
      {
         memory[addr + i] = 0;
      }
   }

   void copyForward(uint32_t from, uint32_t to, uint16_t size)
   {
      for(size_t i=0; i<size; i++)
      {
         memory[to + i] = memory[from + i];
      }
   }

   void copyBackward(uint32_t from, uint32_t to, uint16_t size)
   {
      for(size_t i=size; i>0; i--)
      {
         memory[to + i - 1] = memory[from + i - 1];
      }
   }
};

#endif
