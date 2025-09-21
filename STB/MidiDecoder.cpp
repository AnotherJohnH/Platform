//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "STB/Midi.h"

namespace STB {

namespace MIDI {


unsigned Decoder::decode(const uint8_t* data, unsigned length)
{
   if((data[0] & 0x80) == 0)
   {
      // Use running status
      return decodeCommand(data, length);
   }
   else
   {
      // New command
      state.command = data[0];
      return decodeCommand(data + 1, length - 1) + 1;
   }
}


unsigned Decoder::decodeCommand(const uint8_t* data, unsigned length)
{
   uint8_t message = state.command & 0xF0;
   uint8_t channel = state.command & 0x0F;

   switch(message)
   {
   case NOTE_OFF:
      noteOff(channel, data[0], data[1]);
      return 2;

   case POLY_KEY_PRESSURE:
      notePressure(channel, data[0], data[1]);
      return 2;

   case NOTE_ON:
      if(data[1] != 0)
      {
         noteOn(channel, data[0], data[1]);
      }
      else
      {
         noteOff(channel, data[0], data[1]);
      }
      return 2;

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
      switch(state.command)
      {
      case 0xF0:
      {
         unsigned size;
         for(size = 1; size < length; ++size)
         {
            if(data[size] == 0xF7) break;
         }
         sysExcl(size - 1, data + 1);
         return size + 1;
      }

      case 0xF1: /* undefined */                        return 1;
      case 0xF2: songPosition((data[1]<<7) | data[0]);  return 3;
      case 0xF3: songSelect(data[0]);                   return 2;
      case 0xF4: /* undefined */                        return 1;
      case 0xF5: /* undefined */                        return 1;
      case 0xF6: sysEvent(SystemEvent::TUNE_REQUEST);   return 1;
      case 0xF7: /* end of SysEx */                     return 1;
      case 0xF8: sysEvent(SystemEvent::CLOCK);          return 1;
      case 0xF9: /* undefined */                        return 1;
      case 0xFA: sysEvent(SystemEvent::START);          return 1;
      case 0xFB: sysEvent(SystemEvent::CONTINUE);       return 1;
      case 0xFC: sysEvent(SystemEvent::STOP);           return 1;
      case 0xFD: /* undefined */                        return 1;
      case 0xFE: sysEvent(SystemEvent::ACTIVE_SENSING); return 1;

      case 0xFF:
#if 0
         if (!file_stream)
         {
            sysEvent(RESET);
            return 1;
         }
#endif

         // MIDI file meta events
         switch(data[0])
         {
         case 0x00: // TODO Sequence number
            todo();
            return 2;

         case 0x01:
         case 0x02:
         case 0x03:
         case 0x04:
         case 0x05:
         case 0x06:
         case 0x07:
         {
            uint32_t       length;
            const uint8_t* ptr = data + 1 + decodeVarLength(data + 1, length);
            textEvent(TextEvent(data[1]), (const char*)ptr, length);
            return ptr - data + length;
         }

         case 0x20: // TODO MIDI channel prefix
            todo();
            return 3;

         case 0x21:
            todo();
            return 3;

         case 0x2F: // TODO End of track
            todo();
            return 2;

         case 0x51: // TODO Set tempo uS
            todo();
            return 5;

         case 0x54: // TODO SMPTE offset
            todo();
            return 7;

         case 0x58: // TODO Time signature
            todo();
            return 6;

         case 0x59: // TODO Key signature
            todo();
            return 4;

         case 0x7F: // TODO Special
         {
            uint32_t       length;
            const uint8_t* ptr = data + 1 + decodeVarLength(data + 1, length);
            todo();
            return ptr - data + length;
         }

         default:
            todo();
            return 1;
         }

      default: break;
      }
      break;

   default: break;
   }

   return 1;
}


class Trace : public Decoder
{
public:
   virtual unsigned decode(const uint8_t* data, unsigned length) override
   {
      unsigned size = get_size.decode(data, length);

      bool running_status = (data[0] & 0x80) == 0;

      if(running_status)
      {
         printf("   ");
      }

      for(unsigned i = 0; i < (running_status ? 7 : 8); ++i)
      {
         if(i < size)
            printf(" %02X", data[i]);
         else
            printf("   ");
      }

      printf(" : ");

      return Decoder::decode(data, length);
   }

private:
   virtual void noteOn(uint8_t channel, uint8_t note, uint8_t velocity) override
   {
      printf("CH%u NOTE ON  %3u %3u\n", channel, note, velocity);
   }

   virtual void notePressure(uint8_t channel, uint8_t note, uint8_t value) override
   {
      printf("CH%u NOTE AFT %3u %3u\n", channel, note, value);
   }

   virtual void noteOff(uint8_t channel, uint8_t note, uint8_t velocity) override
   {
      printf("CH%u NOTE OFF %3u %3u\n", channel, note, velocity);
   }

   virtual void controlChange(uint8_t channel, uint8_t index, uint8_t value) override
   {
      printf("CH%u CTRL     %3u %3u\n", channel, index, value);
   }

   virtual void pitchBend(uint8_t channel, int16_t value) override
   {
      printf("CH%u PITCHB   %5d\n", channel, value);
   }

   virtual void channelPressure(uint8_t channel, uint8_t value) override
   {
      printf("CH%u CHAN AFT %3u\n", channel, value);
   }

   virtual void programChange(uint8_t channel, uint8_t index) override
   {
      printf("CH%u PROGRAM  %3u\n", channel, index);
   }

   virtual void textEvent(STB::MIDI::TextEvent event, const char* s, unsigned length) override
   {
      printf("TEXT %u ", unsigned(event));
      for(unsigned i = 0; i < length; i++)
      {
         putchar(s[i]);
      }
      putchar('\n');
   }

   virtual void localControl(uint8_t channel, bool on) override
   {
      printf("CH%u LOCAL_CONTROL %s\n", channel, on ? "ON" : "OFF");
   }

   virtual void allNotesOff(uint8_t channel) override
   {
      printf("CH%u ALL_NOTES_OFF\n", channel);
   }

   virtual void omniMode(uint8_t channel, bool on) override
   {
      printf("CH%u OMNI_MODE %s\n", channel, on ? "ON" : "OFF");
   }

   virtual void monoMode(uint8_t channel, uint8_t num_channels) override
   {
      printf("CH %u MONO %u\n", channel, num_channels);
   }

   virtual void polyMode(uint8_t channel) override
   {
      printf("CH%u POLY\n", channel);
   }

   virtual void sysExcl(uint32_t length, const uint8_t* data) override
   {
      printf("SYSEX\n");
   }

   virtual void sysEvent(SystemEvent event) override
   {
      printf("SYSTEM EVENT\n");
   }

   virtual void songPosition(uint16_t pos) override
   {
      printf("SONG_POSITION %u\n", pos);
   }

   virtual void songSelect(uint8_t song) override
   {
      printf("SONG_SELECT %u\n", song);
   }

   virtual void todo() override
   {
      printf("<todo>\n");
   }

   // Empty decoder just used to determine the length of each event
   Decoder get_size;
};


unsigned disassemble(const uint8_t* data, unsigned length)
{
   static Trace trace;

   return trace.decode(data, length);
}


} // namespace MIDI

} // namespace STB
