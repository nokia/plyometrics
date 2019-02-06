/*
 * find_in_container.cpp
 * Copyright (C) 2019 bsadowsk <bsadowsk@bsadowsk>
 *
 * Distributed under terms of the MIT license.
 */

#include "nbench/nbench.hpp"
#include <map>
#include <unordered_map>


BENCHMARK("lookup").types<
    std::map<int, int>, std::unordered_map<int,int>
>().range(1, 64) = [](auto& loop)
{
    auto sequence_data = nbench::sequence_range(loop.number());
    auto container = loop.type();
    for(auto i:sequence_data)
        container.emplace(i,i);

    while (loop)
        for(auto i : sequence_data)
            nbench::use(container[i]);
};

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
