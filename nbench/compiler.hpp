#pragma once

#include "random.hpp"
#include <iostream>

#ifndef __OPTIMIZE__
    #warning it appears that you are building with -O0
#endif

namespace nbench
{

void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
}

template<class T>
void use(T&& v)
{
    asm volatile("" : : "g"(v) : "memory");
}

__attribute__((noinline)) void im_here()
{
    asm volatile("");
}

void clear_cache()
{
    static std::vector<char> memory(1024 * 1024 * 32);

    for (auto& i : memory)
    {
        i = 0;
        escape(&i);
    }
}

struct fragmentized_heap
{
    std::vector<std::unique_ptr<char[]>> _allocated;
};

auto fragmentize_heap()
{
    const auto allocs = 1000000;
    const auto min_size = 1;
    const auto max_size = 1024;

    fragmentized_heap heap;
    fragmentized_heap temporary_heap;

    std::size_t total_size = 0;

    for (auto size : random_range(allocs))
    {
        const auto real_size = min_size + (std::abs(size) % max_size);
        total_size += real_size;
        auto mem = std::make_unique<char[]>(real_size);
        if (size % 2)
            heap._allocated.push_back(std::move(mem));
        else
            temporary_heap._allocated.push_back(std::move(mem));
    }

    std::cerr << "size of fragmentized memory: " << total_size << std::endl;

    return heap;
}

}
