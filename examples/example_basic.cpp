#include "nbench/nbench.hpp"

#include <memory>

BENCHMARK("allocate by make_shared") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::make_shared<int>(5);
        nbench::escape(p.get());
    }
};

BENCHMARK("allocate by make_unique") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::make_unique<int>(5);
        nbench::escape(p.get());
    }
};

BENCHMARK("allocate by new and shared_ptr") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::shared_ptr<int>(new int(5));
        nbench::escape(p.get());
    }
};

BENCHMARK("allocate by malloc") = [](auto& loop)
{
    while (loop)
    {
        auto* p = reinterpret_cast<int*>(malloc(sizeof(int)));
        *p = 5;
        nbench::escape(p);
        free(p);
    }
};

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}