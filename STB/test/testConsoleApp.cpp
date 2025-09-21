//-------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "STB/ConsoleApp.h"
#include "STB/Option.h"

#define  PROGRAM         "HelloWorld"
#define  DESCRIPTION     "Confidence test for development environment"
#define  LINK            "https://github.com/AnotherJohnH/Platform"
#define  AUTHOR          "John D. Haughton"
#define  COPYRIGHT_YEAR  "2017"

class MyApp : public STB::ConsoleApp
{
private:
   STB::Option<bool>  debug{'d', "debug", "Enable debug", false};

   // TODO remove this override if not required
   virtual void showExtraHelp() override
   {
      printf("Some extra help text.\n");
   }

   virtual int startConsoleApp() override
   {
      // TODO Body of application goes here

      printf("Hello, world!\n");

      if(debug)
      {
         printf("HERE %s:%u\n", __FILE__, __LINE__);
      }

      return 0;
   }

public:
   MyApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   MyApp(argc, argv);
}

