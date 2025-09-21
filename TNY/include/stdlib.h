//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
extern int   system(const char*);

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
