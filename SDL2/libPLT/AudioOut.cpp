//------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
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

#include "SDL.h"
#include "SDL_audio.h"

#include "PLT/AudioOut.h"

namespace PLT {

template <typename TYPE, TYPE zero>
static void mixAudio(void* userdata, uint8_t* stream, int len)
{
   AudioOut* audio = (AudioOut*)userdata;

   TYPE*    buffer = reinterpret_cast<TYPE*>(stream);
   unsigned n      = len/sizeof(TYPE);

   if (!audio->fill(buffer, n))
   {
      for(unsigned i=0; i<n; ++i)
      {
         buffer[i] = zero;
      }
   }
}

bool AudioOut::setFormat(AudioFormat format_, unsigned channels_)
{
   if (format != AUDIO_NONE)
   {
      SDL_CloseAudio();
   }

   SDL_AudioSpec fmt;

   fmt.freq     = freq;
   fmt.channels = channels_;
   fmt.samples  = 1024;
   fmt.userdata = this;

   switch(format_)
   {
   case PLT::AUDIO_UINT8:  fmt.format = AUDIO_U8;  fmt.callback = mixAudio<uint8_t, 0x80>;   break;
   case PLT::AUDIO_UINT16: fmt.format = AUDIO_U16; fmt.callback = mixAudio<uint16_t,0x8000>; break;

   case PLT::AUDIO_SINT8:  fmt.format = AUDIO_S8;  fmt.callback = mixAudio<int8_t, 0>; break;
   case PLT::AUDIO_SINT16: fmt.format = AUDIO_S16; fmt.callback = mixAudio<int16_t,0>; break;
   case PLT::AUDIO_SINT32: fmt.format = AUDIO_S32; fmt.callback = mixAudio<int32_t,0>; break;

   default: return false;
   }

   if (SDL_OpenAudio(&fmt, nullptr) < 0) return false;

   format = format_;

   return true;
}

AudioOut::~AudioOut()
{
   if (format != AUDIO_NONE)
   {
      SDL_CloseAudio();
   }
}

void AudioOut::setEnable(bool enable_)
{
   if (format == AUDIO_NONE) return;

   if (enable_ != enable)
   {
      enable = enable_;

      SDL_PauseAudio(enable ? 0 : 1);
   }
}

} // namespace PLT

