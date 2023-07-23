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

#pragma once

#include <cstdio>

#include "STB/MIDIInstrument.h"

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

   void setDebug(bool debug_ = true)
   {
      debug = debug_;
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

               case 0xF0:
                  while(true)
                  {
                     inst->sysEx(byte);
                     if (byte == 0xF7) break;
                     byte = rx();
                  }
                  break;

               default:
                  if (debug) printf("SYSTEM %02X\n", byte);
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
            if (debug) printf("CH%u NOTE OFF %3u %3u\n", chan, note, level);
            inst->noteOff(chan, note, level);
         }
         break;

      case 0x9:
         {
            uint8_t note  = byte;
            uint8_t level = rx();
            if (debug) printf("CH%u NOTE ON  %3u %3u\n", chan, note, level);
            inst->noteOn(chan, note, level);
         }
         break;

      case 0xA:
         {
            uint8_t note  = byte;
            uint8_t level = rx();
            if (debug) printf("CH%u NOTE PRE %3u %3u\n", chan, note, level);
            inst->notePressure(chan, note, level);
         }
         break;

      case 0xB:
         {
            uint8_t ctrl  = byte;
            uint8_t value = rx();
            if (debug) printf("CH%u CTRL     %3u %3u\n", chan, ctrl, value);
            inst->controlChange(chan, ctrl, value);
         }
         break;

      case 0xC:
         {
            uint8_t prog = byte;
            if (debug) printf("CH%u PROG     %3u\n", chan, prog);
            inst->programChange(chan, prog);
         }
         break;

      case 0xD:
         {
            uint8_t level = byte;
            if (debug) printf("CH%u PRES     %3u\n", chan, rx());
            inst->channelPressure(chan, level);
         }
         break;

      case 0xE:
         {
            uint8_t lsb   = byte;
            uint8_t msb   = rx();
            int16_t pitch = ((msb << 7) | lsb) - 0x2000;
            if (debug) printf("CH%u PTCH     %d\n", chan, pitch);
            inst->channelPitchBend(chan, pitch);
         }
         break;
      }
   }

   MIDI::Instrument* inst {nullptr};
   unsigned          cmd {0};
   unsigned          chan {0};
   bool              debug {false};
};

} // namespace MIDI
