#include <vector>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>
#include <memory>
#include <type_traits>

#include "compiler.hpp"
#include "output.hpp"
#include "registry.hpp"

namespace nbench
{

template<class T = int>
struct loop
{
    explicit loop(std::size_t number) : number_(number)
    {
    }

    operator bool()
    {
        if (!_started)
        {
            _start = clock::now();
            _started = true;
        }

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
    bool _started = false;
};

template<class... Args>
void swallow(Args&&...)
{
}

struct nothing
{
};

struct range_t
{
    std::size_t from = 1, to = 1;
};

template<class Body = nothing, class... Types>
struct benchmark_t : public abstract_benchmark
{
    explicit benchmark_t(const char* name) : _name(name)
    {
    }

    explicit benchmark_t(const char* name, range_t range, Body body)
        : _name(name), _range(range), _body(body)
    {
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

    void run() override
    {
        run<Body, std::tuple<Types...>>(_body);
    }

//private:
    template<class F, class Type = nothing>
    auto run_type(const F& f) -> nothing
    {
        for (auto i = _range.from; i <= _range.to; i *= 2)
        {
            auto l = loop<Type>{i};
            f(l);

            std::cout << _name << " / "
                      << demangle<Type>{}
                      << " [" << l.number() << "]"
                      << ": " << l.iteration_time() << std::endl;
        }

        return nothing{};
    }

    const char* _name;
    range_t _range;
    Body _body;
};

struct benchmark_adder
{
    benchmark_adder(std::unique_ptr<abstract_benchmark> benchmark)
    {
        registry::get().register_benchmark(std::move(benchmark));
    }
};

template<class Body = nothing, class... Types>
struct benchmark_builder
{
    template<class... _Types>
    constexpr auto types()
    {
        return benchmark_builder<Body, _Types...>{_name, _range, _body};
    }

    constexpr auto range(std::size_t from, std::size_t to)
    {
        return benchmark_builder<Body, Types...>{_name, {from, to}, _body};
    }

    template<class F>
    constexpr auto body(const F& f)
    {
        return benchmark_adder{std::make_unique<benchmark_t<F, Types...>>(_name, _range, f)};
    }

    template<class F>
    constexpr auto operator=(const F& f)
    {
        return benchmark_adder{std::make_unique<benchmark_t<F, Types...>>(_name, _range, f)};
    }

    const char* _name = "unnamed";
    range_t _range;
    Body _body;
};


}
