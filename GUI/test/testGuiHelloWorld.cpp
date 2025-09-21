//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "GUI/GUI.h"

int main(int argc, const char* argv[])
{
   GUI::App  app("Hi", &GUI::font_teletext18);
   GUI::Text text(&app, "Hello, world!");

   return app.eventLoop();
}
