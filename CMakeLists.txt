cmake_minimum_required(VERSION 3.5)

add_executable(example main.cpp)
target_compile_options(example PRIVATE -Wall -Wextra -pedantic)
set_property(TARGET example PROPERTY CXX_STANDARD 17)
