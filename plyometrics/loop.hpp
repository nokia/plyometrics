#pragma once

#include "output.hpp"
#include "result.hpp"
#include "clock.hpp"

namespace plyometrics
{

template<std::size_t N, class T>
struct loop : public result
{
    explicit loop(std::string name, std::size_t number) : _name(name), number_(number)
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

    auto iteration_time() const -> clock::duration override
    {
        return (_end - _start) / _iterations;
    }

    constexpr auto number() const -> std::size_t override
    {
        return N;
    }

    template<class... Args>
    auto type(Args&&... args) const -> T
    {
        return T{std::forward<Args>(args)...};
    }

    auto type_name() const -> std::string override
    {
        std::stringstream ss;
        ss << demangle<T>{};
        return ss.str();
    }

    auto name() const -> std::string override
    {
        return _name;
    }

private:
    std::string _name;
    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
    std::size_t number_;
    bool _started = false;
};

}
