cmake_minimum_required(VERSION 3.21)
project(Platform_Independent_Utils_For_Embedded_Development C CXX)

# Optimization level for different compile options##############################
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "O2 optimization for release build")
    add_compile_options(-O2 -flto)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "O2 optimization for release build, debug info included")
    add_compile_options(-O2 -ggdb -flto)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os -flto)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -ggdb)
endif ()
# End of optimization level for different compile options#######################

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

include_directories(PIUFED)
add_subdirectory(PIUFED)

# Add Catch2
include_directories(Catch2/src)
add_subdirectory(Catch2)

add_executable(TestRun testMain.cpp vtimer_test.cpp margined_linear_test.cpp sim_uart_test.cpp)
target_link_libraries(TestRun PRIVATE PIUFED Catch2::Catch2WithMain)
