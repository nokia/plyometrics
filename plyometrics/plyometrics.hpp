#include <vector>
#include <functional>
#include <chrono>
#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>
#include <memory>
#include <type_traits>

#include "compiler.hpp"
#include "output.hpp"
#include "registry.hpp"
#include "loop.hpp"
#include "benchmark.hpp"
#include "ranges.hpp"

namespace plyometrics
{

struct benchmark_adder
{
    benchmark_adder(std::unique_ptr<abstract_benchmark> benchmark)
    {
        registry::get().register_benchmark(std::move(benchmark));
    }
};

}
