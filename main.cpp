#include "nbench/nbench.hpp"

#include <memory>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>

using namespace nbench;

BENCHMARK("finding 42 in a set")
    .types<std::set<int>,
           std::unordered_set<int>>()
    .range(1e6, 1e8) = [](auto& loop)
{
    auto v = loop.type();

    for (std::size_t i = 0; i < loop.number(); i++)
    {
        v.emplace(i);
    }

    while (loop)
    {
        auto c = v.find(42) != v.end();
        escape(&c);
    }
};

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
