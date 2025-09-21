//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "GUI/GUI.h"

class PopUp : public GUI::PopUpWindow
{
public:
   PopUp(GUI::App* app)
      : GUI::PopUpWindow(app, "", 0, 0, GUI::NO_BORDER)
   {
      gap = 0;
   }

private:
   GUI::MenuButton one{  this, 1, "One"};
   GUI::MenuButton two{  this, 2, "Two"};
   GUI::MenuButton three{this, 3, "Three"};

   virtual void eventDraw(Canvas& canvas) override
   {
      signed size_x = getSize().x;
      signed size_y = getSize().y;

      canvas.fillRect(GUI::FACE, 0, 0, size_x - 1, size_y - 1);
      canvas.drawLine(GUI::HILIGHT, 0, 0, size_x - 1, 0);
      canvas.drawLine(GUI::HILIGHT, 0, 0, 0, size_y - 1);
      canvas.drawLine(GUI::SHADOW,  size_x - 1, size_y - 1, size_x - 1, 0);
      canvas.drawLine(GUI::SHADOW,  size_x - 1, size_y - 1, 0, size_y - 1);
   }

   virtual void appEvent(Widget* widget_, unsigned code_) override
   {
      printf("PopUp raiseEvent(%u)\n", code_);
      hide();
   }
};

class GuiApp : public GUI::App
{
public:
   GuiApp()
      : GUI::App("GUI test", &GUI::font_teletext18)
   {
      setTimer(7, 2000);
   }

private:
   PopUp popup{this};

   GUI::Text        text2{this, "Hello, world!"};
   GUI::Button      button{this, 1};
   GUI::TextButton  txt_btn{this, 2, "0123456789"};
   GUI::TextButton  btn_a{this, 'A', "Cancel"};
   GUI::TextButton  btn_b{this, 'B', "OK"};
   GUI::TickBox     tick{this, 3};
   GUI::TextTickBox text_tick{this, 3, "Do you agree?"};
   GUI::Field<16>   field{this, 4, ""};
   GUI::Slider      slider{this, 5};
   GUI::ScrollBar   scroll_bar{this, 6};

   virtual void appEvent(Widget* widget_, unsigned code_) override
   {
      printf("raiseEvent(%u)\n", code_);

      if(code_ == 'B') popup.show();
   }
};

int main( int argc, char *argv[] )
{
   GuiApp().eventLoop();
}
