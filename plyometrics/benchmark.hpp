#pragma once

#include "cmdline.hpp"
#include "loop.hpp"
#include "utils.hpp"

#include <memory>

namespace plyometrics
{

struct abstract_benchmark
{
    virtual void run(result_printer&) = 0;
    virtual std::string name() const = 0;
    virtual ~abstract_benchmark() = default;
};

template<class Types = std::tuple<nothing>,
         std::size_t From = 1,
         std::size_t To = 1>
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

/**
 * Define parameters for the benchmarks
 */
using spec = default_spec<>;

template<class Crtp, class Spec = default_spec<>>
struct benchmark_base : public abstract_benchmark
{
    auto name() const -> std::string override
    {
        std::stringstream ss;
        ss << demangle<Crtp>{};
        return ss.str();
    }

    void run(result_printer& printer) override
    {
        visit_each_type<typename Spec::types>(*this, printer);
    }

    template<class Type>
    void accept(result_printer& printer)
    {
        for (auto i = Spec::from; i <= Spec::to; i *= 2)
        {
            auto l = loop<Type>{ name(), i };
            static_cast<Crtp*>(this)->body(l);
            printer.print_result(l);
        }
    }
};

} // namespace plyometrics
