#pragma once

#include <vector>
#include <memory>

namespace nbench
{

struct abstract_benchmark
{
    virtual void run() = 0;
    virtual ~abstract_benchmark() = default;
};

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

}
