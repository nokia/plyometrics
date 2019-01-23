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

template<std::size_t LineSize>
struct two_aligned_variables
{
    alignas(64) std::atomic<int> a;
    alignas(LineSize) int b;
};

BENCHMARK("false sharing").range(1, 64).types<two_aligned_variables<1>, two_aligned_variables<64>>() = [](auto& loop)
{
    decltype(loop.type()) data;

    std::atomic<bool> running(true);

    std::vector<std::thread> threads;
    for (auto i = 0u; i < loop.number(); i++)
        threads.emplace_back([&] { while(running.load()) nbench::use(data.a++); });

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
