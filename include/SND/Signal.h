//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

// Audio signal classes
//
//   Signal       - A signal value
//   SignalOut    - Base class for signal sources
//   SignalConst  - A simple signal source with a fixed value
//   SignalIn     - A signal sink
//   SignalJunc   - A signal fan-out junction
//   SignalPort   - A signal interconnect


#ifndef SND_SIGNAL_H
#define SND_SIGNAL_H

#include <cassert>
#include <cstdint>


namespace SND {


//! A signal value
using Signal = double;


class SignalPort;


struct SignalSink
{
   SignalSink(unsigned sample_freq_) : sample_freq(sample_freq_) {}

   uint32_t  sequence{0};
   unsigned  sample_freq;
};



//! A signal source
class SignalOut
{
public:
   //! Compute and return the next sample
   virtual Signal output()
   {
      return 0;
   }

   //! Backwards initialise signal path from the final sink
   virtual void init(const SignalSink* sink)
   {
   }
};


//! A simple signal source with a fixed value
class SignalConst : public SignalOut
{
public:
   SignalConst(Signal initial=0) : value(initial) {}

   void operator=(Signal value_)
   {
      value = value_;
   }

   virtual Signal output() override
   {
      return value;
   }

private:
   Signal  value{0};
};


//! An audio signal sink
class SignalIn
{
public:
   SignalIn(Signal level = 0)
      : fixed(level)
   {}

   bool isConnected() const { return source != &fixed; }

   //! Connect this input to an output
   void linkOutput(SignalOut* source_)
   {
      assert(!isConnected());
      source = source_;
   }

   //! Change the fixed value for this input
   void operator=(Signal level)
   {
      assert(!isConnected());
      fixed = level;
   }

   //! Direct connection of an output to an input
   inline void operator=(SignalOut& out)
   {
      linkOutput(&out);
   }

   //! Connect a port to an input
   inline void operator=(SignalPort& port);

   //! Change the fixed value for this input
   operator Signal() const
   {
      return source->output();
   }

   //! Backwards initialise signal path from the final sink
   void init(const SignalSink* sink)
   {
      source->init(sink);
   }

private:
   SignalConst  fixed;
   SignalOut*   source{&fixed};
};


//! Fan out junction for signals
class SignalJunc : public SignalOut
{
public:
   SignalIn  in;

   template<typename ARG>
   SignalOut& operator()(ARG& arg)
   {
      in = arg;
      return *this;
   }

private:
   uint32_t           value_sequence{0xFFFFffff};
   Signal             value{0};
   const SignalSink*  sink{nullptr};

   virtual Signal output() override
   {
      if (sink->sequence != value_sequence)
      {
         // Get a fresh inout
         value_sequence = sink->sequence;
         value = in;
      }

      return value;
   }

   virtual void init(const SignalSink* sink_) override
   {
      if (sink == nullptr)
      {
         sink = sink_;

         in.init(sink);
      }
   }
};


//! Interconnect for signals
class SignalPort
{
private:
   SignalOut*   source{nullptr};
   SignalIn*    sink{nullptr};

public:
   SignalPort& port()
   {
      return *this;
   }

   //! Connect this port to a signal source
   void linkOutput(SignalOut* source_)
   {
      assert(source == nullptr); // Not already connected

      source = source_;
      if (sink)
      {
         sink->linkOutput(source);
      }
   }

   //! Connect this port to a signal sink
   void linkInput(SignalIn* sink_)
   {
      assert(sink == nullptr); // Not already connected

      sink = sink_;
      if (source)
      {
         sink->linkOutput(source);
      }
   }

   //! Connect this port to another port
   void linkPort(SignalPort* sink_port)
   {
      assert(sink == nullptr); // Not already connected

      if (source)
      {
         sink_port->linkOutput(source);
      }
      else if (sink_port->sink)
      {
         linkInput(sink_port->sink);
      }
   }

   //! Connect a port to another port
   inline void operator=(SignalPort& port)
   {
      port.linkPort(this);
   }

   //! Connect an output to another port
   inline void operator=(SignalOut& out)
   {
      linkOutput(&out);
   }
};


inline void SignalIn::operator=(SignalPort& port)
{
   port.linkInput(this);
}


} // namespace SND

#endif

