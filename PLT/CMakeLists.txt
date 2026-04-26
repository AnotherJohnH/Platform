#-------------------------------------------------------------------------------
# Copyright (c) 2023 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(PDK_NATIVE)
   add_subdirectory(${PDK_TARGET})

   if(BUILD_TESTING)
      add_subdirectory(test)
   endif()
else()
   add_subdirectory(Stub)
endif()

target_compile_definitions(PLT PUBLIC PDK_VERSION="${PROJECT_VERSION}")
target_compile_definitions(PLT PUBLIC PDK_TARGET="${PDK_TARGET}")
target_compile_definitions(PLT PUBLIC PDK_MACHINE="${PDK_MACHINE}")
target_compile_definitions(PLT PUBLIC PDK_COMMIT="${PDK_COMMIT}")

target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Wall>")
target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:C>:-Werror>")

target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Wall>")
target_compile_options(PLT PUBLIC "$<$<COMPILE_LANGUAGE:CXX>:-Werror>")

target_include_directories(PLT PUBLIC ${PDK_TARGET} ..)

