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

namespace Audio {

enum Format
{
   NONE = 0,

   SINT8,
   SINT16,
   SINT32
};


//! Raw audio base class
class Base
{
public:
   unsigned getFreq()     const { return freq; }
   Format   getFormat()   const { return format; }
   unsigned getCHannels() const { return channels; }
   bool     isOpen()      const { return open; }
   bool     isEnabled()   const { return enable; }

   bool setEnable(bool enable_);

protected:
   Base(unsigned freq_, Format format_, unsigned channels_, bool input_);

   ~Base();

private:
   unsigned freq{0};
   Format   format{NONE};
   unsigned channels{0};
   bool     open{false};
   bool     enable{false};
   unsigned handle{0};
};


class Out : public Base
{
public:
   Out(unsigned freq_, Format format_, unsigned channels_)
      : Base(freq_, format_, channels_, /* input */ false)
   {}

   virtual void getSamples(int8_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i]   = 0;
   }

   virtual void getSamples(int16_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i]   = 0;
   }

   virtual void getSamples(int32_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i]   = 0;
   }
};


class In : public Base
{
public:
   In(unsigned freq_, Format format_, unsigned channels_)
      : Base(freq_, format_, channels_, /* input */ true)
   {}

   virtual void setSamples(const int8_t* buffer, unsigned n) {}

   virtual void setSamples(const int16_t* buffer, unsigned n) {}

   virtual void setSamples(const int32_t* buffer, unsigned n) {}
};


} // namespace Audio

} // namespace PLT

#endif
