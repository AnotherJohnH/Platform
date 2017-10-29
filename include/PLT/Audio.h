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

//! \brief Platform abstraction interface for raw audio hardware

#ifndef PLT_AUDIO_H
#define PLT_AUDIO_H

#include <cstdint>

namespace PLT {

namespace Audio {


//! Audio sample format
enum Format
{
   NONE = 0, //!< Sample format not configured
   SINT8,    //!< Signed 8-bit sample format
   SINT16,   //!< Signed 16-bit sample format
   SINT32    //!< Signed 32-bit sample format
};


//! Base class for raw audio hardware interfaces
class IOBase
{
public:
   //! Get sample frequency (Hz)
   unsigned getFreq() const { return freq; }

   //! Get sample format
   Format getFormat() const { return format; }

   //! Get number of channels
   //
   //! \return 1 is mono, 2 is stereo
   unsigned getChannels() const { return channels; }

   //! Check if audio interface is ready
   bool isOpen() const { return open; }

   //! Audio interface is enabled
   bool isEnabled() const { return enable; }

   //! Enable or disable audio interface
   //
   //! \param enable true => enable, false => disable
   bool setEnable(bool enable);

protected:
   //! Configure and open a connection to a raw audio device
   //
   //! \param freq Sample frequency (Hz)
   //! \param format Sample format
   //! \param channels Number of audio channels (1 or 2)
   //! \param input true => input, false => output
   IOBase(unsigned freq, Format format, unsigned channels, bool input);

   //! Close a connection to a raw audio device
   ~IOBase();

private:
   unsigned freq{0};
   Format   format{NONE};
   unsigned channels{0};
   bool     open{false};
   bool     enable{false};
   unsigned handle{0};
};


//! Interface to raw audio output hardware
//
//! To use, inherit from this class and override at least one getSamples()
//! method
class Out : public IOBase
{
public:
   //! Configure and open a connection to a raw audio output device
   //
   //! The connections is not enabled by default but can be enabled
   //! using setEnable()
   //! \param freq Sample frequency (Hz)
   //! \param format Sample format
   //! \param channels Number of audio channels (1 or 2)
   Out(unsigned freq, Format format, unsigned channels)
      : IOBase(freq, format, channels, /* input */ false)
   {}

   //! Overide this method to supply 8-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 8-bit signed (SINT8) sample format. This implementation
   //! fills the buffer with silence.
   //! \param buffer Pointer to a buffer for the audio samples
   //! \param n Number of samples required
   virtual void getSamples(int8_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i] = 0;
   }

   //! Overide this method to supply 16-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 16-bit signed (SINT16) sample format. This implementation
   //! fills the buffer with silence.
   //! \param buffer Pointer to a buffer for the audio samples
   //! \param n Number of samples required
   virtual void getSamples(int16_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i] = 0;
   }

   //! Overide this method to supply 32-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 32-bit signed (SINT32) sample format. This implementation
   //! fills the buffer with silence.
   //! \param buffer Pointer to a buffer for the audio samples
   //! \param n Number of samples required
   virtual void getSamples(int32_t* buffer, unsigned n)
   {
      for(unsigned i = 0; i < n; ++i)
         buffer[i] = 0;
   }
};


//! Interface to raw audio input hardware
//
//! To use, inherit from this class and override at least one setSamples()
//! method
class In : public IOBase
{
public:
   //! Configure and open a connection to a raw audio input device
   //
   //! The connections is not enabled by default but can be enabled
   //! using setEnable()
   //! \param freq Sample frequency (Hz)
   //! \param format Sample format
   //! \param channels Number of audio channels (1 or 2)
   In(unsigned freq, Format format, unsigned channels)
      : IOBase(freq, format, channels, /* input */ true)
   {}

   //! Overide this method to recieve 8-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 8-bit signed (SINT8) sample format.
   //! \param buffer Pointer to a buffer of audio samples
   //! \param n Number of samples in the buffer
   virtual void setSamples(const int8_t* buffer, unsigned n) {}

   //! Overide this method to recieve 16-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 16-bit signed (SINT16) sample format.
   //! \param buffer Pointer to a buffer of audio samples
   //! \param n Number of samples in the buffer
   virtual void setSamples(const int16_t* buffer, unsigned n) {}

   //! Overide this method to recieve 32-bit signed sample data
   //
   //! This method will be automatically called in real-time when
   //! the audio device is open, enabled and has been configured
   //! for 32-bit signed (SINT32) sample format.
   //! \param buffer Pointer to a buffer of audio samples
   //! \param n Number of samples in the buffer
   virtual void setSamples(const int32_t* buffer, unsigned n) {}
};


} // namespace Audio

} // namespace PLT

#endif // PLT_AUDIO_H
