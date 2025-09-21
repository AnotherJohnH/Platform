//-------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "STB/Tree.h"

#include "GUI/Canvas.h"
#include "GUI/Layout.h"

namespace GUI {

//! Generic Widget base class
class Widget : public Layout, public STB::Tree<Widget>
{
public:
   Widget(Widget* parent_ = nullptr)
   {
      setParent(parent_);
   }

   //! Is this widget in a row container
   bool isParentRow() const { return parent ? parent->isRow() : false; }

   //! Allocate this widgets parent
   void setParent(Widget* parent_)
   {
      if (parent_ != nullptr)
      {
         assert(parent_ != this);
         parent_->pushBack(this);
      }
   }

   //! Add a child widget
   void pushBack(Widget* child)
   {
      child->set_parent(this);

      push_back(child);
   }

   void layout()
   {
      // Find the minimum size for all widgets
      layoutSizeShrink();

      // Resize any expanding widgets
      layoutSizeExpand();

      eventResize();

      // Calculate widget positions
      layoutPos();

      raiseEvent(this, EVENT_REDRAW);
   }

   //! 
   virtual bool wantsHover() const
   {
      return false;
   }

   void redraw(Canvas& canvas)
   {
      static signed level = 0;

      eventDraw(canvas);

      level++;

      for(Widget* child = children; child; child = child->next)
      {
         child->redraw(canvas);
      }

      if(--level == 0)
      {
         canvas.refresh();
      }
   }

   // Event handlers that may be overriden

   //! Widget is about to be layed out
   virtual void eventLayout()
   {
      //  Default do nothing
   }

   //! Widget has been resized
   virtual void eventResize()
   {
      //  Default do nothing
   }

   //! Draw widget
   virtual void eventDraw(Canvas& canvas)
   {
      //  Default do nothing
   }

   //! Handle mouse button press event
   virtual void eventBtnPress(signed x, signed y, bool select, bool down)
   {
      //  Default float up to top level
      if(parent)
      {
         parent->eventBtnPress(x, y, select, down);
      }
   }

   //! Handle mouse move event
   virtual void eventPtrMove(unsigned x, unsigned y)
   {
      //  Default do nothing
   }

   //! Handle button press event
   virtual void eventKeyPress(uint8_t key, bool down)
   {
      //  Default do nothing
   }

protected:
   static const unsigned EVENT_PRIVATE = 0xFFFFFFFF;
   static const unsigned EVENT_REDRAW  = EVENT_PRIVATE - 1;
   static const unsigned EVENT_FOCUS   = EVENT_PRIVATE - 2;

   virtual ~Widget() = default;

   Widget* find(unsigned x_, unsigned y_)
   {
      if(!isHit(x_, y_)) return nullptr;

      for(Widget* child = children; child; child = child->next)
      {
         Widget* hit = child->find(x_, y_);
         if(hit)
         {
            return hit;
         }
      }

      return this;
   }

   //! Raise an event up towards the top level
   virtual void raiseEvent(Widget* source_, unsigned code_)
   {
      //  Default float up
      if(parent)
      {
         parent->raiseEvent(source_, code_);
      }
   }

   //! Check if pointer hovering over a Widget
   virtual bool isHover(const Widget* that) const
   {
      return parent ? parent->isHover(that) : false;
   }

   //! Get default font from top level
   virtual const Font* getDefaultFont() const
   {
      return parent ? parent->getDefaultFont() : nullptr;
   }

private:
   //! Recursively determine the minimum size of this item and this items children
   void layoutSizeShrink()
   {
      Vector area{0, 0};

      for(Widget* child = children; child != nullptr; child = child->next)
      {
         child->layoutSizeShrink();

         child->eventLayout();

         if(row)
         {
            area.x += child->size.x;
            area.y =  std::max(child->size.y, area.y);
         }
         else
         {
            area.x =  std::max(child->size.x, area.x);
            area.y += child->size.y;
         }

         if (child != children)
         {
            if(row)
            {
               area.x += gap;
            }
            else
            {
               area.y += gap;
            }
         }
      }

      setChildrenArea(area);
   }

   //! Recursively resize this item and this items children
   void layoutSizeExpand()
   {
      unsigned n     = 0;
      unsigned total = 0;

      for(Widget* child = children; child != nullptr; child = child->next)
      {
         if(row)
         {
            if(child->horz_fit == Fit::EXPAND) ++n;
            total += child->size.x;
         }
         else
         {
            if(child->vert_fit == Fit::EXPAND) ++n;
            total += child->size.y;
         }
      }

      Vector area;
      getChildrenArea(area);

      Vector expanding_child_size;

      if(n > 0)
      {
         if(row)
         {
            area.x = (area.x - total) / n;
         }
         else
         {
            area.y = (area.y - total) / n;
         }
      }

      for(Widget* child = children; child != nullptr; child = child->next)
      {
         if(child->horz_fit == Fit::EXPAND)
         {
            child->size.x = area.x;
         }

         if(child->vert_fit == Fit::EXPAND)
         {
            child->size.y = area.y;
         }

         child->layoutSizeExpand();
      }
   }

   //! Recursively determine position of this item and this items children
   void layoutPos()
   {
      unsigned x = pos.x;
      unsigned y = pos.y;

      switch(horz_align)
      {
      case Align::LEFT:   x += top_left.x;                            break;
      case Align::CENTER: x += (top_left.x + size.x - btm_right.x)/2; break;
      case Align::RIGHT:  x += size.x - btm_right.x;                  break;
      }

      switch(vert_align)
      {
      case Align::TOP:    y += top_left.y;                            break;
      case Align::CENTER: y += (top_left.y + size.y - btm_right.y)/2; break;
      case Align::BOTTOM: y += size.y - btm_right.y;                  break;
      }

      for(Widget* child = children; child; child = child->next)
      {
         if(row)
         {
            if(child != children) x += gap;
            child->pos.x = x;

            switch(vert_align)
            {
            case Align::TOP:    child->pos.y = y;                     break;
            case Align::CENTER: child->pos.y = y - (child->size.y/2); break;
            case Align::BOTTOM: child->pos.y = y - child->size.y;     break;
            }

            x += child->size.x;
         }
         else
         {
            switch(horz_align)
            {
            case Align::LEFT:   child->pos.x = x;                     break;
            case Align::CENTER: child->pos.x = x - (child->size.x/2); break;
            case Align::RIGHT:  child->pos.x = x - child->size.x;     break;
            }

            if(child != children) y += gap;
            child->pos.y = y;

            y += child->size.y;
         }

         child->layoutPos();
      }
   }
};

} // namespace GUI
