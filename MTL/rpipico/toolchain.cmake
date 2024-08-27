#-------------------------------------------------------------------------------
#  Copyright (c) 2019 John D. Haughton
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

# cmake configuration for RaspberryPi Pico builds

set(PLT_PREFIX  arm-none-eabi-)
set(PLT_MACHINE armv6m)

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(CMAKE_ASM_FLAGS "-mcpu=cortex-m0")

set(CMAKE_C_FLAGS "-DPLT_NCONSOLE -DPLT_SMALL_MEMORY -DMTL_RP2040 \
-mcpu=cortex-m0 -mthumb -mfloat-abi=soft \
-fno-common -fno-builtin -fmessage-length=0 \
-fno-default-inline -fno-exceptions -ffunction-sections -fdata-sections")

set(CMAKE_CXX_FLAGS "-DNO_RTTI -fno-rtti ${CMAKE_C_FLAGS}")

set(PLT_LD_FLAGS  "--static -T${CMAKE_SOURCE_DIR}/Platform/MTL/${PLT_TARGET}/script.ld")

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_SYSTEM_NAME         Generic)
set(CMAKE_SYSTEM_PROCESSOR    ${PLT_MACHINE})

set(CMAKE_ASM_COMPILER        ${PLT_PREFIX}as)
set(CMAKE_C_COMPILER          ${PLT_PREFIX}gcc)
set(CMAKE_CXX_COMPILER        ${PLT_PREFIX}g++)
set(CMAKE_AR                  ${PLT_PREFIX}ar)
set(CMAKE_RANLIB              ${PLT_PREFIX}ranlib)
set(CMAKE_OBJCOPY             ${PLT_PREFIX}objcopy)
set(CMAKE_OBJDUMP             ${PLT_PREFIX}objdump)
set(CMAKE_SIZE                ${PLT_PREFIX}size)

set(CMAKE_C_LINK_EXECUTABLE
    "${PLT_PREFIX}ld ${PLT_LD_FLAGS} <OBJECTS> -o <TARGET>.elf <LINK_LIBRARIES>; \
     ${CMAKE_OBJCOPY} -O ihex <TARGET>.elf <TARGET>.hex; \
     ${CMAKE_SOURCE_DIR}/Platform/MTL/util/ihexToUF2.py <TARGET>.hex -f 0xE48BFF57 -o<TARGET>.uf2; \
     ${CMAKE_SOURCE_DIR}/Platform/scripts/objdump.py -b ${CMAKE_OBJDUMP} <TARGET>.elf; \
     ${CMAKE_SIZE} <TARGET>.elf")

set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Prevent compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE     STATIC_LIBRARY)
