#include <vector>
#include <functional>
#include <chrono>
#include <iostream>

namespace nbench
{

struct loop
{
    operator bool()
    {
        _end = clock::now();
        _iterations++;
        if (_end - _start > std::chrono::seconds{2})
            return false;
        return true;
    }

    auto iteration_time() const
    {
        return (_end - _start) / _iterations;
    }

private:
    using clock = std::chrono::high_resolution_clock;

    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
};

template<class F>
void benchmark(F&& f)
{
    loop l;
    f(l);
    std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(l.iteration_time()).count() << "ns" << std::endl;
}

}
