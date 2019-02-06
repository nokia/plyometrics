/*
 * find_in_container.cpp
 * Copyright (C) 2019 bsadowsk <bsadowsk@bsadowsk>
 *
 * Distributed under terms of the MIT license.
 */

#include "nbench/nbench.hpp"

#include <map>
#include <unordered_map>
#include <list>

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

NBENCHMARK_P(constructing_sequence_containers, nbench::spec::with_types<std::vector<int>, std::list<int>>::with_range<1, 128>)
{
    auto sequence_data = nbench::sequence_range(loop.number());

    while (loop)
        nbench::use(loop.type(sequence_data.begin(), sequence_data.end()));
}

NBENCHMARK_P(iterating_though_sequence_containers, nbench::spec::with_types<std::vector<int>, std::list<int>>::with_range<1, 128>)
{
    auto sequence_data = nbench::sequence_range(loop.number());
    auto container = loop.type(sequence_data.begin(), sequence_data.end());

    while (loop)
        for(auto i : container)
            nbench::use(i);
}

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
