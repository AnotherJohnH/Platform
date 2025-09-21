#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------


if(${PLT_NATIVE})

add_library(GUI STATIC
            Bitmap/BitmapPNG.cpp
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

add_subdirectory(test)

endif()
