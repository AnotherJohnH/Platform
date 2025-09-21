//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "GUI/GUI.h"

int main(int argc, const char* argv[])
{
   GUI::App app("GUI::App test", &GUI::font_teletext18, 320, 256);

   return app.eventLoop();
}
