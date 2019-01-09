#pragma once

#include <chrono>

namespace nbench
{

using clock = std::chrono::high_resolution_clock;

struct result
{
    virtual auto iteration_time() const -> clock::duration = 0;
    virtual auto number() const -> std::size_t = 0;
    virtual auto type_name() const -> std::string = 0;
    virtual auto name() const -> std::string = 0;

    virtual ~result() = default;
};

template<class T = int>
struct loop : public result
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

    auto iteration_time() const -> clock::duration override
    {
        return (_end - _start) / _iterations;
    }

    auto number() const -> std::size_t override
    {
        return number_;
    }

    auto type() const -> T
    {
        return T{};
    };

    auto type_name() const -> std::string override
    {
        return "";
    }

    auto name() const -> std::string override
    {
        return _name;
    }

private:
    const char* _name;
    clock::time_point _start = clock::now();
    clock::time_point _end;
    std::size_t _iterations = 0;
    std::size_t number_;
    bool _started = false;
};

}
