cmake_minimum_required(VERSION 3.5)

add_executable(example_basic example_basic.cpp)
target_link_libraries(example_basic plyometrics)

add_executable(cpu_phenomenons cpu_phenomenons.cpp)
target_link_libraries(cpu_phenomenons plyometrics pthread)

add_executable(memory_fragmentation memory_fragmentation.cpp)
target_link_libraries(memory_fragmentation plyometrics)

add_executable(compiler_hacks compiler_hacks.cpp)
target_link_libraries(compiler_hacks plyometrics)

add_executable(containers containers.cpp)
target_link_libraries(containers plyometrics)
