//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include "TRM/App.h"

#define  PROGRAM         "MyApp"
#define  DESCRIPTION     "A test"
#define  LINK            "https://github.com/AnotherJohnH/Platform"
#define  AUTHOR          "John D. Haughton"
#define  COPYRIGHT_YEAR  "2017"

class MyApp : public TRM::App
{
private:
   // TODO remove this override if not required
   virtual void showExtraHelp() override
   {
      printf("Some extra help text.\n");
   }

   virtual int startTerminalApp(TRM::Device& device) override
   {
      // TODO Body of application goes here

      device.write("Hello, world!\n");

      char ch;
      while(device.read(&ch, 1) != -1) {}

      return 0;
   }

public:
   MyApp(int argc, const char* argv[])
      : TRM::App(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   MyApp(argc, argv);
}

