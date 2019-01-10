#pragma once

#ifndef __OPTIMIZE__
    #warning it appears that you are building with -O0
#endif

namespace nbench
{

void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
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

}
