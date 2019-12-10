Memory fragmentation
--------------------
There are two common ways to allocate memory in C++. _Stack_ and _heap_ allocation. There are number of differences between them but we want to focus mainly on one. _Heap_ allocated memory can be deallocated in different order to the allocations. This means that holes are created in the used memory. Good allocator algorithms will of course try to reuse this holes to conserve the memory but this will also mean that consecutive allocations might get fragmented.

`memory_fragmentation.cpp` shows the impact of the memory fragmentation on some STL containers. Because allocated elements might be far apart of each other, the program will probably generate a lot of _data cache misses_.

Not all of STL containers will be impacted, `std::vector` for example, keeps the elements in contiguous sequence. _Open addressing_ hash tables will also most likely behave better than their counterparts.
