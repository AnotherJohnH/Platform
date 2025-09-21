//-------------------------------------------------------------------------------
// Copyright (c) 2013 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

//! \brief tiny C library implementation

#pragma once

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned long       uint32_t;
typedef unsigned long  long uint64_t;
typedef unsigned long       uintptr_t;

typedef signed char      int8_t;
typedef signed short     int16_t;
typedef signed long      int32_t;
typedef signed long long int64_t;
typedef signed long      intptr_t;

#define UINT8_MAX 255
#define INT8_MAX  127
#define INT8_MIN  -128

#define UINT16_MAX 65535
#define INT16_MAX  32767
#define INT16_MIN  -32768

#define UINT32_MAX 4294967295
#define INT32_MAX  2147483647
#define INT32_MIN  -2147483648
