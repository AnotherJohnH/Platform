//-------------------------------------------------------------------------------
// Copyright (c) 2021 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

// Frame implementation for mictobitV2

#include "PLT/Frame.h"

#include "MTL/PALVideo.h"

static const unsigned VIDEO_SYNC = MTL::PIN_PAD_15;
static const unsigned VIDEO_LUM  = MTL::PIN_PAD_14;

// Teletext 40x25 cells of 12x20 pixel chars
static const unsigned WIDTH  = 480;
static const unsigned HEIGHT = 500;

static MTL::PALVideo<VIDEO_SYNC,VIDEO_LUM> video(WIDTH, HEIGHT);
static uint8_t                             frame[WIDTH * HEIGHT / 8];

PAL_VIDEO_ATTACH_IRQ(video);

namespace PLT {

class Frame::Impl
{
public:
   Impl() = default;

   void setGenerator(Generator* generator_)
   {
      generator = generator_;

      generator->getConfig(width, height, interlace, char_height);

      words_per_line = width / 32;

      video.setSize(width, height);
      video.setFramePtr(frame);
   }

   void resize(unsigned width_, unsigned height_)
   {
      width     = width_;
      height    = height_;
      interlace = height > 256;

      words_per_line = width / 32;

      video.setSize(width, height);
   }

   void refresh()
   {
      if (generator == nullptr) return;

      generator->startField(field);

      uint8_t* raw = frame;

      if (interlace)
      {
         if (field)
         {
            raw += words_per_line * 4;
         }

         for(unsigned line = field; line < height; line += 2)
         {
            renderLine(line, raw);

            raw += words_per_line * 4;
         }

         field ^= 1;
      }
      else
      {
         for(unsigned line = 0; line < height; line++)
         {
            renderLine(line, raw);
         }
      }
   }

private:
   PLT::Frame::Generator* generator{};
   uint16_t               width, height;
   uint16_t               words_per_line;
   bool                   interlace;
   uint8_t                char_height;
   uint8_t                field{0};

   void renderLine(unsigned line, uint8_t*& raw)
   {
      unsigned char_row  = line / char_height;
      unsigned char_line = line % char_height;

      generator->startLine(char_row, char_line);

      for(unsigned word = 0; word < words_per_line; ++word)
      {
         uint32_t pixels = generator->getPixelData_1BPP();

         *raw++ = uint8_t(pixels >> 24);
         *raw++ = uint8_t(pixels >> 16);
         *raw++ = uint8_t(pixels >> 8);
         *raw++ = uint8_t(pixels);
      }
   }
};

//----------------------------------------------------------------------

Frame::Frame(const char* title_, unsigned width_, unsigned height_, uint32_t flags_)
   : Image(width_, height_)
{
   static Frame::Impl impl;

   pimpl = &impl;
}

Frame::~Frame() { }

void* Frame::getHandle() const { return nullptr; }

uint32_t Frame::getId() const { return 0; }

void Frame::setTitle(const char* title_) {}

void Frame::setFlags(uint32_t flags_) {}

void Frame::setVisible(bool visible_) {}

void Frame::resize(unsigned width_, unsigned height_)
{
   pimpl->resize(width_, height_);
}

void Frame::refresh()
{
   pimpl->refresh();
}

void Frame::setGenerator(Generator* generator_)
{
   pimpl->setGenerator(generator_);
}

} // namespace PLT
