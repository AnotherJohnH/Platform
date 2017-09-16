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
      return decode(command, data, length);
   }
   else
   {
      // Running status
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

   case SYSEX:
      switch(channel)
      {
      case 0:
         {
            unsigned size;
            for(size = 1; size<length; ++size)
            {
               if (data[size] == 0xF7) break;
            }
            sysExcl(size - 1, data + 1);
            return size + 1;
         }
         break;

      case  1: /* undefined */                       return 1;
      case  2: songPosition((data[1]<<7) | data[0]); return 3;
      case  3: songSelect(data[0]);                  return 2;
      case  4: /* undefined */                       return 1;
      case  5: /* undefined */                       return 1;
      case  6: sysEvent(TUNE_REQUEST);               return 1;
      case  7: /* end of SysEx */                    return 1;
      case  8: sysEvent(CLOCK);                      return 1;
      case  9: /* undefined */                       return 1;
      case 10: sysEvent(START);                      return 1;
      case 11: sysEvent(CONTINUE);                   return 1;
      case 12: sysEvent(STOP);                       return 1;
      case 13: /* undefined */                       return 1;
      case 14: sysEvent(ACTIVE_SENSING);             return 1;

      case 15:
         sysEvent(RESET);
         return 1;

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

