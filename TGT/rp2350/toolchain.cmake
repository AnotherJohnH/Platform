#-------------------------------------------------------------------------------
# Copyright (c) 2024 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

# cmake configuration for RaspberryPi Pico builds

set(PDK_TARGET  rp2350)
set(PDK_PREFIX  arm-none-eabi-)
set(PDK_MACHINE armv8m)

#-------------------------------------------------------------------------------
# Special compile flags for this platform

set(CMAKE_ASM_FLAGS "-mcpu=cortex-m33")

set(CMAKE_C_FLAGS "-DPDK_NCONSOLE -DPDK_SMALL_MEMORY \
-mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb \
-fno-common -fno-builtin -fmessage-length=0 \
-fno-default-inline -fno-exceptions -ffunction-sections -fdata-sections \
-I${CMAKE_SOURCE_DIR}/PDK/UCL/include \
-I${CMAKE_SOURCE_DIR}/PDK/TNY/include")

set(CMAKE_CXX_FLAGS "-DNO_RTTI -fno-rtti -Wno-volatile ${CMAKE_C_FLAGS}")

set(PDK_LD_FLAGS  "--static -T${CMAKE_SOURCE_DIR}/PDK/TGT/${PDK_TARGET}/script.ld")

#-------------------------------------------------------------------------------
# Configure the cmake tools

set(CMAKE_SYSTEM_NAME         Generic)
set(CMAKE_SYSTEM_PROCESSOR    ${PDK_MACHINE})

set(CMAKE_ASM_COMPILER        ${PDK_PREFIX}as)
set(CMAKE_C_COMPILER          ${PDK_PREFIX}gcc)
set(CMAKE_CXX_COMPILER        ${PDK_PREFIX}g++)
set(CMAKE_AR                  ${PDK_PREFIX}ar)
set(CMAKE_RANLIB              ${PDK_PREFIX}ranlib)
set(CMAKE_OBJCOPY             ${PDK_PREFIX}objcopy)
set(CMAKE_OBJDUMP             ${PDK_PREFIX}objdump)
set(CMAKE_SIZE                ${PDK_PREFIX}size)

set(CMAKE_C_LINK_EXECUTABLE
    "${PDK_PREFIX}ld ${PDK_LD_FLAGS} <OBJECTS> -o <TARGET> <LINK_LIBRARIES>; \
     ${CMAKE_OBJCOPY} -O ihex <TARGET> <TARGET>.hx; \
     ${CMAKE_SOURCE_DIR}/PDK/TGT/util/ihexToUF2.py <TARGET>.hx -f 0xE48BFF59 -o<TARGET>.uf2; \
     ${CMAKE_SOURCE_DIR}/PDK/TGT/util/objdump.py -b ${CMAKE_OBJDUMP} <TARGET>; \
     ${CMAKE_SIZE} <TARGET>")

set(CMAKE_CXX_LINK_EXECUTABLE ${CMAKE_C_LINK_EXECUTABLE})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Prevent compiler sanity check when cross-compiling.
set(CMAKE_TRY_COMPILE_TARGET_TYPE     STATIC_LIBRARY)
