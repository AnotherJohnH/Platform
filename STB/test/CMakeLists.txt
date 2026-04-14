#-------------------------------------------------------------------------------
# Copyright (c) 2019 John D. Haughton
# SPDX-License-Identifier: MIT
#-------------------------------------------------------------------------------

if(${PLT_NATIVE})

   add_executable(testConsoleApp testConsoleApp.cpp)
   target_link_libraries(testConsoleApp PLT)

   add_executable(testSTB
                  testMain.cpp
                  testFAT16.cpp
                  testFixP.cpp
                  testBitArray.cpp
                  testEndian.cpp
                  testHeap.cpp
                  testLicense.cpp
                  testList.cpp
                  testUFixP.cpp)

   target_link_libraries(testSTB STB)

   add_test(NAME testSTB COMMAND testSTB)

endif()
