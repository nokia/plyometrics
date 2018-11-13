#pragma once

#include <chrono>
#include <ostream>

#include <cxxabi.h>

namespace nbench
{

template<class Rep, class Period>
struct humanized_time
{
    std::chrono::duration<Rep, Period> _time;
};

template<class Rep, class Period>
std::ostream& operator<<(std::ostream& os, humanized_time<Rep, Period> t)
{
    return os << std::chrono::duration_cast<std::chrono::nanoseconds>(t._time).count() << "ns";
}

template<class Rep, class Period>
auto humanize(std::chrono::duration<Rep, Period> time)
{
    return humanized_time<Rep, Period>{time};
}

template<class T>
struct demangle
{
};

template<class T>
std::ostream& operator<<(std::ostream& os, demangle<T>)
{
    int status;
    char* realname;
    realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    os << realname;
    free(realname);
    return os;
}

struct benchmark_results
{
    std::string name;

};

}
