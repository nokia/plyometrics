#include "plyometrics/plyometrics.hpp"

int factorial(int i)
{
    return i <= 1 ? 1 : factorial(i - 1) * i;
}

NBENCHMARK(using_variable)
{
    while (loop)
    {
        plyometrics::use(factorial(1000));
    }
}

NBENCHMARK(not_using_variable)
{
    while (loop)
    {
        factorial(1000);
    }
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
