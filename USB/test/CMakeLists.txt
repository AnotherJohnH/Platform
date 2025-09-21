#-------------------------------------------------------------------------------
# Copyright (c) 2025 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(test_USB
                  testMain.cpp
                  testAudio.cpp)

   target_link_libraries(test_USB STB)

   add_test(NAME test_USB COMMAND test_USB)

endif()
