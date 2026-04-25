//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

#pragma once

typedef unsigned size_t;

#ifdef __cplusplus
extern "C" {
#endif

extern char*  strcpy(char*, const char*);
extern char*  strncpy(char*, const char*, size_t);
extern char*  strcat(char*, const char*);
extern int    strcmp(const char*, const char*);
extern int    strncmp(const char*, const char*, size_t);
extern size_t strlen(const char*);

extern char*  strchr(const char*, int);
extern char*  strrchr(const char*, int);

extern void*  memcpy(void*, const void*, size_t);
extern void*  memset(void*, int, size_t);
extern void*  memmove(void* dest, const void* src, size_t n);

#ifdef __cplusplus
}
#endif
