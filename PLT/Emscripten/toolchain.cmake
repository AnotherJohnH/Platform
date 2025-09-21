#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake toolchain configuration for Emscripten builds

set(CMAKE_SYSTEM_NAME    Generic)

set(CMAKE_C_COMPILER     emcc)
set(CMAKE_CXX_COMPILER   emcc)
set(CMAKE_AR             emar)
set(CMAKE_RANLIB         emranlib)

set(CMAKE_EXECUTABLE_SUFFIX_CXX ".html")

set(PLT_MACHINE embc)
