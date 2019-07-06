#include "plyometrics/plyometrics.hpp"

#include <memory>

NBENCHMARK(allocate_by_make_shared)
{
    while (loop)
    {
        auto p = std::make_shared<int>(5);
        plyometrics::escape(p.get());
    }
}

NBENCHMARK(allocate_by_make_unique)
{
    while (loop)
    {
        auto p = std::make_unique<int>(5);
        plyometrics::escape(p.get());
    }
}

NBENCHMARK(allocate_by_new_and_shared_ptr)
{
    while (loop)
    {
        auto p = std::shared_ptr<int>(new int(5));
        plyometrics::escape(p.get());
    }
}

NBENCHMARK(allocate_by_malloc)
{
    while (loop)
    {
        auto* p = reinterpret_cast<int*>(malloc(sizeof(int)));
        *p = 5;
        plyometrics::escape(p);
        free(p);
    }
}

__attribute__((noinline)) void foo_value(std::shared_ptr<int> i)
{
    plyometrics::use(*i);
}

NBENCHMARK(pass_shared_ptr_by_value)
{
    auto p = std::make_shared<int>(5);

    while (loop)
    {
        foo_value(p);
        foo_value(p);
        foo_value(p);
        foo_value(p);
    }
}

__attribute__((noinline)) void foo_reference(const std::shared_ptr<int>& i)
{
    plyometrics::use(*i);
}

NBENCHMARK(pass_shared_ptr_by_reference)
{
    auto p = std::make_shared<int>(5);

    while (loop)
    {
        foo_reference(p);
        foo_reference(p);
        foo_reference(p);
        foo_reference(p);
    }
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
