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

#include <cinttypes>
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
void Option<bool>::showDefault() const
{}

template <>
const char* Option<bool>::getValueDescription() const
{
   return nullptr;
}


template <>
bool Option<int32_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = int32_t(strtol(arg_, nullptr, 0));
   return true;
}

template <> void Option<int32_t>::showDefault() const
{
   printf(" [%" PRId32 "]", value);
}

template <>
const char* Option<int32_t>::getValueDescription() const
{
   return "<integer>";
}


template <>
bool Option<int64_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = strtol(arg_, nullptr, 0);
   return true;
}

template <> void Option<int64_t>::showDefault() const
{
   printf(" [%" PRId64, value);
}

template <>
const char* Option<int64_t>::getValueDescription() const
{
   return "<int64>";
}


template <>
bool Option<uint8_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = uint8_t(strtoul(arg_, nullptr, 0));
   return true;
}

template <> void Option<uint8_t>::showDefault() const
{
   printf(" [0x%x]", value);
}

template <>
const char* Option<uint8_t>::getValueDescription() const
{
   return "<uint8>";
}


template <>
bool Option<uint16_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = uint16_t(strtoul(arg_, nullptr, 0));
   return true;
}

template <> void Option<uint16_t>::showDefault() const
{
   printf(" [0x%x]", value);
}

template <>
const char* Option<uint16_t>::getValueDescription() const
{
   return "<uint16>";
}


template <>
bool Option<uint32_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = uint32_t(strtoul(arg_, nullptr, 0));
   return true;
}

template <> void Option<uint32_t>::showDefault() const
{
   printf(" [%" PRIu32 "]", value);
}

template <>
const char* Option<uint32_t>::getValueDescription() const
{
   return "<unsigned>";
}


template <>
bool Option<uint64_t>::set(const char* arg_)
{
   if(arg_ != nullptr) value = strtoul(arg_, nullptr, 0);
   return true;
}

template <> void Option<uint64_t>::showDefault() const
{
   printf(" [0x%" PRIx64, value);
}

template <>
const char* Option<uint64_t>::getValueDescription() const
{
   return "<uint64>";
}


template <>
bool Option<double>::set(const char* arg_)
{
   if(arg_ != nullptr) value = atof(arg_);
   return true;
}

template <> void Option<double>::showDefault() const
{
   printf(" [%g]", value);
}

template <>
const char* Option<double>::getValueDescription() const
{
   return "<float>";
}


template <>
bool Option<const char*>::set(const char* arg_)
{
   if(arg_ != nullptr) value = arg_;
   return true;
}

template <>
void Option<const char*>::showDefault() const
{
   if (value != nullptr) printf(" [\"%s\"]", value);
}

template <>
const char* Option<const char*>::getValueDescription() const
{
   return "<string>";
}

} // namespace STB
