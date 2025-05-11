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

//! Interface between a MIDI stream and a MIDI instrument
class Interface
{
public:
   Interface() = default;

   Interface(MIDI::Instrument& instrument_, bool debug_ = false)
      : debug(debug_)
   {
      attachInstrument(instrument_);
   }

   //! Assign an instrument to all channels in this interface
   void attachInstrument(MIDI::Instrument& instrument_)
   {
      for(unsigned chan = 0; chan < NUM_CHAN; ++chan)
         inst_map[chan] = &instrument_;
   }

   //! Assign an instrument to a specific channel in this interface
   void attachInstrument(unsigned chan_, MIDI::Instrument& instrument_)
   {
      if (chan_ > NUM_CHAN)
         return;

      inst_map[chan_ - 1] = &instrument_;
   }

   //! Enable console debug
   void setDebug(bool debug_ = true)
   {
      debug = debug_;
   }

   //! Check MIDI stream for pending messages
   void tick()
   {
      while(not empty())
      {
         uint8_t byte = rx();

         if (in_sysex)
         {
            // Handling a SYSEX message
            sysexCommand(byte);

            if (byte == 0xF7)
               in_sysex = false;
         }
         else if ((byte & 0b10000000) != 0)
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
                  in_sysex = true;
                  sysexCommand(byte);
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

   // Implement MIDI stream in derived class

   //! Stream is currently empty
   virtual bool empty() const = 0;

   //! Get next byte from in stream
   virtual uint8_t rx() = 0;

   //! Sent next byte to out stream
   virtual void tx(uint8_t byte) = 0;

private:
   //! Broadcast SYSEX byte
   void sysexCommand(uint8_t byte)
   {
      MIDI::Instrument* sent_inst = {nullptr};

      for(unsigned chan = 0; chan < NUM_CHAN; ++chan)
      {
         MIDI::Instrument* inst = inst_map[chan];

         if ((inst != sent_inst) && (inst != nullptr))
         {
            inst->sysEx(byte);

            sent_inst = inst;
         }
      }
   }

   void channelCommand(unsigned chan, unsigned cmd, uint8_t byte)
   {
      MIDI::Instrument* inst = inst_map[chan];

      switch(cmd)
      {
      case 0x8:
         {
            uint8_t note     = byte;
            uint8_t velocity = rx();
            if (debug) printf("CH%u NOTE OFF %3u %3u\n", chan + 1, note, velocity);
            if (inst != nullptr)
               inst->noteOff(chan, note, velocity);
         }
         break;

      case 0x9:
         {
            uint8_t note     = byte;
            uint8_t velocity = rx();
            if (debug) printf("CH%u NOTE ON  %3u %3u\n", chan + 1, note, velocity);
            if (inst != nullptr)
               inst->noteOn(chan, note, velocity);
         }
         break;

      case 0xA:
         {
            uint8_t note  = byte;
            uint8_t level = rx();
            if (debug) printf("CH%u NOTE PRE %3u %3u\n", chan + 1, note, level);
            if (inst != nullptr)
               inst->notePressure(chan, note, level);
         }
         break;

      case 0xB:
         {
            uint8_t ctrl  = byte;
            uint8_t value = rx();
            if (debug) printf("CH%u CTRL     %3u %3u\n", chan + 1, ctrl, value);
            if (inst != nullptr)
               inst->controlChange(chan, ctrl, value);
         }
         break;

      case 0xC:
         {
            uint8_t prog = byte;
            if (debug) printf("CH%u PROG     %3u\n", chan + 1, prog);
            if (inst != nullptr)
               inst->programChange(chan, prog);
         }
         break;

      case 0xD:
         {
            uint8_t level = byte;
            if (debug) printf("CH%u PRES     %3u\n", chan + 1, level);
            if (inst != nullptr)
               inst->channelPressure(chan, level);
         }
         break;

      case 0xE:
         {
            uint8_t lsb   = byte;
            uint8_t msb   = rx();
            int16_t pitch = ((msb << 7) | lsb) - 0x2000;
            if (debug) printf("CH%u PTCH     %d\n", chan + 1, pitch);
            if (inst != nullptr)
               inst->channelPitchBend(chan, pitch);
         }
         break;
      }
   }

   static const unsigned NUM_CHAN = 16;

   MIDI::Instrument* inst_map[NUM_CHAN] = {nullptr};
   unsigned          cmd {0};
   unsigned          chan {0};
   bool              in_sysex {false};
   bool              debug {false};
};

} // namespace MIDI
