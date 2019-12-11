#pragma once

#include "cmdline.hpp"
#include "loop.hpp"
#include "utils.hpp"

#include <memory>

namespace plyometrics
{

template<class Types = std::tuple<nothing>,
         class Sequence = std::index_sequence<1>>
struct default_spec
{
    using types = Types;
    using range = Sequence;

    template<class... T>
    using with_types = default_spec<std::tuple<T...>, Sequence>;

    template<std::size_t Base, std::size_t Multiplier, std::size_t Max>
    using with_range = default_spec<types, geometric_sequence<Base, Multiplier, Max>>;
};

/**
 * Define parameters for the benchmarks
 */
using spec = default_spec<>;

struct abstract_benchmark
{
    virtual void run(result_printer&) = 0;
    virtual std::string name() const = 0;
    virtual ~abstract_benchmark() = default;
};

template<class Crtp, class Spec = default_spec<>>
struct benchmark_base : public abstract_benchmark
{
    template<class T>
    struct forward_type
    {
        using type = T;
    };

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
        visit_each_type<typename Spec::range>(*this, printer, forward_type<Type>{});
    }

    template<std::size_t N, class T>
    void accept(result_printer& printer, T)
    {
        auto l = loop<typename T::type>{name(), N};
        static_cast<Crtp*>(this)->body(l);
        printer.print_result(l);
    }
};

} // namespace plyometrics
