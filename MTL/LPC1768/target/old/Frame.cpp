//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// \brief mbed LPC1768 Frame implementation

#include "PLT/Frame.h"

#include "MTL/LPC1768/PALVideo.h"
#include "MTL/LPC1768/RAM.h"

#include <cstring>


static MTL::PALVideo  video;
PAL_VIDEO_ATTACH_IRQ(video);

static PLT::Frame::Generator* generator{nullptr};

static void scanCallBack(uint8_t* buffer, uint16_t line)
{
   generator->getRawPixels(buffer, line);
}


namespace PLT {

Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   MTL::RAM  ram;

   pimpl  = nullptr;
   buffer = ram.data();

   if (width_ != 0)
   {
      resize(width_, height_);
   }
}

Frame::~Frame()
{
   // nohing to do here
}

void* Frame::getHandle() const { return nullptr; }

uint32_t Frame::getId() const { return 0; }

void Frame::setTitle(const char* title_) { }

void Frame::setVisible(bool visible_) { }

void Frame::resize(unsigned width_, unsigned height_)
{
   width  = width_;
   height = height_;
   pitch  = ((width_ + 0x1F) & ~0x1F) / 8;

   video.setHorzPos(0);
   video.resize(pitch * 8, height);
   video.setFramePtr(buffer);

   memset(buffer, 0, height * pitch);
}

void Frame::refresh()
{
   // nothing to do here
}

void Frame::setGenerator(Generator* generator_)
{
   generator = generator_;

   video.setScanner(generator != nullptr ? scanCallBack : nullptr);
}

} // namespace PLT
