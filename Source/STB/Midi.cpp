//------------------------------------------------------------------------------
// Copyright (c) 2015-2017 John D. Haughton
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

#include <cstdlib>
#include <cstdint>

#include "STB/Midi.h"

namespace STB {

namespace MIDI {


unsigned Handler::messageIn(const uint8_t* data, unsigned length)
{
   if ((data[0] & 0x80) == 0)
   {
      // Use running status
      return decode(command, data, length);
   }
   else
   {
      command = data[0];
      return decode(command, data + 1, length - 1) + 1;
   }
}


unsigned Handler::decode(uint8_t command_, const uint8_t* data, unsigned length)
{
   uint8_t message = command_ & 0xF0;
   uint8_t channel = command_ & 0x0F;

   switch(message)
   {
   case NOTE_OFF:
      noteOff(channel, data[0], data[1]);
      return 2;

   case POLY_KEY_PRESSURE:
      notePressure(channel, data[0], data[1]);
      return 2;

   case NOTE_ON:
      if (data[1] != 0)
      {
         noteOn(channel, data[0], data[1]);
      }
      else
      {
         noteOff(channel, data[0], data[1]);
      }
      return 1;

   case CHANNEL_PRESSURE:
      channelPressure(channel, data[0]);
      return 1;

   case PITCH_BEND:
      pitchBend(channel, ((data[1] << 7) | data[0]) - 0x2000);
      return 2;

   case CONTROL_CHANGE:
      switch(data[0])
      {
      case 122:
         localControl(channel, data[1] != 0);
         break;

      case 123:
         allNotesOff(channel);
         break;

      case 124:
         allNotesOff(channel);
         omniMode(channel, /* on */ false);
         break;

      case 125:
         allNotesOff(channel);
         omniMode(channel, /* on */ true);
         break;

      case 126:
         allNotesOff(channel);
         omniMode(channel, /* on */ ((data[1] & 0xF) == 0));
         monoMode(channel, data[1] & 0xF);
         break;

      case 127:
         allNotesOff(channel);
         polyMode(channel);
         break;

      default:
         controlChange(channel, data[0], data[1]);
         break;
      }
      return 2;

   case PROGRAM_CHANGE:
      programChange(channel, data[0]);
      return 1;

   case SYSTEM:
      switch(command_)
      {
      case 0xF0:
         {
            unsigned size;
            for(size = 1; size<length; ++size)
            {
               if (data[size] == 0xF7) break;
            }
            sysExcl(size - 1, data + 1);
            return size + 1;
         }

      case 0xF1: /* undefined */                       return 1;
      case 0xF2: songPosition((data[1]<<7) | data[0]); return 3;
      case 0xF3: songSelect(data[0]);                  return 2;
      case 0xF4: /* undefined */                       return 1;
      case 0xF5: /* undefined */                       return 1;
      case 0xF6: sysEvent(TUNE_REQUEST);               return 1;
      case 0xF7: /* end of SysEx */                    return 1;
      case 0xF8: sysEvent(CLOCK);                      return 1;
      case 0xF9: /* undefined */                       return 1;
      case 0xFA: sysEvent(START);                      return 1;
      case 0xFB: sysEvent(CONTINUE);                   return 1;
      case 0xFC: sysEvent(STOP);                       return 1;
      case 0xFD: /* undefined */                       return 1;
      case 0xFE: sysEvent(ACTIVE_SENSING);             return 1;

      case 0xFF:
         sysEvent(RESET);

         // MIDI file meta events
         switch(data[0])
         {
         case 0x00: // TODO Sequence number
            return 2;

         case 0x01:
         case 0x02:
         case 0x03:
         case 0x04:
         case 0x05:
         case 0x06:
         case 0x07:
            {
               uint32_t length;
               const uint8_t* ptr = data + 1 + decodeVarLength(data + 1, length);
               textEvent(TextEvent(data[1]), (const char*)ptr, length);
               return ptr - data + length;
            }

         case 0x20:  // TODO MIDI channel prefix
            return 3;

         case 0x21:
            return 3;

         case 0x2F: // TODO End of track
            return 2;

         case 0x51: // TODO Set tempo uS
            return 5;

         case 0x54: // TODO SMPTE offset
            return 7;

         case 0x58: // TODO Time signature
            return 6;

         case 0x59: // TODO Key signature
            return 4;

         case 0x7F: // TODO Special
            {
               uint32_t length;
               const uint8_t* ptr = data + 1 + decodeVarLength(data + 1, length);
               return ptr - data + length;
            }

         default:
            return 1;
         }

      default:
         break;
      }
      return 0;

   default:
      return 0;
   }
}

} // namespace MIDI

} // namespace STB

