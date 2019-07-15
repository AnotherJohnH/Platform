
set(source  Source/BINARY.cpp)
set(binary  BINARY)
set(app     APPLICATION)
set(version VERSION)

add_compile_options(-O3)

#-------------------------------------------------------------------------------

cmake_minimum_required(`VERSION' 3.11.4)
`include'(Platform/build.cmake NO_POLICY_SCOPE)

#-------------------------------------------------------------------------------

add_executable(${binary} ${source})
target_link_libraries(${binary} ${platform_libs})
