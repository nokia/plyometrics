#include "nbench/nbench.hpp"
#include "nbench/random.hpp"
#include <memory>
#include <vector>
#include <list>

int main()
{
    using namespace nbench;

    benchmark("make_shared").run([](auto& loop)
    {
        while (loop)
        {
            auto p = std::make_shared<int>(5);
            escape(p.get());
        }
    });

    benchmark("make_unique").run([](auto& loop)
    {
        while (loop)
        {
            auto p = std::make_unique<int>(5);
            escape(p.get());
        }
    });

    benchmark("sorting vector").range(1, 4).run([](auto& loop)
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

    benchmark("counting")
        .types<std::vector<int>, std::list<int>>()
        .range(1, 1e3)
        .run([](auto& loop)
    {
        auto data = nbench::random_range(loop.number());
        auto v = loop.type();
        std::copy(data.begin(), data.end(), std::back_inserter(v));

        while (loop)
        {
            std::count(v.begin(), v.end(), 42);
            escape(&*v.begin());
        }
    });

}
