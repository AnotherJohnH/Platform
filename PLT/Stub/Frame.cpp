//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Stub Frame implementation

#include "PLT/Frame.h"

namespace PLT {

Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   pimpl = nullptr;
}

Frame::~Frame()
{
}

void* Frame::getHandle() const
{
   return nullptr;
}

uint32_t Frame::getId() const
{
   return 0;
}

void Frame::setTitle(const char* title_)
{
}

void Frame::setFlags(uint32_t flags_)
{  
}  

void Frame::setVisible(bool visible_)
{
}

void Frame::resize(unsigned width_, unsigned height_)
{
}

void Frame::refresh()
{
}

void Frame::setGenerator(Generator* generator_)
{
}

} // namespace PLT
