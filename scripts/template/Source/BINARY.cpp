//-------------------------------------------------------------------------------
// Copyright (c) YEAR AUTHOR
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#include <cstdio>

#include "STB/ConsoleApp.h"


`#'define  PROGRAM         "APPLICATION"
`#'define  `DESCRIPTION'     "DESCRIPTION"
`#'define  `LINK'            "LINK"
`#'define  `AUTHOR'          "AUTHOR"
`#'define  COPYRIGHT_YEAR  "YEAR"


class APPLICATION`App' : public STB::ConsoleApp
{
public:
   APPLICATION`App'()
      : ConsoleApp(PROGRAM, `DESCRIPTION', `LINK', `AUTHOR', COPYRIGHT_YEAR)
   {
   }

private:
   virtual int startConsoleApp() override
   {
      printf("Hello, world!\n");
      return 0;
   }
};


int main(int argc, const char* argv[])
{
   return APPLICATION`App'().parseArgsAndStart(argc, argv);
}
