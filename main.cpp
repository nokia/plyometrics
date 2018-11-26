#include "nbench/nbench.hpp"
#include "nbench/random.hpp"
#include <memory>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <boost/container/flat_set.hpp>

using namespace nbench;

#define UNIQUE_NAME_IMPL(prefix, counter)  prefix##counter
#define UNIQUE_NAME(prefix, counter) UNIQUE_NAME_IMPL(prefix, counter)

#define BENCHMARK(name) auto UNIQUE_NAME(nbench_benchmark_, __COUNTER__) = benchmark_builder<>{name}

BENCHMARK("make_shared").body([](auto& loop)
{
    while (loop)
    {
        auto p = std::make_shared<int>(5);
        escape(p.get());
    }
});

BENCHMARK("make_unique").body([](auto& loop)
{
    while (loop)
    {
        auto p = std::make_unique<int>(5);
        escape(p.get());
    }
});

BENCHMARK("sorting vector").range(1, 4).body([](auto& loop)
{
    auto data = nbench::random_range(loop.number());
    std::vector<int> v;
    std::copy(data.begin(), data.end(), std::back_inserter(v));

    while (loop)
    {
        std::sort(v.begin(), v.end());
        escape(v.data());
    }
});

BENCHMARK("counting")
    .types<std::vector<int>, std::list<int>>()
    .range(1, 1e6)
    .body([](auto& loop)
{
    auto data = nbench::random_range(loop.number());
    auto v = loop.type();
    std::copy(data.begin(), data.end(), std::back_inserter(v));

    while (loop)
    {
        auto c = std::count(v.begin(), v.end(), 42);
        escape(&c);
    }
});

BENCHMARK("finding 42 in a set")
    .types<std::set<int>,
           std::unordered_set<int>,
           boost::container::flat_set<int>>()
    .range(1e6, 1e8)
    .body([](auto& loop)
{
    auto v = loop.type();

    for (int i = 0; i < loop.number(); i++)
    {
        v.emplace(i);
    }

    while (loop)
    {
        auto c = v.find(42) != v.end();
        escape(&c);
    }
});

int main()
{
    registry::get().run_all();
}
