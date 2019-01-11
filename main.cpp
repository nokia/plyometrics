#include "nbench/nbench.hpp"

#include <memory>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>

using namespace nbench;

BENCHMARK("allocate by make_shared") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::make_shared<int>(5);
        escape(p.get());
    }
};

BENCHMARK("allocate by make_unique") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::make_unique<int>(5);
        escape(p.get());
    }
};

BENCHMARK("allocate by new and shared_ptr") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::shared_ptr<int>(new int(5));
        escape(p.get());
    }
};

BENCHMARK("allocate by malloc") = [](auto& loop)
{
    while (loop)
    {
        auto* p = reinterpret_cast<int*>(malloc(sizeof(int)));
        *p = 5;
        escape(p);
        free(p);
    }
};

BENCHMARK("sorting array") = [](auto& loop)
{
    constexpr std::size_t N = 1000;
    auto data = nbench::random_range(N);
    std::array<int, N> array;
    std::copy(data.begin(), data.end(), array.begin());

    while (loop)
    {
        std::sort(array.begin(), array.end());
        //nbench::escape(v.data());
    }
};

BENCHMARK("sorting vector").range(1, 1e8) = [](auto& loop)
{
    auto data = nbench::random_range(loop.number());
    auto v = std::vector<int>{data.begin(), data.end()};

    while (loop)
    {
        std::sort(v.begin(), v.end());
        nbench::escape(v.data());
    }
};

int factorial(int i)
{
    return i <= 1 ? 1 : factorial(i - 1) * i;
}

BENCHMARK("using variable") = [](auto& loop)
{
    while (loop)
    {
        nbench::use(factorial(1000));
    }
};

BENCHMARK("not using variable") = [](auto& loop)
{
    while (loop)
    {
        factorial(1000);
    }
};

BENCHMARK("cache").range(1, 1e9) = [](auto& loop)
{
    auto random = nbench::random_range(10000);
    auto indicies = std::vector<std::size_t>{};

    for (auto r : random)
        indicies.emplace_back(r % loop.number());

    auto v = std::vector<char>(1e9, 0);

    while (loop)
    {
        for (auto idx : indicies)
        {
            v[idx]++;
            escape(&v[idx]);
        }
    }
};

BENCHMARK("counting").types<std::vector<int>, std::list<int>>().range(1, 1e7) = [](auto& loop)
{
    auto data = nbench::random_range(loop.number());
    auto v = loop.type(data.begin(), data.end());

    while (loop)
    {
        auto c = std::count(v.begin(), v.end(), 42);
        escape(&c);
    }
};

BENCHMARK("iterating").types<
    std::vector<int>, std::list<int>,
    std::set<int>, std::unordered_set<int>
>().range(1, 1e7) = [](auto& loop)
{
    auto data = nbench::sequence_range(loop.number());
    auto container = loop.type(data.begin(), data.end());

    while (loop)
    {
        for (auto i : container)
            nbench::use(i);
    }
};

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
