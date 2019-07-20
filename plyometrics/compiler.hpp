#pragma once

#include "ranges.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <sys/utsname.h>

#ifndef __OPTIMIZE__
    #warning it appears that you are building with -O0
#endif

namespace plyometrics
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

struct allocated_chunk
{
    explicit allocated_chunk(std::size_t size)
        : _mem(std::make_unique<char[]>(size))
    {
    }

private:
    std::unique_ptr<char[]> _mem;
};

struct fragmented_heap
{
    void allocate(std::size_t size)
    {
        _allocated.push_back(allocated_chunk(size));
    }

private:
    std::vector<allocated_chunk> _allocated;
};

/**
 * Allocate random memory chunks and free half of them to achieve memory fragmentation.
 */
auto fragmentize_heap()
{
    const auto allocs = 1000000;
    const auto min_size = 1;
    const auto max_size = 1024;

    // half of it will be freed and half returned to the call scope
    // this way a lot of holes should be created
    fragmented_heap heap;
    fragmented_heap temporary_heap;

    std::size_t total_size = 0;

    for (auto size : random_range(allocs))
    {
        const auto real_size = min_size + (std::abs(size) % max_size);
        total_size += real_size;

        if (size % 2)
            heap.allocate(real_size);
        else
            temporary_heap.allocate(real_size);
    }

    return heap;
}

void warn_on_cpu_freq_scaling()
{
    std::size_t cpu_number = 0;

    while (true)
    {
        std::stringstream path;
        path << "/sys/devices/system/cpu/cpu" << cpu_number << "/cpufreq/scaling_governor";
        auto f = std::ifstream(path.str());

        if (!f)
            return;

        std::string governor;
        f >> governor;
        if (governor != "performance")
            std::cerr << "cpu" << cpu_number << " is running on '" << governor << "' governor" << std::endl;

        cpu_number++;
    }
}

std::string cpu_model()
{
    utsname buf;
    uname(&buf);
    return buf.machine;
}

}
