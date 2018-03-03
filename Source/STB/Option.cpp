//------------------------------------------------------------------------------
// Copyright (c) 2015-2017 John D. Haughton
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

#include <cstdint>
#include <cstdlib>

#include "STB/Option.h"

namespace STB {

template <>
bool Option<bool>::set(const char* arg_)
{
   value = true;
   return false;
}

template <>
bool Option<int32_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = int32_t(strtol(arg_, nullptr, 0));
   return true;
}

template <>
bool Option<int64_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = strtol(arg_, nullptr, 0);
   return true;
}

template <>
bool Option<uint16_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = uint16_t(strtoul(arg_, nullptr, 0));
   return true;
}

template <>
bool Option<uint32_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = uint32_t(strtoul(arg_, nullptr, 0));
   return true;
}

template <>
bool Option<uint64_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = strtoul(arg_, nullptr, 0);
   return true;
}

template <>
bool Option<double>::set(const char* arg_)
{
   if(arg_ != nullptr) value = atof(arg_);
   return true;
}

template <>
bool Option<const char*>::set(const char* arg_)
{
   if(arg_ != nullptr) value = arg_;
   return true;
}

} // namespace STB
