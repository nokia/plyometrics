cmake_minimum_required(VERSION 3.5)

option(PLYOMETRICS_BUILD_TESTS "build the unit tests" OFF)

if(PLYOMETRICS_BUILD_TESTS)
    add_subdirectory(tests)
endif()

add_library(plyometrics INTERFACE)
target_include_directories(plyometrics INTERFACE .)
target_compile_options(plyometrics INTERFACE -Wall -Wextra -pedantic)

add_subdirectory(examples)
