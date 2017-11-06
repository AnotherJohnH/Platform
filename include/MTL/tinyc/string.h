/*------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
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

//! \file string.h
//! \brief tiny C library implementation

#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

extern char*  strcpy(char*, const char*);
extern char*  strncpy(char*, const char*, size_t);
extern char*  strcat(char*, const char*);
extern int    strcmp(const char*, const char*);
extern int    strncmp(const char*, const char*, size_t);
extern size_t strlen(const char*);

extern char*  strchr(const char*, char);
extern char*  strrchr(const char*, char);

extern void*  memcpy(void*, const void*, size_t);
extern void*  memset(void*, int, size_t);

#ifdef __cplusplus
}
#endif

#endif // STRING_H
