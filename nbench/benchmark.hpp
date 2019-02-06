#pragma once

#include "utils.hpp"
#include "cmdline.hpp"
#include "loop.hpp"

namespace nbench
{

struct abstract_benchmark
{
    virtual void run(result_printer&) = 0;
    virtual std::string name() const = 0;
    virtual ~abstract_benchmark() = default;
};

struct default_spec
{
    using types = std::tuple<nothing>;
    static constexpr range_t range = range_t{1, 1};
};

template<class Crtp, class Spec = default_spec>
struct benchmark_base : public abstract_benchmark
{
    static auto construct()
    {
        return std::make_unique<Crtp>();
    }

    void run(result_printer& printer) override
    {
        run<typename Spec::types>(printer, std::make_index_sequence<std::tuple_size<typename Spec::types>::value>{});
    }

    auto name() const -> std::string override
    {
        return typeid(*this).name();
    }

private:
    template<class Types, std::size_t... Ints>
    auto run(result_printer& printer, std::index_sequence<Ints...>)
    {
        swallow(run_with_type<typename std::tuple_element<Ints, Types>::type>(printer)...);
    }

    template<class Type = nothing>
    auto run_with_type(result_printer& printer) -> nothing
    {
        for (auto i = Spec::range.from; i <= Spec::range.to; i *= 2)
        {
            auto l = loop<Type>{name(), i};
            static_cast<Crtp*>(this)->body(l);
            printer.print_result(l);
        }

        return nothing{};
    }
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

    auto name() const -> std::string override
    {
        return _name;
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
