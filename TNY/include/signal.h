//-------------------------------------------------------------------------------
// Copyright (c) 2023 John D. Haughton
// SPDX-License-Identifier: MIT
//-------------------------------------------------------------------------------

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
