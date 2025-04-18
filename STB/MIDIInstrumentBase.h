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

#include <cstdint>

namespace MIDI {

//! MIDI instrument base class
class InstrumentBase
{
public:
   InstrumentBase(uint8_t num_voices_, uint8_t base_channel_)
      : num_voices(num_voices_)
      , base_channel(base_channel_)
   {
   }

   //---------------------------------------------------------------------------
   // Override the following for instrument implementation

   //! Mute a voice
   virtual void voiceMute(unsigned voice_) {}

   //! Reset a voice
   virtual void voiceReset(unsigned voice_) {}

   //! Start a voice
   virtual void voiceOn(unsigned voice_, uint8_t note_, uint8_t velocity_) {}

   //! Stop a voice
   virtual void voiceOff(unsigned voice_, uint8_t velocity_) {}

   //! Set voice aftertouch
   virtual void voicePressure(unsigned voice_, uint8_t level_) {}

   //! Voice pitch bend
   virtual void voicePitchBend(unsigned voice_, int16_t value_) {}

   //! Control a voice
   virtual void voiceControl(unsigned voice_, uint8_t control_, uint8_t value_) {}

   //! Program change
   virtual void voiceProgram(unsigned voice_, uint8_t prog_) {}

   // System Exclusive interface
   virtual void sysEx(uint8_t byte_) {}


   //---------------------------------------------------------------------------
   // Override the following for voice management (see MIDI::Instrument)

   virtual signed findVoice(uint8_t note_)  { return -1; }
   virtual signed allocVoice(uint8_t note_) { return -1; }
   virtual signed offVoice(uint8_t note_)   { return -1; }

   //---------------------------------------------------------------------------
   // Incomming MIDI channel message handlers

   void noteOn(uint8_t channel_, uint8_t note_,  uint8_t level_)
   {
      if (level_ == 0)
         return noteOff(channel_, note_, 0);

      if (not isValidChannel(channel_)) return;

      signed index = poly ? allocVoice(note_)
                          : channel_ - base_channel;

      if ((index >= 0) && (index < num_voices))
      {
         voiceOn(index, note_, level_);
      }
   }

   void noteOff(uint8_t channel_, uint8_t note_,  uint8_t velocity_)
   {
      if (not isValidChannel(channel_)) return;

      signed index = poly ? offVoice(note_)
                          : channel_ - base_channel;

      if ((index >= 0) && (index < num_voices))
      {
         voiceOff(index, velocity_);
      }
   }

   void notePressure(uint8_t channel_, uint8_t note_, uint8_t level_)
   {
      if (not isValidChannel(channel_)) return;

      signed index = poly ? findVoice(note_)
                          : channel_ - base_channel;

      if ((index >= 0) && (index < num_voices))
      {
         voicePressure(index, level_);
      }
   }

   void controlChange(uint8_t channel_, uint8_t control_, uint8_t value_)
   {
      if (not isValidChannel(channel_)) return;

      switch (control_)
      {
      case 0x78:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceMute(index);
         }
         break;

      case 0x79:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceReset(index);
         }
         break;

      case 0x7A:
         local_control = value_ == 0x7F;
         break;

      case 0x7B:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceOff(index, /* velocity */ 0);
         }
         break;

      case 0x7C: omni = false; return;
      case 0x7D: omni = true;  return;

      case 0x7E:
         poly         = false;
         num_channels = value_ == 0 ? num_voices : value_;
         break;

      case 0x7F:
         poly         = true;
         num_channels = 1;
         break;

      default:
         for(unsigned index = 0; index < num_voices; ++index)
         {
            voiceControl(index, control_, value_);
         }
         break;
      }
   }

   void programChange(uint8_t channel_, uint8_t prog_)
   {
      if (not isValidChannel(channel_)) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voiceMute(index);
         voiceProgram(index, prog_);
      }
   }

   void channelPressure(uint8_t channel_, uint8_t value_)
   {
      if (not isValidChannel(channel_)) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voicePressure(index, value_);
      }
   }

   void channelPitchBend(uint8_t channel_, int16_t value_)
   {
      if (not isValidChannel(channel_)) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voicePitchBend(index, value_);
      }
   }

   // Channel mode messages

   void allSoundsOff(uint8_t channel_)
   {
      controlChange(channel_, 0x78, 0x00);
   }

   void resetAllControllers(uint8_t channel_)
   {
      controlChange(channel_, 0x79, 0x00);
   }

   void localControl(uint8_t channel_, bool on_)
   {
      controlChange(channel_, 0x7A, on_ ? 0x7F : 0x00);
   }

   void allNotesOff(uint8_t channel_)
   {
      controlChange(channel_, 0x7B, 0x00);
   }

   void omniMode(uint8_t channel_, bool on_)
   {
      controlChange(channel_, on_ ? 0x7D : 0x7C, 0x00);
   }

   void monoMode(uint8_t channel_, uint8_t num_channels_ = 0)
   {
      controlChange(channel_, 0x7E, num_channels_);
   }

   void polyMode(uint8_t channel_)
   {
      controlChange(channel_, 0x7F, 0x00);
   }

private:
   bool isValidChannel(uint8_t channel_)
   {
      return omni or ((channel_ >= base_channel) and
                      (channel_ < (base_channel + num_channels)));
   }

   uint8_t num_voices;
   uint8_t base_channel {0};
   uint8_t num_channels {1};
   bool    local_control {true};
   bool    omni {true};
   bool    poly {true};
};

} // namespace MIDI
