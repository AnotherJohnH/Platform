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


static MIDIClientRef  input_client;
static MIDIPortRef    input_port;

static MIDIClientRef  output_client;
static MIDIPortRef    output_port;


static bool createClient(MIDIClientRef& client)
{
   CFStringRef name   = CFStringCreateWithCString(nullptr, "PLT_client", kCFStringEncodingASCII);
   OSStatus    status = MIDIClientCreate(name,
                             /* notifyProc */   nullptr,
                             /* notifyRefCon */ nullptr,
                             &client);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIClientCreate() failed[%d]\n", status);
      return false;
   }

   CFRelease(name);

   return true;
}


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

   // Access one of the MIDI sources in the system
   MIDIEndpointRef source = MIDIGetSource(device_index);
   if (source == 0)
   {
      fprintf(stderr, "ERROR - MIDIGetSource() failed\n");
      return;
   }

   if (!createClient(input_client)) return;

   // Create an input port
   CFStringRef name = CFStringCreateWithCString(nullptr, "PLT_port", kCFStringEncodingASCII);
   status = MIDIInputPortCreate(input_client,
                                name,
                                midiInputCallBack,
                                this,
                                &input_port);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIInputPortCreate() failed [%d]\n", status);
      return;
   }
   CFRelease(name);

   // Connect a source to the clients input port
   status = MIDIPortConnectSource(input_port, source, /* connRefCon */ nullptr);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIConnectSource() failed [%d]\n", status);
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

   if (!createClient(output_client)) return;

   // Create an output port
   name = CFStringCreateWithCString(nullptr, "PLT_port", kCFStringEncodingASCII);
   status = MIDIOutputPortCreate(output_client, name, &output_port);
   if (status != noErr)
   {
      fprintf(stderr, "ERROR - MIDIoutputPortCreate() failed [%d]\n", status);
      return;
   }
   CFRelease(name);
}


Out::~Out()
{
   // TODO tidy up
}


void Out::messageOut(unsigned length, const uint8_t* data)
{
   // TODO
}


} // namespace MIDI

} // namespace PLT

