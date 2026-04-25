#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(UCL STATIC
            memset.cpp
            memcpy.cpp
            strcat.cpp
            strcmp.cpp
            strcpy.cpp
            strlen.cpp
            strncmp.cpp
            strncpy.cpp)

target_include_directories(UCL PUBLIC .)
