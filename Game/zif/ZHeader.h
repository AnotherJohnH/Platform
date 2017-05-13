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

#ifndef ZHEADER_H
#define ZHEADER_H

#include  <stdint.h>
#include  <cassert>

#include "ZConfig.h"
#include "ZConsole.h"

//! Overlay for a Z story header
struct ZHeader
{
   //! A 16-bit unsigned integer that is stored in big-endian format
   class Big16
   {
   public:
      Big16(const uint16_t value = 0)
      {
         operator=(value);
      }

      operator uint16_t() const
      {
         return (byte[0]<<8) | byte[1];
      }

      const uint16_t operator=(const uint16_t value)
      {
         byte[0] = value >> 8;
         byte[1] = value;
         return value;
      }

   private:
      uint8_t  byte[2];
   };

   uint8_t        version;                  // $00: Version number
   uint8_t        flags1;                   // $01: Flags 1
   Big16          release;                  // $02
   Big16          himem;                    // $04: Byte address of high memory
   Big16          init_pc;                  // $06: Byte address of first instruction
   Big16          dict;                     // $08: Byte address of dictionary
   Big16          obj;                      // $0A: Byte address of object table
   Big16          glob;                     // $0C: Byte address of globals
   Big16          stat;                     // $0E: Byte address of static memory
   uint8_t        flags2;                   // $10: Flags 2
   uint8_t        pad1[7];
   Big16          abbr;                     // $18: Byte address of abbreviations table
   Big16          length;                   // $1A: Length of file
   Big16          checksum;                 // $1C: Checksum

   uint8_t        interpreter_number;       // $1E:
   uint8_t        interpreter_version;      // $1F:
   uint8_t        screen_lines;             // $20:
   uint8_t        screen_cols;              // $21:
   Big16          screen_width;
   Big16          screen_height;

   // TODO these are swapped v5 V v6?
   uint8_t        font_height;
   uint8_t        font_width;

   Big16          routines;
   Big16          static_strings;
   uint8_t        background_colour;
   uint8_t        foreground_colour;
   Big16          terminating_characters;
   Big16          width_text_stream3;
   Big16          standard_revision;
   Big16          alphabet_table;
   Big16          header_ext;
   uint8_t        pad2[8];

   ZHeader()
   {
      assert(sizeof(ZHeader) == 64);
   }

   //! Check for supported versions
   bool isVersionValid() const
   {
      return (version >= 1) && (version <= 8);
   }

   //! Return size of story (bytes)
   uint32_t getStorySize() const
   {
      switch(version)
      {
      case 1:
      case 2:
      case 3:
         return length<<1;

      case 4:
      case 5:
         return length<<2;

      case 6:
      case 7:
      case 8:
         return length<<3;

      default:
         return 0;
      }
   }

   //! 
   void print() const
   {
      printf( "Version : %d\n",            version);
      printf( "Flags1  : %02X\n",          flags1);
      printf( "Flags2  : %02X\n",          flags2);
      printf( "Length  : 0x%04X\n",        (uint16_t)length);
      printf( "\n" );
      printf( "Dynamic Memory : 0x0040-0x%04X\n", (uint16_t)stat - 1);
      printf( "Static Memory  : 0x%04X-0x%04X\n", (uint16_t)stat,
                                                  getStorySize() > 0xFFFF ? 0xFFFF
                                                                          : getStorySize());
      printf( "High Memory    : 0x%04X-0x%05X\n", (uint16_t)himem, getStorySize());
      printf( "\n" );
      printf( "Objects : 0x%04X\n",      (uint16_t)obj     );
      printf( "Globals : 0x%04X\n",      (uint16_t)glob    );
      printf( "Abbr    : 0x%04X\n",      (uint16_t)abbr    );
      printf( "Dict    : 0x%04X\n",      (uint16_t)dict    );
      printf( "Init PC : 0x%04X\n",      (uint16_t)init_pc );
      printf( "\n" );
   }

   //!
   void init(ZConsole& console, ZConfig& config)
   {
      if (version <= 3)
      {
         if (config.status_line)      flags1 |= 1<<4;
         if (config.screen_splitting) flags1 |= 1<<5;
         if (config.var_pitch_font)   flags1 |= 1<<6;
      }
      else
      {
         if (console.getAttr(ZConsole::BOLD))          flags1 |= 1<<2;
         if (console.getAttr(ZConsole::ITALIC))        flags1 |= 1<<3;
         if (console.getAttr(ZConsole::FIXED_FONT))    flags1 |= 1<<4;
         if (console.getAttr(ZConsole::READ_TIMEOUT))  flags1 |= 1<<7;

         if (version >= 5)
         {
            if (console.getAttr(ZConsole::COLOURS))  flags1 |= 1<<0;

            if (version >= 6)
            {
               if (config.pictures)   flags1 |= 1<<1;
               if (config.sounds)     flags1 |= 1<<5;
            }
         }
      }

      // 8.1.5.1
      if ((version == 5) && !console.getAttr(ZConsole::GRAPHIC_FONT))
      {
         flags2 &= ~(1<<3);
      }

      standard_revision = (config.interp_major_version<<8) |
                           config.interp_minor_version;

      if (version >= 4)
      {
         interpreter_number  = 0;
         interpreter_version = 'A';
      }
      else
      {
         interpreter_number  = 0;
         interpreter_version = 0;
      }

      screen_lines = console.getAttr(ZConsole::LINES);
      screen_cols  = console.getAttr(ZConsole::COLS);

      font_height  = console.getAttr(ZConsole::FONT_HEIGHT);
      font_width   = console.getAttr(ZConsole::FONT_WIDTH);
   }
};

#endif
