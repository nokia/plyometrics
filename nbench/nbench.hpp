#include <vector>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>

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

template<class... Args>
void swallow(Args&&...)
{
}

struct nothing
{
};

struct exponential_range
{
    std::size_t from = 1, to = 1;
};

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

    auto range(std::size_t from, std::size_t to)
    {
        benchmark2<Types...> b{_name};
        b._range = {from, to};
        return b;
    }

    template<class F, class U = std::tuple<Types...>>
    auto run(const F& f) -> typename std::enable_if<std::tuple_size<U>::value != 0>::type
    {
        swallow(run_type<F, Types>(f)...);
    }

    template<class F, class U = std::tuple<Types...>>
    auto run(const F& f) -> typename std::enable_if<std::tuple_size<U>::value == 0>::type
    {
        run_type<F, nothing>(f);
    }

private:
    template<class F, class Type = nothing>
    auto run_type(const F& f)
    {
        for (auto i = _range.from; i <= _range.to; i *= 2)
        {
            loop<Type> l{i};
            f(l);

            std::cout << _name << " / "
                      << typeid(Type).name()
                      << " [" << l.number() << "]"
                      << ": " << l.iteration_time() << std::endl;
        }

        return nothing{};
    }

    std::string _name;
    exponential_range _range;
};

auto benchmark(const std::string& name)
{
    return benchmark2<>{name};
}

}
