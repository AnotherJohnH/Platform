#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

add_library(PLT STATIC
            ../Stub/Audio.cpp
            Event.cpp
            Frame.cpp
            Image.cpp
            ../Stub/NullBitmap.cpp
            PALVideo.cpp
            ../Stub/Midi.cpp
            ../Stub/Sounder.cpp
            ../Stub/Yield.cpp
            ../Stub/Info.cpp
            ../Stub/File.cpp)

target_link_libraries(PLT PUBLIC TNY MTL)
