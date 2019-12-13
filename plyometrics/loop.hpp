#pragma once

#include "output.hpp"
#include "result.hpp"
#include "clock.hpp"

namespace plyometrics
{

template<std::size_t N, class T>
struct loop
{
    explicit loop(std::string name) : _name(name)
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
        return _end - _start < std::chrono::seconds{1};
    }

    auto result() const
    {
        result_t r;
        r.iteration_time = (_end - _start) / _iterations;
        r.name = _name;
        r.number = N;
        return r;
    }

    static constexpr auto number() -> std::size_t
    {
        return N;
    }

    static constexpr auto cnumber() -> std::size_t
    {
        return N;
    }

    template<class... Args>
    auto type(Args&&... args) const -> T
    {
        return T{std::forward<Args>(args)...};
    }

private:
    auto type_name() const -> std::string
    {
        std::stringstream ss;
        ss << demangle<T>{};
        return ss.str();
    }

    std::string _name;
    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
    bool _started = false;
};

}
