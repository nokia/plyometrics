cmake_minimum_required(VERSION 3.5)

add_subdirectory(tests)

add_library(nbench INTERFACE)
target_include_directories(nbench INTERFACE .)
target_compile_options(nbench INTERFACE -Wall -Wextra -pedantic)
target_compile_features(nbench INTERFACE cxx_std_17)

add_subdirectory(examples)
