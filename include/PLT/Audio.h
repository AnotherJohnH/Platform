//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef PLT_AUDIO_H
#define PLT_AUDIO_H

#include <cstdint>

namespace PLT {


enum AudioFormat
{
   AUDIO_NONE,

   AUDIO_UINT8,
   AUDIO_UINT16,

   AUDIO_SINT8,
   AUDIO_SINT16,
   AUDIO_SINT32
};


//! Raw audio base class
class Audio
{
public:
   Audio(unsigned    freq_,
         AudioFormat format_,
         unsigned    channels_,
         bool        input_);

   ~Audio();

   unsigned     getFreq()     const { return freq;     }
   AudioFormat  getFormat()   const { return format;   }
   unsigned     getCHannels() const { return channels; }
   bool         isInput()     const { return input;    }
   bool         isOpen()      const { return open;     }
   bool         isEnabled()   const { return enable;   }

   bool         setEnable(bool enable_);

   virtual void getSamples(uint8_t*  buffer, unsigned n)
   { for(unsigned i=0; i<n; ++i) buffer[i] = 0x80; }

   virtual void getSamples(uint16_t* buffer, unsigned n)
   { for(unsigned i=0; i<n; ++i) buffer[i] = 0x8000; }

   virtual void getSamples(int8_t*   buffer, unsigned n)
   { for(unsigned i=0; i<n; ++i) buffer[i] = 0; }

   virtual void getSamples(int16_t*  buffer, unsigned n)
   { for(unsigned i=0; i<n; ++i) buffer[i] = 0; }

   virtual void getSamples(int32_t*  buffer, unsigned n)
   { for(unsigned i=0; i<n; ++i) buffer[i] = 0; }


   virtual void setSamples(const uint8_t*  buffer, unsigned n) {}

   virtual void setSamples(const uint16_t* buffer, unsigned n) {}

   virtual void setSamples(const int8_t*   buffer, unsigned n) {}

   virtual void setSamples(const int16_t*  buffer, unsigned n) {}

   virtual void setSamples(const int32_t*  buffer, unsigned n) {}


private:
   unsigned     freq{0};
   AudioFormat  format{AUDIO_NONE};
   unsigned     channels{0};
   bool         input{false};
   bool         open{false};
   bool         enable{false};
   unsigned     handle{0};
};


class AudioOut : public Audio
{
public:
   AudioOut(unsigned    freq_,
            AudioFormat format_,
            unsigned    channels_)
      : Audio(freq_, format_, channels_, /* setSamples */ false) {}
};


class AudioIn : public Audio
{
public:
   AudioIn(unsigned    freq_,
           AudioFormat format_,
           unsigned    channels_)
      : Audio(freq_, format_, channels_, /* setSamples */ true) {}
};


} // namespace PLT

#endif
