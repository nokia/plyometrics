#include "nbench/nbench.hpp"
#include "nbench/random.hpp"
#include <memory>
#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <boost/container/flat_set.hpp>

    using namespace nbench;

//auto b = benchmark_adder<>{registry::register_benchmark}.body([](auto& loop)
//    {
//        while (loop)
//        {
//            auto p = std::make_shared<int>(5);
//            escape(p.get());
//        }
//    });
//

int main()
{

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
        .range(1, 1e6)
        .run([](auto& loop)
    {
        auto data = nbench::random_range(loop.number());
        auto v = loop.type();
        std::copy(data.begin(), data.end(), std::back_inserter(v));

        while (loop)
        {
            auto c = std::count(v.begin(), v.end(), 42);
            escape(&c);
        }
    });

    benchmark("finding 42 in a set")
        .types<std::set<int>,
               std::unordered_set<int>,
               boost::container::flat_set<int>>()
        .range(1e6, 1e8)
        .run([](auto& loop)
    {
        auto v = loop.type();

        for (int i = 0; i < loop.number(); i++)
        {
            v.emplace(i);
        }

        while (loop)
        {
            auto c = v.find(42) != v.end();
            escape(&c);
        }
    });

}
