#include "nbench.hpp"


int main()
{

nbench::benchmark([](nbench::loop& loop)
{
    while (loop)
    {
    }
});

}
