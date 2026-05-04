#-------------------------------------------------------------------------------
`#' Copyright (c) YEAR AUTHOR
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

cmake_minimum_required(`VERSION' 3.20)

project(APPLICATION
        LANGUAGES CXX C ASM
        `VERSION' VERSION)

if(NOT DEFINED BUILD_TESTING)
    set(BUILD_TESTING OFF CACHE BOOL "Build unit tests")
endif()
include(CTest)

set(HWR_CONFIG blinky)

add_subdirectory(PDK)
add_subdirectory(Source)
