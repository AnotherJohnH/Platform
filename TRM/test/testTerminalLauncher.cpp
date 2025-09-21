//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "TRM/Launcher.h"

#define  PROGRAM         "MyApp"
#define  DESCRIPTION     "A test"
#define  LINK            "https://github.com/AnotherJohnH/Platform"
#define  AUTHOR          "John D. Haughton"
#define  COPYRIGHT_YEAR  "2017"

class MyApp : public TRM::Launcher
{
private:
   // TODO remove this override if not required
   virtual void showExtraHelp() override
   {
      printf("Some extra help text.\n");
   }

   virtual int startTerminalLauncher(const char* file) override
   {
      // TODO Body of application goes here

      curses.addstr("Hello, world!\n");

      while(curses.getch() != -1) {}

      return 0;
   }

public:
   MyApp(int argc, const char* argv[])
      : TRM::Launcher(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR,
                      "<file>", "launcher.cfg")
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   MyApp(argc, argv);
}

