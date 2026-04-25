#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(UCL STATIC
            isalpha.cpp
            isdigit.cpp
            islower.cpp
            isspace.cpp
            isupper.cpp
            memset.cpp
            memcpy.cpp
            sprintf.cpp
            snprintf.cpp
            strcat.cpp
            strcmp.cpp
            strcpy.cpp
            strlen.cpp
            strncmp.cpp
            strncpy.cpp
            vsprintf.cpp
            vsnprintf.cpp)

target_include_directories(UCL PUBLIC .)
