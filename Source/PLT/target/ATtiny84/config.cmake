#-------------------------------------------------------------------------------
#  Copyright (c) 2020 John D. Haughton
# 
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
# 
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
# 
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#  SOFTWARE.
#-------------------------------------------------------------------------------

# cmake configuration for Atmel ATtiny84 builds

set(PLT_prefix  avr-)
set(PLT_machine avr24)
set(PLT_chip    ATtiny84)

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(PLT_asm_flags "-mmcu=attiny84")

set(PLT_c_flags   "-DNCONSOLE -DSMALL_MEMORY \
                   -mmcu=attiny84 \
                   -fno-common -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections")

set(PLT_cxx_flags "-std=c++11 -fno-rtti")

set(PLT_ld_flags  "--static -T${CMAKE_SOURCE_DIR}/Platform/Source/PLT/target/${PLT_TARGET}/script.ld")

include_directories(Platform/include/MTL/board/${PLT_TARGET})

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_ASM_COMPILER        ${PLT_prefix}as)
set(CMAKE_C_COMPILER          ${PLT_prefix}gcc)
set(CMAKE_CXX_COMPILER        ${PLT_prefix}g++)
set(CMAKE_AR                  ${PLT_prefix}ar)
set(CMAKE_RANLIB              ${PLT_prefix}ranlib)
set(CMAKE_OBJCOPY             ${PLT_prefix}objcopy)
set(CMAKE_OBJDUMP             ${PLT_prefix}objdump)
set(CMAKE_SIZE                ${PLT_prefix}size)

set(CMAKE_EXECUTABLE_SUFFIX   .elf)

set(CMAKE_C_LINK_EXECUTABLE
    "${PLT_prefix}ld ${PLT_ld_flags} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>; \
     ${CMAKE_OBJCOPY} -O binary <TARGET> <TARGET>.bin; \
     ${CMAKE_OBJDUMP} -d <TARGET>; \
     ${CMAKE_SIZE} <TARGET>")

set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

#-------------------------------------------------------------------------------
# Configuration for libPLT.a

set(PLT_source
    Platform/Source/PLT/target/${PLT_TARGET}/startup.s
    Platform/Source/PLT/target/${PLT_TARGET}/platform.cpp
    Platform/Source/PLT/Stub/Audio.cpp
    Platform/Source/PLT/Stub/Event.cpp
    Platform/Source/PLT/Stub/Frame.cpp
    Platform/Source/PLT/Stub/Image.cpp
    Platform/Source/PLT/Stub/Bitmap.cpp
    Platform/Source/PLT/Stub/Midi.cpp
    Platform/Source/PLT/Stub/Sounder.cpp
    Platform/Source/PLT/Stub/Yield.cpp
    Platform/Source/PLT/Stub/Info.cpp
    Platform/Source/PLT/Stub/File.cpp)

set(PLT_libs tinyc) # ${gcc_lib}/libgcc.a)
