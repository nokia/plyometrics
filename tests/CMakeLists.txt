cmake_minimum_required(VERSION 3.5)

add_executable(tests options_tests.cpp)
target_include_directories(tests PRIVATE 3rd-party)
