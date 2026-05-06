#-------------------------------------------------------------------------------
# Copyright (c) 2026 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PDK_NATIVE})

   add_library(TGT STATIC empty.cpp)

else()

   add_subdirectory(${PDK_TARGET})

   add_library(TGT STATIC
               gcc_global_obj.cpp
               gcc_guard.cpp
               gcc_loader.cpp
               gcc_pure_virtual.cpp)

   # This warning seems broken in gcc 10 and later
   target_compile_options(TGT PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Wno-stringop-overflow>")
   target_compile_options(TGT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Wno-stringop-overflow>")

   # Legacy, are these still reuired?
   target_compile_definitions(TGT PUBLIC PDK_NCONSOLE=1)
   target_compile_definitions(TGT PUBLIC PDK_SMALL_MEMORY=1)

   separate_arguments(c_flags NATIVE_COMMAND "${CMAKE_C_FLAGS}")
   execute_process(COMMAND ${CMAKE_C_COMPILER} ${c_flags} -print-file-name=libgcc.a
                   OUTPUT_VARIABLE gcc_lib
                   OUTPUT_STRIP_TRAILING_WHITESPACE)

   target_link_libraries(TGT PUBLIC ${gcc_lib})

endif()

string(TOUPPER "${PDK_TARGET}" PDK_TARGET_UPPER)

target_compile_definitions(TGT PUBLIC PDK_VERSION="${PROJECT_VERSION}")
target_compile_definitions(TGT PUBLIC PDK_COMMIT="${PDK_COMMIT}")
target_compile_definitions(TGT PUBLIC PDK_TARGET="${PDK_TARGET}")
target_compile_definitions(TGT PUBLIC PDK_MACHINE="${PDK_MACHINE}")
target_compile_definitions(TGT PUBLIC PDK_${PDK_TARGET_UPPER}=1)
