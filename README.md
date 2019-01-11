Installation
------------
`nbench` is a header-only library so while the samples are compiled using CMake, you can easily integrate it into you own, customized build system by just copying `nbench` directory and optionaly the `contrib` which is the place for some usefull scripts.


Basics
------
You can write new benchmarks using BENCHMARK macro.

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


Visualizing parametric tests
----------------------------

```
cmake --build build-release/ && ./build-release/example -b 'cache' -x | contrib/visualize.py
```

![alt visualizing cache benchmark](screenshots/cache.png)
