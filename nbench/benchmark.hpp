#pragma once

#include "utils.hpp"

namespace nbench
{

struct abstract_benchmark
{
    virtual void run() = 0;
    virtual ~abstract_benchmark() = default;
};

template<class Body = nothing, class... Types>
struct benchmark_t : public abstract_benchmark
{
    explicit benchmark_t(const char* name, range_t range, Body body)
        : _name(name), _range(range), _body(body)
    {
    }

    void run() override
    {
        run<std::tuple<Types...>>();
    }

private:
    template<class U = std::tuple<Types...>>
    auto run() -> typename std::enable_if<std::tuple_size<U>::value != 0>::type
    {
        swallow(run_with_type<Types>()...);
    }

    template<class U = std::tuple<Types...>>
    auto run() -> typename std::enable_if<std::tuple_size<U>::value == 0>::type
    {
        run_with_type<nothing>();
    }

    template<class Type = nothing>
    auto run_with_type() -> nothing
    {
        for (auto i = _range.from; i <= _range.to; i *= 2)
        {
            auto l = loop<Type>{_name, i};
            _body(l);
            std::cout << l << std::endl;
        }

        return nothing{};
    }

    const char* _name;
    range_t _range;
    Body _body;
};

}
