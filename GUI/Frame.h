//-------------------------------------------------------------------------------
// Copyright (c) 2015 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief Platform abstraction interface for 2D rendering on a frame buffer

#pragma once

#include "PLT/Frame.h"

#include "GUI/Canvas.h"

namespace GUI {

const uint32_t RESIZABLE   = PLT::Frame::RESIZABLE;
const uint32_t NO_BORDER   = PLT::Frame::NO_BORDER;
const uint32_t FULL_SCREEN = PLT::Frame::FULL_SCREEN;

//! A frame buffer with 2D rendering primitive support
class Frame : public Canvas
{
public:
   Frame(const char* title_, unsigned width_ = 0, unsigned height_ = 0, uint32_t flags_ = 0)
      : GUI::Canvas(width_, height_)
      , frame(title_, width_, height_, flags_)
   {
   }

   PLT::Frame* getFramePtr() { return &frame; }

   uint32_t getId() const { return frame.getId(); }

   virtual void show() { frame.setVisible(true); }

   void hide() { frame.setVisible(false); }

private:
   // Implement GUI::Canvas

   virtual STB::Colour canvasGetPixel(int32_t x, int32_t y) const override
   {
      return frame.getPixel(x, y);
   }

   virtual const PLT::Image* canvasGetImage() const override { return &frame; }

   virtual void canvasResize(uint32_t width, uint32_t height) override
   {
      frame.resize(width, height);
   }

   virtual void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) override
   {
      frame.refresh();
   }

   virtual void canvasClear(STB::Colour colour) override
   {
      frame.clear(colour);
   }

   virtual void canvasPoint(STB::Colour colour, int32_t x, int32_t y) override
   {
      frame.point(colour, x, y);
   }

   virtual void canvasSpan(STB::Colour colour, int32_t x1, int32_t y, int32_t x2) override
   {
      frame.span(colour, x1, y, x2);
   }

   virtual void canvasBlit(const Canvas& source,
                           int32_t  x,     int32_t  y,
                           uint32_t w,     uint32_t h,
                           int32_t  src_x, int32_t  src_y) override
   {
      const PLT::Image* image = source.getImage();

      if(image->getHandle() == nullptr)
      {
         Canvas::canvasBlit(source, x, y, w, h, src_x, src_y);
      }
      else
      {
         frame.blit(*image, x, y, w, h, src_x, src_y);
      }
   }

   PLT::Frame frame;
};

} // namespace GUI
