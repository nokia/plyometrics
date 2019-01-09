#pragma once

#include "clock.hpp"

#include <string>

namespace nbench
{

struct result
{
    virtual auto iteration_time() const -> clock::duration = 0;
    virtual auto number() const -> std::size_t = 0;
    virtual auto type_name() const -> std::string = 0;
    virtual auto name() const -> std::string = 0;

    virtual ~result() = default;
};

}
