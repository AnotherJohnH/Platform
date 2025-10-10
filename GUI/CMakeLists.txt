#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(GUI STATIC
            Bitmap/BitmapStub.cpp
            Font/FontLED11.cpp
            Font/FontLED22.cpp
            Font/FontNew8.cpp
            Font/FontTeletext12.cpp
            Font/FontTeletext15.cpp
            Font/FontTeletext18.cpp
            Font/FontTeletext9.cpp)

target_include_directories(GUI PUBLIC ..)

target_link_libraries(GUI PUBLIC STB PLT)

if(${PLT_NATIVE})
    target_sources(GUI PRIVATE Bitmap/BitmapPNG.cpp)
    add_subdirectory(test)
endif()
