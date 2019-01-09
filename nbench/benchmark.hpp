#pragma once

#include "utils.hpp"
#include "cmdline.hpp"

namespace nbench
{

struct abstract_benchmark
{
    virtual void run(const options&) = 0;
    virtual ~abstract_benchmark() = default;
};

template<class Body = nothing, class... Types>
struct benchmark_t : public abstract_benchmark
{
    explicit benchmark_t(const char* name, range_t range, Body body)
        : _name(name), _range(range), _body(body)
    {
    }

    void run(const options& opts) override
    {
        run<std::tuple<Types...>>(opts);
    }

private:
    template<class U = std::tuple<Types...>>
    auto run(const options& opts) -> typename std::enable_if<std::tuple_size<U>::value != 0>::type
    {
        swallow(run_with_type<Types>(opts)...);
    }

    template<class U = std::tuple<Types...>>
    auto run(const options& opts) -> typename std::enable_if<std::tuple_size<U>::value == 0>::type
    {
        run_with_type<nothing>(opts);
    }

    template<class Type = nothing>
    auto run_with_type(const options& opts) -> nothing
    {
        for (auto i = _range.from; i <= _range.to; i *= 2)
        {
            auto l = loop<Type>{_name, i};
            _body(l);
            print_result(l, opts);
        }

        return nothing{};
    }

    const char* _name;
    range_t _range;
    Body _body;
};

}
