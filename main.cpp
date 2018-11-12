#include "nbench.hpp"
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


    nbench::exponential_benchmark([](nbench::loop& loop, auto i)
    {
        auto data = nbench::generate_random_data(i);
        //std::cout << data.size() << std::endl;
        //for (auto i : data)
        //{
        //    std::cout << i << " ";
        //}
        //std::cout << std::endl;

        while (loop)
        {
        auto data = nbench::generate_random_data(i);
            auto p = std::make_unique<int>(5);
            escape(p.get());
        }
    });

}
