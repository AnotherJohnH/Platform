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

//! \brief tiny C library implementation

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RAND_MAX 0xFFFFFF

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

extern void* malloc(size_t);
extern void  free(void*);
extern int   rand(void);
extern void  srand(unsigned);
extern int   abs(int);
extern int   atexit(void (*)(void));
extern void  exit(int);
extern void  abort(void);

extern long               strtol(const char* str, const char** endptr, int base);
extern unsigned long      strtoul(const char* str, const char** endptr, int base);
extern long long          strtoll(const char* str, const char** endptr, int base);
extern unsigned long long strtoull(const char* str, const char** endptr, int base);

extern int       atoi(const char* str);
extern long      atol(const char* str);
extern long long atoll(const char* str);

extern float  strtof(const char* str, const char** endptr, int base);
extern double strtod(const char* str, const char** endptr, int base);

extern double atof(const char* str);

#ifdef __cplusplus
}
#endif
