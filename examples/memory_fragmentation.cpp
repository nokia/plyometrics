#include "nbench/nbench.hpp"

#include <vector>
#include <list>
#include <set>
#include <unordered_set>

NBENCHMARK_P(iterating_over_various_containers_with_fragmented_memory,
             nbench::spec::with_types<std::vector<int>, std::list<int>, std::set<int>, std::unordered_set<int>>
                         ::with_range<1, 512>)
{
    auto frag = nbench::fragmentize_heap();
    auto data = nbench::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
        for (auto i : container)
            nbench::use(i);
}

NBENCHMARK_P(iterating_over_various_containers,
             nbench::spec::with_types<std::vector<int>, std::list<int>, std::set<int>, std::unordered_set<int>>
                         ::with_range<1, 512>)
{
    auto data = nbench::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
        for (auto i : container)
            nbench::use(i);
}

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
