#include "nbench.hpp"
#include "nbench/random.hpp"
#include <memory>

void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
}


int main()
{
    nbench::benchmark([](nbench::loop& loop)
    {
        while (loop)
        {
            auto p = std::make_shared<int>(5);
            escape(p.get());
        }
    });

    nbench::benchmark([](nbench::loop& loop)
    {
        while (loop)
        {
            auto p = std::make_unique<int>(5);
            escape(p.get());
        }
    });

    nbench::exponential_benchmark("sorting a std::vector", [](nbench::loop& loop, auto n)
    {
        auto data = nbench::random_range(n);
        std::vector<int> v;
        std::copy(data.begin(), data.end(), std::back_inserter(v));

        while (loop)
        {
            std::sort(v.begin(), v.end());
            escape(v.data());
        }
    });

}
