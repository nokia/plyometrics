#pragma once

#include "cmdline.hpp"
#include "result.hpp"

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
struct json_object
{
    const T& object;
};

//template<class T>
//void print_json(std::ostream& os, const loop<T>& l)
//{
//    os << "{"
//       << "name: \"" << l.name() << " [" << demangle<T>{} << "]\", "
//       << "type: \"" << demangle<T>{} << "\", "
//       << "number: " << l.number() << ", "
//       << "time: " << l.iteration_time().count()
//       << "}" << std::endl;
//}
//
//template<class Loop>
//void print_result(const Loop& loop, const options& opts)
//{
//    if (opts.has_switch("x"))
//        print_json(std::cout, loop);
//    else
//        std::cout << loop << std::endl;
//}

struct result_printer
{
    virtual void print_result(const result&) = 0;
    virtual ~result_printer() = default;
};

struct printer_for_humans : public result_printer
{
    void print_result(const result& res) override
    {
        std::cout << res.name() << " / "
                  << res.type_name()
                  << " [" << res.number() << "]"
                  << ": " << humanize(res.iteration_time()) << std::endl;
    }

    ~printer_for_humans() override
    {
        std::cout << '\n' << "above results are printed using default, human readable printer";
    }
};

auto make_result_printer(const options& opts) -> std::unique_ptr<result_printer>
{
    return std::make_unique<printer_for_humans>();
}

}
