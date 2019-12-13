#pragma once

#include "clock.hpp"

#include <string>

namespace plyometrics
{

struct result_t
{
    clock::duration iteration_time;
    std::size_t number;
    std::string type_name;
    std::string name;
};

}
