cmake_minimum_required(VERSION 3.5)

add_library(nbench INTERFACE)
target_include_directories(nbench INTERFACE .)
target_compile_options(nbench INTERFACE -Wall -Wextra -pedantic)
target_compile_features(nbench INTERFACE cxx_std_17)

add_executable(example main.cpp)
target_link_libraries(example nbench)

add_executable(example_basic examples/example_basic.cpp)
target_link_libraries(example_basic nbench)

add_executable(cpu_phenomenons examples/cpu_phenomenons.cpp)
target_link_libraries(cpu_phenomenons nbench)

add_executable(memory_fragmentation examples/memory_fragmentation.cpp)
target_link_libraries(memory_fragmentation nbench)

add_executable(compiler_hacks examples/compiler_hacks.cpp)
target_link_libraries(compiler_hacks nbench)
