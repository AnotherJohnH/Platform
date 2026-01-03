//-------------------------------------------------------------------------------
// Copyright (c) 2024 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "PLT/Event.h"
#include "PLT/Frame.h"

class Rack
{
public:
    class EventHandler
    {
    public:
       virtual void eventClick(unsigned x_, unsigned y_, bool down_) {}
       virtual void eventKey(unsigned code_, bool down_) {}
    };

    class Panel
    {
       friend class Rack;

    public:
       Panel(unsigned width_, unsigned height_, EventHandler* handler_ = nullptr)
          : w(width_)
          , h(height_)
          , handler(handler_)
       {
          Rack::alloc(this);
       }

       void eventPoll()
       {
          rack->eventPoll();
       }

    protected:
       unsigned      w, h;
       unsigned      ox, oy;
       EventHandler* handler{nullptr};
       Rack*         rack{};
    };

    static void alloc(Panel* panel_)
    {
       static Rack* rack{};

       if (rack == nullptr)
       {
          rack = new Rack(panel_);

          panel_->ox = 0;
          panel_->oy = 0;
       }
       else
       {
          panel_->ox = rack->frame.getWidth();
          panel_->oy = 0;

          rack->extend(panel_);
       }

       panel_->rack = rack;
    }

    void span(STB::Colour colour, unsigned x1, unsigned y, unsigned x2)
    {
       frame.span(colour, x1, y, x2);
    }

    void refresh()
    {
       frame.refresh();
    }

    void eventPoll()
    {
       PLT::Event::Message event;
       PLT::Event::poll(event);

       switch(event.type)
       {
       case PLT::Event::QUIT:
          exit(0);
          break;

       case PLT::Event::BUTTON_DOWN:
       case PLT::Event::BUTTON_UP:
          for(auto& panel : panel_list)
          {
             if ((event.x >= panel->ox) && (event.x < (panel->ox + panel->w)) &&
                 (event.y >= panel->oy) && (event.y < (panel->oy + panel->h)))
             {
                if (panel->handler != nullptr)
                {
                   panel->handler->eventClick(event.x - panel->ox, event.y - panel->oy,
                                              event.type == PLT::Event::BUTTON_DOWN);
                }
                break;
             }
          }
          break;

       case PLT::Event::KEY_DOWN:
       case PLT::Event::KEY_UP:
          for(auto& panel : panel_list)
          {
             if (panel->handler != nullptr)
             {
                panel->handler->eventKey(event.code,
                                         event.type == PLT::Event::KEY_DOWN);
             }
          }
          break;

       default:
          break;
       }
    }

private:
    Rack(Panel* panel_)
       : frame("Hardware - Virtual Component Rack", panel_->w, panel_->h)
       , max_height(panel_->h)
    {
       panel_list.push_back(panel_);
    }

    void extend(Panel* panel_)
    {
       if (panel_->h > max_height)
          max_height = panel_->h;

       frame.resize(frame.getWidth() + panel_->w, max_height);

       panel_list.push_back(panel_);
    }

    PLT::Frame          frame;
    unsigned            max_height{0};
    std::vector<Panel*> panel_list{};
};
