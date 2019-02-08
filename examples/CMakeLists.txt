cmake_minimum_required(VERSION 3.5)

add_executable(example_basic example_basic.cpp)
target_link_libraries(example_basic nbench)

add_executable(cpu_phenomenons cpu_phenomenons.cpp)
target_link_libraries(cpu_phenomenons nbench pthread)

add_executable(memory_fragmentation memory_fragmentation.cpp)
target_link_libraries(memory_fragmentation nbench)

add_executable(compiler_hacks compiler_hacks.cpp)
target_link_libraries(compiler_hacks nbench)

add_executable(look_up_in_map look_up_in_map.cpp)
target_link_libraries(look_up_in_map nbench)
