#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(PLT STATIC
            ../Stub/Audio.cpp
            ../Stub/Event.cpp
            ../Stub/Frame.cpp
            ../Stub/Image.cpp
            ../Stub/NullBitmap.cpp
            ../Stub/Midi.cpp
            ../Stub/Sounder.cpp
            ../Stub/Yield.cpp
            ../Stub/Info.cpp
            ../Stub/File.cpp)

target_link_libraries(PLT PUBLIC TNY MTL)
