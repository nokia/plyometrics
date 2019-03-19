#pragma once

#include "cmdline.hpp"
#include "benchmark.hpp"

#include <memory>
#include <iostream>
#include <vector>
#include <memory>
#include <regex>

namespace plyometrics
{

struct registry
{
    static auto& get()
    {
        static registry r;
        return r;
    }

    static void register_benchmark(std::unique_ptr<abstract_benchmark> b)
    {
        get()._benchmarks.push_back(std::move(b));
    }

    void run_all(const options& opts)
    {
        std::cerr << "CPU: " << cpu_model() << std::endl;

        try
        {
            if (cpu_scaling_enabled())
            {
                std::cerr << "CPU frequency scaling seems to be enabled, this might affect your benchmarks!\n";

            }
        }
        catch (const std::exception& ex)
        {
            std::cerr << "Couln't determinate CPU frequency scaling status, reason: " << ex.what() << "\n";
        }

        auto printer = make_result_printer(opts);
        auto re = benchmark_regex(opts);

        for (auto& b : _benchmarks)
            if (std::regex_search(b->name(), re))
                b->run(*printer);
    }

private:
    auto benchmark_regex(const options& opts) const -> std::regex
    {
        auto s = opts.option("-b");
        if (s)
            return std::regex{*s};
        return std::regex{".*"};
    }

    std::vector<std::unique_ptr<abstract_benchmark>> _benchmarks;
};

void run_all(int argc, const char* argv[])
{
    auto opts = plyometrics::parse_options(argc, argv);
    registry::get().run_all(opts);
}

}

#define UNIQUE_NAME_IMPL(prefix, counter)  prefix##counter
#define UNIQUE_NAME(prefix, counter) UNIQUE_NAME_IMPL(prefix, counter)

#define BENCHMARK(name) auto UNIQUE_NAME(plyometrics_benchmark_, __COUNTER__) = ::plyometrics::benchmark_builder<>{name}


#define NBENCHMARK(name) \
    struct name : plyometrics::benchmark_base<name> \
    { \
        template<class T> __attribute__((noinline)) void body(T&); \
    }; \
    auto NBENCH_ADDER_##name = plyometrics::benchmark_adder{name::construct()}; \
    template<class T> void name::body(T& loop)

/**
 * Define parametric benchmark, second argument should be a `spec` with the parameters.
 */
#define NBENCHMARK_P(name, ...) \
    struct name : plyometrics::benchmark_base<name, __VA_ARGS__> \
    { \
        template<class T> __attribute__((noinline)) void body(T&); \
    }; \
    auto NBENCH_ADDER_##name = plyometrics::benchmark_adder{name::construct()}; \
    template<class T> void name::body(T& loop)

