#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(testConsoleApp testConsoleApp.cpp)
   target_link_libraries(testConsoleApp PLT)

   add_executable(test_STB
                  testMain.cpp
                  testFAT16.cpp
                  testBitArray.cpp
                  testEndian.cpp
                  testHeap.cpp
                  testLicense.cpp
                  testList.cpp)

   target_link_libraries(test_STB STB)

   add_test(NAME test_STB COMMAND test_STB)

endif()
