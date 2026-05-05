#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(UCL STATIC
            ctype/isalnum.cpp
            ctype/isalpha.cpp
            ctype/islower.cpp
            ctype/isupper.cpp
            ctype/isdigit.cpp
            ctype/isxdigit.cpp
            ctype/iscntrl.cpp
            ctype/isgraph.cpp
            ctype/isspace.cpp
            ctype/isblank.cpp
            ctype/isprint.cpp
            ctype/ispunct.cpp
            ctype/tolower.cpp
            ctype/toupper.cpp
            string/atof.cpp
            string/atoi.cpp
            string/atol.cpp
            string/atoll.cpp
            string/memmove.cpp
            string/memset.cpp
            string/memcpy.cpp
            string/strcat.cpp
            string/strcmp.cpp
            string/strcpy.cpp
            string/strlen.cpp
            string/strncmp.cpp
            string/strncpy.cpp
            string/strtod.cpp
            string/strtof.cpp
            string/strtol.cpp
            string/strtoll.cpp
            string/strtoul.cpp
            string/strtoull.cpp
            stdio/sprintf.cpp
            stdio/snprintf.cpp
            stdio/PrintF.cpp
            stdlib/abs.cpp
            stdlib/rand.cpp)

target_include_directories(UCL PUBLIC include)

if(BUILD_TESTING)
   add_subdirectory(test)
endif()

target_link_libraries(UCL PUBLIC TGT)
