#pragma once

#include "cmdline.hpp"
#include "benchmark.hpp"

#include <vector>
#include <memory>
#include <regex>

namespace nbench
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
        std::cout << _benchmarks.size() << " benchmarks to run\n" << std::endl;
        auto printer = make_result_printer(opts);
        auto re = benchmark_regex(opts);

        for (auto& b : _benchmarks)
            if (std::regex_search(b->name(), re))
                b->run(*printer);
    }

private:
    auto benchmark_regex(const options& opts) const -> std::regex
    {
        auto s = opts.option("b");
        if (s)
            return std::regex{*s};
        return std::regex{".*"};
    }

    std::vector<std::unique_ptr<abstract_benchmark>> _benchmarks;
};

void run_all(int argc, const char* argv[])
{
    auto opts = nbench::parse_options(argc, argv);
    registry::get().run_all(opts);
}

}

#define UNIQUE_NAME_IMPL(prefix, counter)  prefix##counter
#define UNIQUE_NAME(prefix, counter) UNIQUE_NAME_IMPL(prefix, counter)

#define BENCHMARK(name) auto UNIQUE_NAME(nbench_benchmark_, __COUNTER__) = benchmark_builder<>{name}

