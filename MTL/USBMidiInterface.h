//------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
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

#include "MTL/chip/USBDevice.h"

#include "STB/Fifo.h"

#include "USB/Device.h"
#include "USB/Interface.h"
#include "USB/Audio/Control.h"
#include "USB/Audio/MIDIStreaming.h"

namespace MTL {

//! USB Audio Controller interface
class USBACInterface : public USB::Interface
{
public:
   USBACInterface(List& list_)
      : USB::Interface(list_, USB::CLASS_AUDIO, USB::AC::SUB_CLASS)
   {
   }

   USB::AC::Header header{descr_list};
};


//! USB MIDI Streaming interface
class USBMIDIStreamingInterface : public USB::Interface
{
public:
   USBMIDIStreamingInterface(List& list_)
      : USB::Interface(list_, USB::CLASS_AUDIO, USB::MS::SUB_CLASS)
   {
      // This needs to be the end-point address which will be 2
      // but be nice to compute for more complex devices
      d_jack_out.source[0].id = 2;
   }

   bool empty() const { return fifo.empty(); }

   uint8_t rx()
   {
      uint8_t byte = fifo.back();
      fifo.pop();
      return byte;
   }

private:
   void configured() override
   {
      midi_in.startRx(64);
   }

   void buffRx(uint8_t ep_, const uint8_t* data_, unsigned length_) override
   {
      for(unsigned offset = 0; offset < length_; offset += 4)
      {
         const uint8_t* msg = data_ + offset;

         unsigned len;

         switch(msg[0] & 0xF)
         {
         case 0x0: len = 0; break; // ignore reserved misc function codes
         case 0x1: len = 0; break; // ignore reserved cable events
         case 0x2: len = 2; break;
         case 0x5: len = 1; break;
         case 0x6: len = 2; break;
         case 0xC: len = 2; break;
         case 0xD: len = 2; break;
         case 0xF: len = 1; break;
         default:  len = 3; break;
         }

         for(unsigned i = 0; i < len; ++i)
            fifo.push(msg[1 + i]);
      }

      midi_out.startTx(0);
   }

   void buffTx(uint8_t ep_) override
   {
      midi_in.startRx(64);
   }

   STB::Fifo<uint8_t, 10> fifo{};

   // USB Configuration descriptors
   USB::MS::HeaderDescr        d_header{descr_list};
   USB::MS::JackInDescr        d_jack_in{ descr_list, USB::MS::EMBEDDED, 1};
   USB::MS::JackOutDescr<1>    d_jack_out{descr_list, USB::MS::EMBEDDED, 2};
   USB::MS::EndPointDescr      d_midi_in{ descr_list, USB::EndPointDescr::OUT, USB::EndPointDescr::BULK};
   USB::MS::CSEndPointDescr<1> d_cs_midi_in{descr_list, /* jack */ 1};
   USB::MS::EndPointDescr      d_midi_out{descr_list, USB::EndPointDescr::IN, USB::EndPointDescr::BULK};
   USB::MS::CSEndPointDescr<1> d_cs_midi_out{descr_list, /* jack */ 2};

   //! Endpoint implementation
   MTL::USBEndPoint midi_in{d_midi_in};
   MTL::USBEndPoint midi_out{d_midi_out};
};


//! USB-MIDI interface
class USBMidiInterface
{
public:
   USBMidiInterface(USB::Device* device_)
      : ac_interface{device_->getInterfaceList()}
      , ms_interface{device_->getInterfaceList()}
   {
   }

   bool empty() const { return ms_interface.empty(); }

   uint8_t rx() { return ms_interface.rx(); }

private:
   USBACInterface            ac_interface;
   USBMIDIStreamingInterface ms_interface;
};

}
