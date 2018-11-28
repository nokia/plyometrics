#pragma once

namespace nbench
{

struct range_t
{
    std::size_t from = 1, to = 1;
};

template<class... Args>
void swallow(Args&&...)
{
}

struct nothing
{
};

}
