#include "plyometrics/plyometrics.hpp"
#include <atomic>
#include <thread>

using cache_spec = plyometrics::spec::with_range<1, 100000>;

NBENCHMARK_P(cache, cache_spec)
{
    auto random = plyometrics::random_range(10000);
    auto indicies = std::vector<std::size_t>{};

    for (auto r : random)
        indicies.emplace_back(r % loop.number());

    auto v = std::vector<char>(1e9, 0);

    while (loop)
    {
        for (auto idx : indicies)
        {
            v[idx]++;
            plyometrics::escape(&v[idx]);
        }
    }
}

template<std::size_t LineSize>
struct two_aligned_variables
{
    alignas(64) std::atomic<int> a;
    alignas(LineSize) int b;
};

template<class F>
auto make_n_threads(std::size_t n, F f)
{
    std::vector<std::thread> threads;
    threads.reserve(n);
    for (auto i = 0u; i < n; i++)
        threads.emplace_back(f);
    return threads;
}

using false_sharing_spec = plyometrics::spec::with_types<two_aligned_variables<1>, two_aligned_variables<64>>::with_range<1, 64>;

NBENCHMARK_P(false_sharing, false_sharing_spec)
{
    decltype(loop.type()) data;
    std::atomic<bool> running(true);
    auto threads = make_n_threads(loop.number(), [&] { while (running.load()) plyometrics::use(data.a++); });

    while (loop)
        for (int i = 0; i < 100; i++)
            plyometrics::use(data.b++);
    
    running.store(false);

    for (auto& t : threads)
        t.join();
};

template<class T>
struct non_atomic
{
    non_atomic(T value) : _value(value)
    {
    }

    void operator++(int)
    {
        _value++;
    }

    T load() const
    {
        return _value;
    }

private:
    int _value;
};

using atomic_spec = plyometrics::spec::with_types<non_atomic<int>, std::atomic<int>>;

NBENCHMARK_P(read_and_write_to_atomic, atomic_spec)
{
    decltype(loop.type()) value(0);

    while (loop)
    {
        value++;
        plyometrics::use(value.load());
    }
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
