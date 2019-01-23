#include "nbench/nbench.hpp"

int factorial(int i)
{
    return i <= 1 ? 1 : factorial(i - 1) * i;
}

BENCHMARK("using variable") = [](auto& loop)
{
    while (loop)
    {
        nbench::use(factorial(1000));
    }
};

BENCHMARK("not using variable") = [](auto& loop)
{
    while (loop)
    {
        factorial(1000);
    }
};

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}