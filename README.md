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
You can write new benchmarks using `BENCHMARK` macro.

```cpp
BENCHMARK("allocate by make_shared") = [](auto& loop)
{
    while (loop)
    {
        auto p = std::make_shared<int>(5);
        escape(p.get());
    }
};
```

`BENCHMARK`'s argument is the name of your test that will later be used when presenting the results. Next, `nbench` is expecting that you will assign a functor to it. Functor must take one argument - the loop. Benchmarks can be type-parameterized, therefore `loop`'s type may vary and it's a good practice to use `auto`.

Every benchmark must be run in a loop so the measurements can be averaged. For this, you can use `loop`'s implicit conversion to `bool`. This construct also helps when you want to initilize your benchmark with some code that you not not necessarily want to measure. For instance, you want to fill the `std::vector` with some data and measure how long it takes to sort it:

```cpp
auto random_data = nbench::random_range(100);
auto v = std::vector<int>{random_data.begin(), random_data.end()};

while (loop)
    std::sort(v.begin(), v.end());
```


Visualizing parametric tests
----------------------------

```
cmake --build build-release/ && ./build-release/example -b 'cache' -x | contrib/visualize.py
```

![alt visualizing cache benchmark](screenshots/cache.png)

If your benchmark is type-parameterized, the chart will include all types used.
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

![alt heap fragmentation](screenshots/fragmentation.png)
