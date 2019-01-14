#include "nbench/nbench.hpp"

#include <vector>
#include <list>
#include <set>
#include <unordered_set>

BENCHMARK("iterating").types<
    std::vector<int>, std::list<int>,
    std::set<int>, std::unordered_set<int>
>().range(1e6, 1e7) = [](auto& loop)
{
    // keep it in this scope
    auto fragmentized = nbench::fragmentize_heap();

    auto data = nbench::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
        for (auto i : container)
            nbench::use(i);
};

BENCHMARK("iterating - fragmented memory").types<
    std::vector<int>, std::list<int>,
    std::set<int>, std::unordered_set<int>
>().range(1e6, 1e7) = [](auto& loop)
{
    auto data = nbench::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
        for (auto i : container)
            nbench::use(i);
};

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
