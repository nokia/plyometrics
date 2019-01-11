#include "nbench/nbench.hpp"

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

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
