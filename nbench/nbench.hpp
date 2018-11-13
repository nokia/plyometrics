#include <vector>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>

#include "compiler.hpp"

namespace nbench
{

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

template<class T = int>
struct loop
{
    explicit loop(std::size_t number) : number_(number)
    {
    }

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

    auto number() const
    {
        return number_;
    }

    auto type() -> T
    {
        return T{};
    };

private:
    using clock = std::chrono::high_resolution_clock;

    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
    std::size_t number_;
};

template<class F>
void benchmark(F&& f)
{
    loop<> l{0};
    f(l);
    std::cout << l.iteration_time() << std::endl;
}

template<class F, class T = void>
void exponential_benchmark(const std::string& name, F&& f, std::size_t start = 1, std::size_t end = 1e5)
{
    for (std::size_t i = start; i < end; i *= 2)
    {
        loop<T> l{i};
        f(l);
        std::cout << name << "[" << i << "]: " << l.iteration_time() << std::endl;
    }
}

template<class... Args>
void swallow(Args&&...)
{
}

template<class... Types>
struct benchmark2
{
    benchmark2(const std::string& name) : _name(name)
    {
    }

    template<class... _Types>
    auto types()
    {
        return benchmark2<_Types...>(_name);
    }

    template<class F>
    void run(const F& f)
    {
        swallow(run_type<F, Types>(f)...);
    }

private:
    template<class F, class Type>
    auto run_type(const F& f) -> int
    {
        loop<Type> l{0};
        f(l);
        std::cout << _name << ": " << typeid(Type).name() << l.iteration_time() << std::endl;
        return 0;
    }

    std::string _name;
};

}
