//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include "GUI/Canvas.h"
#include "PLT/Event.h"

#include "Rack.h"

template <unsigned WIDTH, unsigned HEIGHT, unsigned SCALE = 4, unsigned BORDER = 0>
class Panel
   : public GUI::Canvas
   , public Rack::Panel
{
public:
   Panel(Rack::EventHandler* handler_ = nullptr)
      : GUI::Canvas(WIDTH, HEIGHT)
      , Rack::Panel(BORDER * 2 + WIDTH * SCALE, BORDER * 2 + HEIGHT * SCALE, handler_)
   {
   }

   static unsigned getWidth() { return WIDTH; }
   static unsigned getHeight() { return HEIGHT; }

private:
   void canvasClear(STB::Colour colour) override
   {
      for(unsigned y = 0; y < (BORDER * 2 + HEIGHT * SCALE); y++)
      { 
         rack->span(colour, ox, oy + y, ox + BORDER * 2 + WIDTH * SCALE);
      }
   }

   void canvasPoint(STB::Colour colour, int32_t x_, int32_t y_) override
   {
      for(unsigned i = 0; i < SCALE; ++i)
      {
         rack->span(colour, ox + BORDER + x_ * SCALE, oy + BORDER + y_ * SCALE + i,
                    ox + BORDER + (x_ + 1) * SCALE);
      }
   }

   void canvasRefresh(int32_t x1, int32_t y1, int32_t x2, int32_t y2) override
   {
      rack->refresh();
   }
};
