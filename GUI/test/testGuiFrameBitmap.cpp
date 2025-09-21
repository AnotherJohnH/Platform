//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "GUI/Frame.h"
#include "GUI/Bitmap.h"

#include "PLT/Event.h"

int main(int argc, const char* argv[])
{
   GUI::Frame  frame("GUI::Frame Bitmap Test", 600, 600);
   GUI::Bitmap bitmap("Test.png");

   frame.clear(STB::BLACK);
   frame.drawImage(bitmap, 10, 10);
   frame.refresh();

   return PLT::Event::eventLoop();
}
