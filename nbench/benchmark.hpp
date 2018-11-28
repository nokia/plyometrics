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
        run<Body, std::tuple<Types...>>(_body);
    }

private:
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

    template<class F, class Type = nothing>
    auto run_type(const F& f) -> nothing
    {
        for (auto i = _range.from; i <= _range.to; i *= 2)
        {
            auto l = loop<Type>{_name, i};
            f(l);
            std::cout << l << std::endl;
        }

        return nothing{};
    }

    const char* _name;
    range_t _range;
    Body _body;
};

}
