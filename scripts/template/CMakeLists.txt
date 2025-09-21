#-------------------------------------------------------------------------------
`#'  Copyright (c) YEAR AUTHOR
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(`VERSION' 3.19.1)

project(APPLICATION
        LANGUAGES CXX C ASM
        `VERSION' VERSION)

add_subdirectory(Platform)

add_executable(BINARY Source/BINARY.cpp)

target_link_libraries(BINARY STB PLT)

install(TARGETS BINARY RUNTIME DESTINATION bin)
