cmake_minimum_required(VERSION 3.5)

add_executable(nbench_tests options_tests.cpp)
target_include_directories(nbench_tests PRIVATE 3rd-party)
target_link_libraries(nbench_tests nbench)
