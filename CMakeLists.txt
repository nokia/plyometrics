cmake_minimum_required(VERSION 3.5)

add_subdirectory(tests)

add_library(plyometrics INTERFACE)
target_include_directories(plyometrics INTERFACE .)
target_compile_options(plyometrics INTERFACE -Wall -Wextra -pedantic)
target_compile_features(plyometrics INTERFACE cxx_std_17)

add_subdirectory(examples)
add_subdirectory(memtrace)
