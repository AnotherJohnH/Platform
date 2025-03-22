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

#include "MIDIInstrumentBase.h"

namespace MIDI {

//! MIDI instrument with a voice allocator
template <unsigned N>
class Instrument : public InstrumentBase
{
public:
   static const unsigned NUM_VOICES = N;

   Instrument(uint8_t base_channel_ = 0)
      : InstrumentBase(NUM_VOICES, base_channel_)
   {
      for(unsigned i = 0; i < NUM_VOICES; ++i)
         setVoiceState(i, FREE);
   }

   bool isAnyVoiceOn() const { return getOnVoices() != 0; }

   unsigned getOnVoices() const { return on_voices; }

protected:
   //! Deallocate the given voice so that it may be reallocated
   void voiceFree(signed voice_)
   {
      setVoiceState(voice_, FREE);
   }

private:
   using State = uint8_t;
   using Event = uint16_t;

   //! Find the voice playing with the given state
   signed findVoice(uint8_t note_) override
   {
      for(unsigned voice = 0; voice < NUM_VOICES; ++voice)
      {
         if (voice_state[voice] == note_)
            return voice;
      }

      return -1;
   }

   //! Allocate a voice
   signed allocVoice(uint8_t note_) override
   {
      signed voice = findVoice(FREE);
      if (voice < 0)
      {
         // Find the oldest GATE_OFF voice
         uint8_t max_age = 0;

         for(unsigned i = 0; i < NUM_VOICES; ++i)
         {
            if (voice_state[i] == GATE_OFF)
            {
               uint8_t age = current_event - voice_event[i];
               if (age > max_age)
               {
                  age   = max_age;
                  voice = i;
               }
            }
         }
      }

      if (voice < 0)
      {
         // Find the oldest ON voice
         uint8_t max_age = 0;

         for(unsigned i = 0; i < NUM_VOICES; ++i)
         {
            uint8_t age = current_event - voice_event[i];
            if (age > max_age)
            {
               age   = max_age;
               voice = i;
            }
         }
      }

      ++on_voices;
      setVoiceState(voice, note_);
      return voice;
   }

   //! Set voice state to gate off
   signed offVoice(uint8_t note_) override
   {
      signed voice = findVoice(note_);
      if (voice >= 0)
      {
         setVoiceState(voice, GATE_OFF);
         --on_voices;
      }

      return voice;
   }

   void setVoiceState(signed index_, State state_)
   {
      voice_state[index_] = state_;
      voice_event[index_] = current_event++;
   }

   static const State GATE_OFF = 0xFF;
   static const State FREE     = 0xFE;

   Event   current_event{1};
   State   voice_state[NUM_VOICES] = {};  //!< Gate on note value or FREE or GATE_OFF
   Event   voice_event[NUM_VOICES] = {};
   uint8_t on_voices{0};
};


} // namespace MIDI
