add_library(memtrace SHARED main.cpp)
target_link_libraries(memtrace dl)
