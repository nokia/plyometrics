#pragma once

#include <chrono>

namespace nbench
{

template<class T = int>
struct loop
{
    explicit loop(const char* name, std::size_t number) : _name(name), number_(number)
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

    auto type() const -> T
    {
        return T{};
    };

    auto name() const
    {
        return _name;
    }

private:
    using clock = std::chrono::high_resolution_clock;

    const char* _name;
    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
    std::size_t number_;
    bool _started = false;
};

}
