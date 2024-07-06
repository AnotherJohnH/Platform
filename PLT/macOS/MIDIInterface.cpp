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

#include <CoreMIDI/CoreMIDI.h>
#include <cstdint>
#include <cstdio>
#include <mutex>

#include "PLT/MIDIInterface.h"
#include "STB/Fifo.h"

namespace PLT {

namespace MIDI {


struct Interface::Pimpl
{
   Pimpl(unsigned device_index_)
   {
      if (not getSource(device_index_))
         return;

      if (not createClient())
         return;

      if (not createPort())
         return;

      if (not connect())
         return;

      connected = true;
   }

   ~Pimpl()
   {
      // TODO
   }

   //! Access one of the MIDI sources in the system
   bool getSource(unsigned device_index_)
   {
      source = MIDIGetSource(device_index_);
      if (source == 0)
      {
         fprintf(stderr, "ERROR - MIDIGetSource() failed\n");
         return false;
      }

      return true;
   }

   //! Create a client
   bool createClient()
   {
      CFStringRef name   = CFStringCreateWithCString(nullptr, "PLT_client", kCFStringEncodingASCII);
      OSStatus    status = MIDIClientCreate(name,
                                            /* notifyProc */   nullptr,
                                            /* notifyRefCon */ nullptr,
                                            &client);
      CFRelease(name);

      if (status != noErr)
      {
         fprintf(stderr, "ERROR - MIDIClientCreate() failed[%d]\n", status);
         return false;
      }

      return true;
   }

   //! Create an input port
   bool createPort()
   {
      CFStringRef name   = CFStringCreateWithCString(nullptr, "PLT_port", kCFStringEncodingASCII);
      OSStatus    status = MIDIInputPortCreate(client,
                                               name,
                                               midiInputCallBack,
                                               this,
                                               &port);
      CFRelease(name);

      if(status != noErr)
      {
         fprintf(stderr, "ERROR - MIDIInputPortCreate() failed [%d]\n", status);
         return false;
      }

      return true;
   }

   // Connect an source to the client port
   bool connect()
   {
      OSStatus status = MIDIPortConnectSource(port, source, /* connRefCon */ nullptr);
      if(status != noErr)
      {
         fprintf(stderr, "ERROR - MIDIConnectSource() failed [%d]\n", status);
         return false;
      }

      return true;
   }

   static void midiInputCallBack(const MIDIPacketList* pkt_list_,
                                 void*                 read_proc_ref_con_,
                                 void*                 src_conn_ref_con_)
   {
      auto pimpl = (Pimpl*)read_proc_ref_con_;

      for(uint32_t i = 0; i < pkt_list_->numPackets; ++i)
      {
         const MIDIPacket* pkt = &pkt_list_->packet[i];

         std::lock_guard<std::mutex> lock{pimpl->mutex};

         for(unsigned i = 0; i < pkt->length; ++i)
         {
             pimpl->fifo.push(pkt->data[i]);
         }
      }
   }

   MIDIEndpointRef       source{};
   MIDIClientRef         client{};
   MIDIPortRef           port{};
   bool                  connected{false};
   std::mutex            mutex{};
   STB::Fifo<uint8_t,10> fifo;
};


Interface::Interface(::MIDI::Instrument& instrument_)
   : ::MIDI::Interface(instrument_)
{
   pimpl = new Pimpl(/* device*/ 0);
}

Interface::~Interface()
{
   delete pimpl;
}

bool Interface::connected() const
{
   return pimpl->connected;
}

bool Interface::empty() const
{
   std::lock_guard<std::mutex> lock{pimpl->mutex};

   return pimpl->fifo.empty();
}

uint8_t Interface::rx()
{
   std::lock_guard<std::mutex> lock{pimpl->mutex};

   uint8_t byte = pimpl->fifo.back();
   pimpl->fifo.pop();
   return byte;
}


} // namespace MIDI

} // namespace PLT
