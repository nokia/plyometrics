#pragma once

#include "cmdline.hpp"
#include "benchmark.hpp"
#include "cpu.hpp"

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
        std::cerr << "cpu: " << cpu_model() << std::endl;
        warn_on_cpu_freq_scaling();

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

#define NBENCHMARK(name) \
    struct name : plyometrics::benchmark_base<name> \
    { \
        template<class T> __attribute__((noinline)) void body(T&); \
    }; \
    auto NBENCH_ADDER_##name = plyometrics::benchmark_adder{std::make_unique<name>()}; \
    template<class T> void name::body(T& loop)

/**
 * Define parametric benchmark, second argument should be a `spec` with the parameters.
 */
#define NBENCHMARK_P(name, ...) \
    struct name : plyometrics::benchmark_base<name, __VA_ARGS__> \
    { \
        template<class T> __attribute__((noinline)) void body(T&); \
    }; \
    auto NBENCH_ADDER_##name = plyometrics::benchmark_adder{std::make_unique<name>()}; \
    template<class T> void name::body(T& loop)

