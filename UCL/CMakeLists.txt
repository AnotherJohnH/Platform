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

target_compile_definitions(UCL PUBLIC PDK_VERSION="${PROJECT_VERSION}")
target_compile_definitions(UCL PUBLIC PDK_COMMIT="${PDK_COMMIT}")
target_compile_definitions(UCL PUBLIC PDK_TARGET="${PDK_TARGET}")
target_compile_definitions(UCL PUBLIC PDK_MACHINE="${PDK_MACHINE}")

if(BUILD_TESTING)
   add_subdirectory(test)
endif()
