//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "SDL_headers.h"

#include "PLT/Audio.h"

namespace PLT {

namespace Audio {


static bool sdl_init{false};


template <typename TYPE>
static void mixAudioIn(void* userdata, uint8_t* stream, int len)
{
   In*      in     = (In*)userdata;
   TYPE*    buffer = reinterpret_cast<TYPE*>(stream);
   unsigned n      = len / sizeof(TYPE);

   in->setSamples(buffer, n);
}


template <typename TYPE>
static void mixAudioOut(void* userdata, uint8_t* stream, int len)
{
   Out*     out    = (Out*)userdata;
   TYPE*    buffer = reinterpret_cast<TYPE*>(stream);
   unsigned n      = len / sizeof(TYPE);

   out->getSamples(buffer, n);
}


IOBase::IOBase(unsigned freq_,
               Format   format_,
               unsigned channels_,
               unsigned samples_,
               bool     input)
   : freq(freq_)
   , format(format_)
   , channels(channels_)
{
   if(not sdl_init)
   {
      SDL_InitSubSystem(SDL_INIT_AUDIO);
      sdl_init = true;
   }

   SDL_AudioSpec requested;

   requested.freq     = freq;
   requested.channels = channels;
   requested.samples  = samples_;

   if(input)
      requested.userdata = (In*)this;
   else
      requested.userdata = (Out*)this;

   switch(format)
   {
   case Format::SINT8:
      requested.format   = AUDIO_S8;
      requested.callback = input ? mixAudioIn<int8_t>
                                 : mixAudioOut<int8_t>;
      break;

   case Format::SINT16:
      requested.format   = AUDIO_S16;
      requested.callback = input ? mixAudioIn<int16_t>
                                 : mixAudioOut<int16_t>;
      break;

   case Format::SINT32:
      requested.format   = AUDIO_S32;
      requested.callback = input ? mixAudioIn<int32_t>
                                 : mixAudioOut<int32_t>;
      break;

   default:
      return;
   }

   SDL_AudioSpec obtained;

   handle = SDL_OpenAudioDevice(nullptr,
                                input ? 1 : 0,
                                &requested, &obtained,
                                SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
   if(handle == 0)
   {
      SDL_Log("SDL_OpenAudioDevice() failed : %s", SDL_GetError());
      return;
   }

   freq = obtained.freq;

   open = true;
}

IOBase::~IOBase()
{
   if(open)
   {
      SDL_CloseAudioDevice(handle);
   }
}

bool IOBase::setEnable(bool enable_)
{
   if(!open) return false;

   if(enable_ != enable)
   {
      enable = enable_;

      SDL_PauseAudioDevice(handle, enable ? 0 : 1);
   }

   return true;
}

} // namespace Audio

} // namespace PLT
