//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#ifndef GUI_WIDGET_H
#define GUI_WIDGET_H

#include <cstdint>

#include "GUI/Canvas.h"
#include "GUI/Layout.h"
#include "STB/Tree.h"

namespace GUI {


//! Generic Widget base class
class Widget : public Layout, public STB::Tree<Widget>
{
private:
   bool auto_delete{false};

protected:
   static const unsigned EVENT_PRIVATE = 0xFFFFFFFF;
   static const unsigned EVENT_REDRAW  = EVENT_PRIVATE - 1;
   static const unsigned EVENT_FOCUS   = EVENT_PRIVATE - 2;

   Colour      fg_colour{FOREGROUND};
   Colour      bg_colour{FACE};
   const Font* font{nullptr};

private:
   //! Recursively determine the minimum size of this item and this items children
   void layoutSizeShrink()
   {
      unsigned width  = 0;
      unsigned height = 0;

      for(Widget* child = children; child; child = child->next)
      {
         child->layoutSizeShrink();

         if(row)
         {
            width += child->size.x;
            if(child != children) width += gap;

            if(child->size.y > height)
            {
               height = child->size.y;
            }
         }
         else
         {
            if(child->size.x > width)
            {
               width = child->size.x;
            }

            height += child->size.y;
            if(child != children) height += gap;
         }
      }

      if(horz_fit != FIX)
      {
         size.x = width + top_left.x + btm_right.x;
      }

      if(vert_fit != FIX)
      {
         size.y = height + top_left.y + btm_right.y;
      }

      eventResize();
   }

   //! Recursively resize this item and this items children
   void layoutSizeExpand()
   {
      unsigned n     = 0;
      unsigned total = 0;

      for(Widget* child = children; child; child = child->next)
      {
         if(row)
         {
            if(child->horz_fit == EXPAND) ++n;
            total += child->size.x;
         }
         else
         {
            if(child->vert_fit == EXPAND) ++n;
            total += child->size.y;
         }
      }

      if(n > 0)
      {
         unsigned width  = 0;
         unsigned height = 0;

         if(row)
         {
            width  = (size.x - top_left.x - btm_right.x - total) / n;
            height = size.y - top_left.y - btm_right.y;
         }
         else
         {
            width  = size.x - top_left.x - btm_right.x;
            height = (size.y - top_left.y - btm_right.y - total) / n;
         }

         for(Widget* child = children; child; child = child->next)
         {
            if(child->horz_fit == EXPAND)
            {
               child->size.x = width;
            }

            if(child->vert_fit == EXPAND)
            {
               child->size.y = height;
            }
         }
      }

      for(Widget* child = children; child; child = child->next)
      {
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
      case LEFT:   x += top_left.x;                            break;
      case CENTER: x += (top_left.x + size.x - btm_right.x)/2; break;
      case RIGHT:  x += size.x - btm_right.x;                  break;
      }

      switch(vert_align)
      {
      case TOP:    y += top_left.y;                            break;
      case CENTER: y += (top_left.y + size.y - btm_right.y)/2; break;
      case BOTTOM: y += size.y - btm_right.y;                  break;
      }

      for(Widget* child = children; child; child = child->next)
      {
         if(row)
         {
            if(child != children) x += gap;
            child->pos.x = x;

            switch(vert_align)
            {
            case TOP:    child->pos.y = y;                     break;
            case CENTER: child->pos.y = y - (child->size.y/2); break;
            case BOTTOM: child->pos.y = y - child->size.y;     break;
            }

            x += child->size.x;
         }
         else
         {
            switch(horz_align)
            {
            case LEFT:   child->pos.x = x;                     break;
            case CENTER: child->pos.x = x - (child->size.x/2); break;
            case RIGHT:  child->pos.x = x - child->size.x;     break;
            }

            if(child != children) y += gap;
            child->pos.y = y;

            y += child->size.y;
         }

         child->layoutPos();
      }
   }

protected:
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

   //! Get default font fro, top level
   virtual const Font* getDefaultFont() const
   {
      return parent ? parent->getDefaultFont() : nullptr;
   }

public:
   Widget(Widget* parent_ = nullptr)
   {
      if(parent_)
      {
         assert(parent_ != this);
         parent_->pushBack(this);
      }

      font = getDefaultFont();
   }

   virtual ~Widget()
   {
      if(auto_delete)
      {
         for(Widget* child = children; child;)
         {
            Widget* next = child->next;
            delete child;
            child = next;
         }
      }
   }

   bool isParentRow() const { return parent ? parent->isRow() : false; }

   void setAutoDelete() { auto_delete = true; }

   void pushBack(Widget* child)
   {
      child->set_parent(this);

      push_back(child);
   }

   void setForegroundColour(Colour colour) { fg_colour = colour; }

   void setBackgroundColour(Colour colour) { bg_colour = colour; }

   void setFont(const Font* font_)
   {
      font = font_ != nullptr ? font_ : getDefaultFont();

      eventSize();
   }

   void show()
   {
      // Find the minimum size for all widgets
      layoutSizeShrink();

      // Resize any expanding widgets
      layoutSizeExpand();

      layoutPos();

      raiseEvent(this, EVENT_REDRAW);
   }


   // Event handlers that may be overriden

   //! Widget has been resized
   virtual void eventResize()
   {
      //  Default do nothing
   }

   //! Update widget size
   virtual void eventSize()
   {
      //  Default do nothing
   }

   //! Draw widget
   virtual void eventDraw(Canvas& canvas)
   {
      //  Default do nothing
   }

   //! Handle mouse button press event
   virtual void eventBtnPress(unsigned x, unsigned y, bool select, bool down)
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
};

} // namespace GUI

#endif
