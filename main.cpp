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


    nbench::exponential_benchmark("some exponential benchmark", [](nbench::loop& loop, auto n)
    {
        auto data = nbench::random_range(n);
        std::vector<int> v;
        std::copy(data.begin(), data.end(), std::back_inserter(v));

        //std::cout << data.size() << std::endl;
        for (auto i : v)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        while (loop)
        {
            auto p = std::make_unique<int>(5);
            escape(p.get());
        }
    });

}
