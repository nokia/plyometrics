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

}
