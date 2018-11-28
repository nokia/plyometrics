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
#include "loop.hpp"
#include "benchmark.hpp"

namespace nbench
{

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
