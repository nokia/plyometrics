#pragma once

#include <memory>

#include "utils.hpp"
#include "cmdline.hpp"
#include "loop.hpp"

namespace plyometrics
{

struct abstract_benchmark
{
    virtual void run(result_printer&) = 0;
    virtual std::string name() const = 0;
    virtual ~abstract_benchmark() = default;
};

template<class Types = std::tuple<nothing>, std::size_t From = 1, std::size_t To = 1>
struct default_spec
{
    using types = Types;
    static constexpr std::size_t from = From;
    static constexpr std::size_t to = To;

    template<class... T>
    using with_types = default_spec<std::tuple<T...>, from, to>;

    template<std::size_t F, std::size_t T>
    using with_range = default_spec<types, F, T>;
};

using spec = default_spec<>;

template<class Crtp, class Spec = default_spec<>>
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
        for (auto i = Spec::from; i <= Spec::to; i *= 2)
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
