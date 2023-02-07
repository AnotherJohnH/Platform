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

#ifndef SND_SIGNAL_OP_H
#define SND_SIGNAL_OP_H

#include <cassert>
#include <cmath>
#include <cstdlib>

#include "SND/Signal.h"

namespace SND {


//! Base class for unary ops
class UnaryOp : public SignalOut
{
public:
   SignalIn in;

   template <typename ARG>
   SignalOut& operator()(ARG&& arg)
   {
      in = arg;
      return *this;
   }

protected:
   UnaryOp() {}

private:
   virtual void init(const SignalSink* sink)
   {
      in.init(sink);
   }
};


//! Base class for unary compomnent
class UnaryComponent : public SignalPort
{
public:
   SignalPort in;

   template <typename ARG>
   SignalPort& operator()(ARG&& arg)
   {
      in = arg;
      return *this;
   }
};


//! Base class for binary ops
class BinaryOp : public SignalOut
{
public:
   SignalIn in1, in2;

   template <typename ARG1, typename ARG2>
   SignalOut& operator()(ARG1&& arg1, ARG2&& arg2)
   {
      in1 = arg1;
      in2 = arg2;
      return *this;
   }

protected:
   BinaryOp() {}

private:
   virtual void init(const SignalSink* sink)
   {
      in1.init(sink);
      in2.init(sink);
   }
};


//! Base class for binary compomnent
class BinaryComponent : public SignalPort
{
public:
   SignalPort in1;
   SignalPort in2;

   template <typename ARG1, typename ARG2>
   SignalPort& operator()(ARG1&& arg1, ARG2&& arg2)
   {
      in1 = arg1;
      in2 = arg1;
      return *this;
   }
};


//! Base class for N-ary ops
template <unsigned N>
class NaryOp : public SignalOut
{
public:
   SignalIn in[N];

   SignalOut& operator()() { return *this; }

   template <typename FIRST_ARG, typename... ARGS>
   SignalOut& operator()(FIRST_ARG& first_arg, ARGS&&... args)
   {
      const size_t n = sizeof...(ARGS);
      assert(n < N);
      in[N - n - 1] = first_arg;
      return operator()(args...);
   }

protected:
   NaryOp() {}

private:
   virtual void init(const SignalSink* sink)
   {
      for(unsigned i = 0; i < N; i++)
      {
         in[i].init(sink);
      }
   }
};


//! Add two signals
class Add : public BinaryOp
{
   virtual Signal output() override { return in1 + in2; }
};


//! Subtract two signals
class Sub : public BinaryOp
{
   virtual Signal output() override { return in1 - in2; }
};


//! Multiply two signals and re-scale the output
class Mul : public BinaryOp
{
   virtual Signal output() override { return in1 * in2; }
};


//! Negate the input
class Neg : public UnaryOp
{
   virtual Signal output() override { return -in; }
};


//! Output is the absolute value of the input
class Abs : public UnaryOp
{
   virtual Signal output() override { return fabs(in); }
};


//! Sum of N inputs with re-scaled output
template <unsigned N>
class Sum : public NaryOp<N>
{
   virtual Signal output() override
   {
      Signal sample = 0;

      for(unsigned i = 0; i < N; i++)
      {
         sample += this->in[i];
      }

      return sample / N;
   }
};


} // namespace SND

#endif
