#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(UCL STATIC
            isalnum.cpp
            isalpha.cpp
            islower.cpp
            isupper.cpp
            isdigit.cpp
            isxdigit.cpp
            iscntrl.cpp
            isgraph.cpp
            isspace.cpp
            isblank.cpp
            isprint.cpp
            ispunct.cpp
            tolower.cpp
            toupper.cpp
            memmove.cpp
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

if(BUILD_TESTING)
   add_subdirectory(test)
endif()
