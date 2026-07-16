cmake_minimum_required(VERSION 3.16)
project(c4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)

option(C4_BUILD_SAMPLES "Build c4 samples" ON)
option(C4_BUILD_TESTS "Build c4 tests" ON)

add_subdirectory(src)

if(C4_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if(C4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
