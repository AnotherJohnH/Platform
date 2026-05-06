//-------------------------------------------------------------------------------
// Copyright (c) 2026 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

#include <stddef.h>

#define RAND_MAX 0xFFFFFF

#define EXIT_FAILURE -1
#define EXIT_SUCCESS  0

#ifdef __cplusplus
extern "C" {
#endif

extern int    atoi(const char* str);
extern long   atol(const char* str);
extern double atof(const char* str);

extern long               strtol(  const char* str, const char** endptr, int base);
extern unsigned long      strtoul( const char* str, const char** endptr, int base);
extern long long          strtoll( const char* str, const char** endptr, int base);
extern unsigned long long strtoull(const char* str, const char** endptr, int base);

extern float  strtof(const char* str, const char** endptr);
extern double strtod(const char* str, const char** endptr);

extern int  abs(int);
extern int  rand();
extern void srand(unsigned);

#ifdef __cplusplus
}
#endif
