/*------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
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
//----------------------------------------------------------------------------*/

//! \brief tiny C library implementation

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*signal_handler_t)(int);

#define SIG_DFL   (signal_handler_t)0
#define SIG_ERR   (signal_handler_t)1
#define SIG_IGN   (signal_handler_t)2

#define SIGABRT   int(0)
#define SIGFPE    int(1)
#define SIGILL    int(2)
#define SIGINT    int(3)
#define SIGSEGV   int(4)
#define SIGTERM   int(5)

signal_handler_t signal(int, signal_handler_t func);

int rais(int signal);

#ifdef __cplusplus
}
#endif
