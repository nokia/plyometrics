#pragma once

#include <chrono>

namespace nbench
{

template<class T = int>
struct loop
{
    explicit loop(std::size_t number) : number_(number)
    {
    }

    operator bool()
    {
        if (!_started)
        {
            _start = clock::now();
            _started = true;
        }

        _end = clock::now();
        _iterations++;
        return _end - _start < std::chrono::seconds{2};
    }

    auto iteration_time() const
    {
        return (_end - _start) / _iterations;
    }

    auto number() const
    {
        return number_;
    }

    auto type() -> T
    {
        return T{};
    };

private:
    using clock = std::chrono::high_resolution_clock;

    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
    std::size_t number_;
    bool _started = false;
};

}
