#include "plyometrics/plyometrics.hpp"

#include <vector>
#include <list>
#include <set>
#include <unordered_set>

NBENCHMARK_P(iterating_over_various_containers_with_fragmented_memory,
             plyometrics::spec::with_types<std::vector<int>, std::list<int>, std::set<int>, std::unordered_set<int>>
                         ::with_range<1, 512>)
{
    auto frag = plyometrics::fragmentize_heap();
    auto data = plyometrics::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
        for (auto i : container)
            plyometrics::use(i);
}

NBENCHMARK_P(iterating_over_various_containers,
             plyometrics::spec::with_types<std::vector<int>, std::list<int>, std::set<int>, std::unordered_set<int>>
                         ::with_range<1, 512>)
{
    auto data = plyometrics::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
        for (auto i : container)
            plyometrics::use(i);
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
