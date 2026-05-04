#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(UCL STATIC
            abs.cpp
            atof.cpp
            atoi.cpp
            atol.cpp
            atoll.cpp
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
            rand.cpp
            sprintf.cpp
            snprintf.cpp
            strcat.cpp
            strcmp.cpp
            strcpy.cpp
            strlen.cpp
            strncmp.cpp
            strncpy.cpp
            strtod.cpp
            strtof.cpp
            strtol.cpp
            strtoll.cpp
            strtoul.cpp
            strtoull.cpp
            PrintF.cpp)

target_include_directories(UCL PUBLIC include)

if(BUILD_TESTING)
   add_subdirectory(test)
endif()
