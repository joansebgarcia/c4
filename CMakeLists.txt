cmake_minimum_required(VERSION 3.16)
project(h3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)

option(H3_BUILD_SAMPLES "Build h3 samples" ON)
option(H3_BUILD_TESTS "Build h3 tests" ON)

add_subdirectory(src)

if(H3_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if(H3_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
