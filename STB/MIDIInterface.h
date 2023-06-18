//------------------------------------------------------------------------------
// This is free and unencumbered software released into the public domain.
//
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.
//
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// For more information, please refer to <https://unlicense.org>
//------------------------------------------------------------------------------

#pragma once

#include <cstdio>

#include "STB/MIDIInstrument.h"

#define DBG if (1) printf

namespace MIDI {

class Interface
{
public:
   Interface() = default;

   Interface(MIDI::Instrument& instrument_)
   {
      attachInstrument(instrument_);
   }

   void attachInstrument(MIDI::Instrument& instrument_)
   {
      inst = &instrument_;
   }

   void tick()
   {
      while(not empty())
      {
         uint8_t byte = rx();

         if ((byte & 0b10000000) != 0)
         {
            // New command
            cmd = byte >> 4;

            if (cmd == 0xF)
            {
               // System command

               switch(byte)
               {
               case 0xFE:
                  break;

               default:
                  DBG("SYSTEM %02X\n", byte);
                  break;
               }
            }
            else
            {
               // Instrument command

               chan = byte & 0xF;

               channelCommand(chan, cmd, rx());
            }
         }
         else
         {
            // Repeat last channel command
            channelCommand(chan, cmd, byte);
         }
      }
   }

protected:
   virtual bool    empty() const = 0;
   virtual uint8_t rx()          = 0;

private:
   void channelCommand(unsigned chan, unsigned cmd, uint8_t byte)
   {
      switch(cmd)
      {
      case 0x8:
         {
            uint8_t note  = byte;
            uint8_t level = rx();
            DBG("CH%u NOTE OFF %3u %3u\n", chan, note, level);
            inst->noteOff(chan, note, level);
         }
         break;

      case 0x9:
         {
            uint8_t note  = byte;
            uint8_t level = rx();
            DBG("CH%u NOTE ON  %3u %3u\n", chan, note, level);
            inst->noteOn(chan, note, level);
         }
         break;

      case 0xA:
         {
            uint8_t note  = byte;
            uint8_t level = rx();
            DBG("CH%u NOTE PRE %3u %3u\n", chan, note, level);
            inst->notePressure(chan, note, level);
         }
         break;

      case 0xB:
         {
            uint8_t ctrl  = byte;
            uint8_t value = rx();
            DBG("CH%u CTRL     %3u %3u\n", chan, ctrl, value);
            inst->controlChange(chan, ctrl, value);
         }
         break;

      case 0xC:
         {
            uint8_t prog = byte;
            DBG("CH%u PROG     %3u\n", chan, prog);
            inst->programChange(chan, prog);
         }
         break;

      case 0xD:
         {
            uint8_t level = byte;
            DBG("CH%u PRES     %3u\n", chan, rx());
            inst->channelPressure(chan, level);
         }
         break;

      case 0xE:
         {
            uint8_t lsb   = byte;
            uint8_t msb   = rx();
            int16_t pitch = ((msb << 7) | lsb) - 0x2000;
            DBG("CH%u PTCH     %d\n", chan, pitch);
            inst->channelPitchBend(chan, pitch);
         }
         break;
      }
   }

   MIDI::Instrument* inst {nullptr};
   unsigned          cmd {0};
   unsigned          chan {0};
};

} // namespace MIDI
