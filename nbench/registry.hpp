#pragma once

#include "cmdline.hpp"
#include "benchmark.hpp"

#include <vector>
#include <memory>

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

    void run_all()
    {
        std::cout << _benchmarks.size() << " benchmarks to run" << std::endl;
        for (auto& b : _benchmarks)
            b->run();
    }

private:
    std::vector<std::unique_ptr<abstract_benchmark>> _benchmarks;
};

void run_all(int argc, const char* argv[])
{
    auto opts = nbench::parse_options(argc, argv);
    registry::get().run_all();
}

}

#define UNIQUE_NAME_IMPL(prefix, counter)  prefix##counter
#define UNIQUE_NAME(prefix, counter) UNIQUE_NAME_IMPL(prefix, counter)

#define BENCHMARK(name) auto UNIQUE_NAME(nbench_benchmark_, __COUNTER__) = benchmark_builder<>{name}

