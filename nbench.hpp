#include <vector>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>

namespace nbench
{

#ifndef __OPTIMIZE__
    #warning debug
#endif

template<class Rep, class Period>
struct humanized_time
{
    std::chrono::duration<Rep, Period> _time;
};

template<class Rep, class Period>
std::ostream& operator<<(std::ostream& os, humanized_time<Rep, Period> t)
{
    return os << std::chrono::duration_cast<std::chrono::nanoseconds>(t._time).count() << "ns";
}

template<class Rep, class Period>
auto humanize(std::chrono::duration<Rep, Period> time)
{
    return humanized_time<Rep, Period>{time};
}

struct loop
{
    operator bool()
    {
        _end = clock::now();
        _iterations++;
        return _end - _start < std::chrono::seconds{2};
    }

    auto iteration_time() const
    {
        return humanize((_end - _start) / _iterations);
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
    std::cout << l.iteration_time() << std::endl;
}

template<class F>
void exponential_benchmark(const std::string& name, F&& f, std::size_t start = 1, std::size_t end = 1e5)
{
    for (int i = start; i < end; i *= 2)
    {
        loop l;
        f(l, i);
        std::cout << name << "[" << i << "]: " << l.iteration_time() << std::endl;
    }
}

auto generate_random_data(std::size_t size)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::vector<int> c(size);
    std::generate(c.begin(), c.end(), rng);
    return c;
}

}
