`nbench` is microbenmarking library targeted for embedded software development. It comes with a number of examples that can be used for rapid testing of new architectures.

Installation
------------
`nbench` is a header-only library so while the samples are compiled using CMake, you can easily integrate it into you own, customized build system by just copying `nbench` directory and optionaly the `contrib` which is the place for some usefull scripts.

When you have the code in place, you can include `nbench.hpp` header and write the `main` function.

```cpp
#include "nbench/nbench.hpp"

int main(int argc, const char* argv[])
{
    nbench::run_all(argc, argv);
}
```

And there you have it, now you can write your first benchmark.


Writing simple benchmarks
-------------------------
You can write new benchmarks using `NBENCHMARK` or `NBENCHMARK_P` macro.

```cpp
NBENCHMARK(allocate_by_make_shared)
{
    while (loop)
    {
        auto p = std::make_shared<int>(5);
        nbench::escape(p.get());
    }
}
```

`BENCHMARK`'s argument is the name of your test that will later be used when presenting the results. Within benchmark's body, a variable called `loop`, for now, we're using it to construct `while` loop where you put time critical code you want to measure.

If your test requires some initialization, say, generating some dataset, you can simply do that outside the `while` loop, like this:

```cpp
auto random_data = nbench::random_range(100);
auto v = std::vector<int>{random_data.begin(), random_data.end()};

while (loop)
    std::sort(v.begin(), v.end());
```


Visualizing parametric tests
----------------------------
You can visualize parametric tests by utility script from `contrib` directory. Just pass `-x` argument (JSON output) to your benchmark and pipe the result through `visualize.py`.

```
cmake --build build-release/ && ./build-release/example -b 'cache' -x | contrib/visualize.py
```

![alt visualizing cache benchmark](screenshots/cache.png)

If your benchmark is also type-parameterized, the chart will include all types used.

![alt visualizing iteration benchmark](screenshots/iterating.png)


Heap fragmentation
------------------
In the real world, your app's heap might be fragmented. To simulate this, you can use `nbench::fragmentize_heap` function. It returns an object which you should keep in the scope of your test.

```cpp
BENCHMARK("iterating").types<
    std::vector<int>, std::list<int>,
    std::set<int>, std::unordered_set<int>
>().range(1e6, 1e7) = [](auto& loop)
{
    // keep it in this scope
    auto fragmentized = nbench::fragmentize_heap();

    auto data = nbench::sequence_range(loop.number());

    while (loop)
        for (auto i : loop.type(data.begin(), data.end()))
            nbench::use(i);
};
```

See the difference in iterating of various containers when memory is fragmented (right) and where it is not (left).

![alt heap fragmentation](screenshots/fragmentation.png)
