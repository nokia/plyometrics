#include "nbench/nbench.hpp"
#include "nbench/random.hpp"
#include <memory>

int main()
{
    using namespace nbench;

    nbench::benchmark([](auto& loop)
    {
        while (loop)
        {
            auto p = std::make_shared<int>(5);
            escape(p.get());
        }
    });

    nbench::benchmark([](auto& loop)
    {
        while (loop)
        {
            auto p = std::make_unique<int>(5);
            escape(p.get());
        }
    });

    nbench::exponential_benchmark("sorting a std::vector", [](auto& loop)
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

}
