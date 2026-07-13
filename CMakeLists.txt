cmake_minimum_required(VERSION 3.16)
project(c4 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)

option(C4_BUILD_EXAMPLES "Build c4 examples" ON)
option(C4_BUILD_TESTS "Build c4 tests" ON)

add_subdirectory(src)

if(C4_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(C4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
