//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once


static const int TCSANOW = 0;

static const int ICANON  = 1;
static const int ECHO    = 2;


struct termios
{
   unsigned c_lflag;
};


#ifdef __cplusplus
extern "C" {
#endif

int tcgetattr(int fd, struct termios*);
int tcsetattr(int fd, int actions, const struct termios*);

#ifdef __cplusplus
}
#endif
