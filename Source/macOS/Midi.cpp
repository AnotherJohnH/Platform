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

#include <cstdio>
#include <cstdint>
#include <CoreMIDI/CoreMIDI.h>

#include "PLT/Midi.h"

namespace PLT {

namespace MIDI {


static MIDIClientRef  client;
static MIDIPortRef    port;


static void midiInputCallBack(const MIDIPacketList*  pkt_list,
                              void*                  read_proc_ref_con,
                              void*                  src_conn_ref_con)
{
   In* midi_in = (In*)read_proc_ref_con;

   for(uint32_t i=0; i<pkt_list->numPackets; ++i)
   {
      const MIDIPacket* pkt = &pkt_list->packet[i];

      midi_in->decode((uint8_t*)pkt->data, pkt->length);
   }
}


In::In(unsigned device_index)
{
   OSStatus    status;
   CFStringRef name;

   // Access one of the MIDI sources in the system
   MIDIEndpointRef source = MIDIGetSource(device_index);
   if (source == 0)
   {
      fprintf(stderr, "ERROR - MIDIGetSource() failed\n");
      return;
   }

   // Create a client
   name = CFStringCreateWithCString(nullptr, "PLT_client", kCFStringEncodingASCII);
   status = MIDIClientCreate(name,
                             /* notifyProc */   nullptr,
                             /* notifyRefCon */ nullptr,
                             &client);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIClientCreate() failed\n");
      return;
   }
   CFRelease(name);

   // Create an input port
   name = CFStringCreateWithCString(nullptr, "PLT_port", kCFStringEncodingASCII);
   status = MIDIInputPortCreate(client,
                                name,
                                midiInputCallBack,
                                this,
                                &port);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIInputPortCreate() failed\n");
      return;
   }
   CFRelease(name);

   // Connect a source to the clients input port
   status = MIDIPortConnectSource(port, source, /* connRefCon */ nullptr);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIConnectSource() failed\n");
      return;
   }

   connected = true;
}


In::~In()
{
   // TODO tidy up
}


Out::Out(unsigned device_index)
{
   OSStatus    status;
   CFStringRef name;

   // Access one of the MIDI destinations in the system
   MIDIEndpointRef destination = MIDIGetDestination(device_index);
   if (destination == 0)
   {
      fprintf(stderr, "ERROR - MIDIGetDestination() failed\n");
      return;
   }
}


Out::~Out()
{
   // TODO tidy up
}


} // namespace MIDI

} // namespace PLT

