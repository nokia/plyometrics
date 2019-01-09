#pragma once

#include "utils.hpp"
#include "cmdline.hpp"
#include "loop.hpp"

namespace nbench
{

struct abstract_benchmark
{
    virtual void run(result_printer&) = 0;
    virtual ~abstract_benchmark() = default;
};

template<class Body = nothing, class... Types>
struct benchmark_t : public abstract_benchmark
{
    explicit benchmark_t(const char* name, range_t range, Body body)
        : _name(name), _range(range), _body(body)
    {
    }

    void run(result_printer& printer) override
    {
        run<std::tuple<Types...>>(printer);
    }

private:
    template<class U = std::tuple<Types...>>
    auto run(result_printer& printer) -> typename std::enable_if<std::tuple_size<U>::value != 0>::type
    {
        swallow(run_with_type<Types>(printer)...);
    }

    template<class U = std::tuple<Types...>>
    auto run(result_printer& printer) -> typename std::enable_if<std::tuple_size<U>::value == 0>::type
    {
        run_with_type<nothing>(printer);
    }

    template<class Type = nothing>
    auto run_with_type(result_printer& printer) -> nothing
    {
        for (auto i = _range.from; i <= _range.to; i *= 2)
        {
            auto l = loop<Type>{_name, i};
            _body(l);
            printer.print_result(l);
        }

        return nothing{};
    }

    const char* _name;
    range_t _range;
    Body _body;
};

}
