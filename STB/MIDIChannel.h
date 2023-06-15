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

//! MIDI channel
class Channel
{
public:
   Channel(uint8_t num_voices_, uint8_t number_ = 0)
      : num_voices(num_voices_)
      , number(number_)
   {
   }

   // Instrument implementation
   virtual signed allocVoice() const { return -1; }
   virtual signed findVoice(uint8_t note) const { return -1; }

   virtual void voiceOn(unsigned voice, uint8_t note, uint8_t level) {}
   virtual void voiceOff(unsigned voice) {}
   virtual void voiceLevel(unsigned voice, uint8_t level) {}
   virtual void voiceControl(unsigned voice, uint8_t control, uint8_t value) {}
   virtual void voicePitch(unsigned voice, int16_t value) {}

   // MIDI Voice messages

   void noteOn(uint8_t channel, uint8_t note,  uint8_t level)
   {
      if ((channel != number) and not omni) return;

      if (poly)
      {
         signed index = allocVoice();
         if (index < 0)
            return;

         voiceOn(index, note, level);
      }
      else
      {
         signed index = channel - number;

         if ((index >= 0) && (index < num_voices))
            voiceOn(index, note, level);
      }
   }

   void noteOff(uint8_t channel, uint8_t note,  uint8_t level)
   {
      if ((channel != number) and not omni) return;

      if (poly)
      {
         signed index = findVoice(note);
         if (index < 0)
            return;
      
         voiceOff(index);
      }
      else
      {
         signed index = channel - number;

         if ((index >= 0) && (index < num_voices))
            voiceOff(index);
      }
   }

   void notePressure(uint8_t channel, uint8_t note, uint8_t level)
   {
      if ((channel != number) and not omni) return;

      if (poly)
      {
         signed index = findVoice(note);
         if (index < 0)
            return;
      }
      else
      {
         signed index = channel - number;

         if ((index >= 0) && (index < num_voices))
            voiceLevel(index, level);
      }
   }

   void controlChange(uint8_t channel, uint8_t control, uint8_t value)
   {
      if ((channel != number) and not omni) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voiceControl(index, control, value);
      }
   }

   void programChange(uint8_t channel, uint8_t index)
   {
      if ((channel != number) and not omni) return;
   }

   void channelPressure(uint8_t channel, uint8_t value)
   {
      if ((channel != number) and not omni) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voiceLevel(index, value);
      }
   }

   void pitchBend(uint8_t channel, int16_t value)
   {
      if ((channel != number) and not omni) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voicePitch(index, value);
      }
   }

   // Channel mode messages

   void localControl(uint8_t channel, bool on)
   {
      if ((channel != number) and not omni) return;

      local_control = on;
   }

   void allNotesOff(uint8_t channel)
   {
      if ((channel != number) and not omni) return;

      for(unsigned index = 0; index < num_voices; ++index)
      {
         voiceOff(index);
      }
   }

   void omniMode(uint8_t channel, bool on)
   {
      if (channel != number) return;

      omni = on;
   }

   void monoMode(uint8_t channel, uint8_t num_channels)
   {
      if ((channel != number) and not omni) return;

      poly          = false;
      mono_channels = num_channels;
   }

   void polyMode(uint8_t channel)
   {
      if ((channel != number) and not omni) return;

      poly          = true;
      mono_channels = 0;
   }

protected:
   uint8_t num_voices {0};
   uint8_t number {0};
   bool    local_control {true};
   bool    omni {true};
   bool    poly {true};
   uint8_t mono_channels {0};
};

} // namespace MIDI
