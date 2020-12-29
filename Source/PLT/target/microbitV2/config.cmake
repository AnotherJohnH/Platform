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

# cmake configuration for micro::bit builds

set(PLT_prefix   arm-none-eabi-)
set(PLT_machine  armv7m)
set(PLT_chip     nRF52)
set(PLT_ram_size 128k)

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(PLT_asm_flags "-mcpu=cortex-m4")

set(PLT_c_flags   "-DNCONSOLE -DSMALL_MEMORY \
                   -mcpu=cortex-m4 -mthumb -mfloat-abi=soft \
                   -fno-common -fno-builtin -fmessage-length=0 \
                   -fno-default-inline -fno-exceptions -ffunction-sections -fdata-sections")

set(PLT_cxx_flags "-DNO_RTTI -std=c++11 -fno-rtti")

set(PLT_ld_flags  "--static -T${PLT_config_dir}/script.ld")

include_directories(Platform/Source/PLT/target/microbitV2)

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_SYSTEM_NAME         Generic)
set(CMAKE_SYSTEM_PROCESSOR    ${PLT_machine})

set(CMAKE_ASM_COMPILER        ${PLT_prefix}as)
set(CMAKE_C_COMPILER          ${PLT_prefix}gcc)
set(CMAKE_CXX_COMPILER        ${PLT_prefix}g++)
set(CMAKE_AR                  ${PLT_prefix}ar)
set(CMAKE_RANLIB              ${PLT_prefix}ranlib)
set(CMAKE_OBJCOPY             ${PLT_prefix}objcopy)
set(CMAKE_OBJDUMP             ${PLT_prefix}objdump)
set(CMAKE_SIZE                ${PLT_prefix}size)

set(CMAKE_EXECUTABLE_SUFFIX   .axf)

set(CMAKE_C_LINK_EXECUTABLE
    "${PLT_prefix}ld ${PLT_ld_flags} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>; \
     ${CMAKE_OBJCOPY} -O ihex <TARGET> <TARGET>.hex; \
     ${PLT_config_dir}/univihex.py -1 ${PLT_config_dir}/stand-alone-error-v1.hex -2 <TARGET>.hex -o <TARGET>-u.hex; \
     ${CMAKE_OBJDUMP} -d <TARGET>; \
     ${CMAKE_SIZE} <TARGET>")

set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Prevent compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE     STATIC_LIBRARY)

#-------------------------------------------------------------------------------
# Configuration for libPLT.a

set(PLT_source
    Platform/Source/PLT/target/microbitV2/startup.s
    Platform/Source/PLT/target/microbitV2/Leds.cpp
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

execute_process(COMMAND ${CMAKE_C_COMPILER} -mcpu=cortex-m4 -print-file-name=libgcc.a OUTPUT_VARIABLE gcc_lib)
string(STRIP ${gcc_lib} gcc_lib)

set(PLT_libs tinyc ${gcc_lib})
