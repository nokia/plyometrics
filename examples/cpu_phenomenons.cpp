#include "nbench/nbench.hpp"
#include <atomic>
#include <thread>

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
            nbench::escape(&v[idx]);
        }
    }
};

template<std::size_t CacheSize>
struct not_exactly_independent_values
{
    alignas(64) std::atomic<int> a;
    alignas(CacheSize) int b;
};

BENCHMARK("false sharing").range(1, 64).types<not_exactly_independent_values<1>, not_exactly_independent_values<64>>() = [](auto& loop)
{
    auto data = loop.type();
    std::atomic<bool> running(true);

    std::vector<std::thread> threads;
    for (auto i = 0u; i < loop.number(); i++)
        threads.emplace_back([&] { while(running.load()) nbench::use(data.a.load()); });

    while (loop)
    {
        for (int i = 0; i < 100; i++)
            nbench::use(data.b++);
    }

    running.store(false);

    for (auto& t : threads)
        t.join();
};

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
