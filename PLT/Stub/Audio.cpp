//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub Audio implementation

#include "PLT/Audio.h"

namespace PLT {

namespace Audio {

IOBase::IOBase(unsigned freq_,
               Format   format_,
               unsigned channels_,
               unsigned samples_,
               bool     input_)
{
    handle = 0;
}

IOBase::~IOBase() {}

bool IOBase::setEnable(bool enable_)
{
   return false;
}

} // namespace Audio

} // namespace PLT
