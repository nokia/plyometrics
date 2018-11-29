#pragma once

#include "loop.hpp"

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

template<class T>
inline std::ostream& operator<<(std::ostream& os, const loop<T>& l)
{
    return os << l.name() << " / "
              << demangle<T>{}
              << " [" << l.number() << "]"
              << ": " << humanize(l.iteration_time());
}

template<class T>
struct json_object
{
    const T& object;
};

template<class T>
inline std::ostream& operator<<(std::ostream& os, const json_object<loop<T>>& l)
{
    return os << "{"
              << "name: \"" << l.object.name() << " [" << demangle<T>{} << "]\", "
              << "number: " << l.object.number() << ", "
              << "time: " << l.object.iteration_time().count()
              << "}";
}

}