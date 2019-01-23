#pragma once

#include "optional.hpp"

#include <map>
#include <sstream>
#include <set>

namespace nbench
{

constexpr auto none = nullopt;

template<class T>
using maybe = optional<T>;

template<class T>
auto operator+(const T& a, const maybe<T>& b) -> maybe<T>
{
    return b ? a + *b : a;
}

struct options
{
    options() = default;

    options(const std::string& switch_) : switches{switch_}
    {
    }

    options(const std::string& name, const std::string& value)
        : named{{name, value}}
    {
    }

    bool has_switch(const std::string& name) const
    {
        return switches.find(name) != switches.end();
    }

    maybe<std::string> option(const std::string& name) const
    {
        auto it = named.find(name);
        if (it != named.end())
            return it->second;
        return none;
    }

    std::set<std::string> switches;
    std::map<std::string, std::string> named;
};

std::ostream& operator<<(std::ostream& os, const options& opts)
{
    if (opts.switches.empty() && opts.named.empty())
        return os << "-";

    for (const auto s : opts.switches)
        os << "switch: " << s << ", ";

    for (const auto s : opts.named)
        os << "named: " << s.first << ": " << s.second << ", ";

    return os;
}

auto operator+(const options& a, const options& b)
{
    options r = a;
    std::copy(b.switches.begin(), b.switches.end(), std::inserter(r.switches, r.switches.end()));
    std::copy(b.named.begin(), b.named.end(), std::inserter(r.named, r.named.end()));
    return r;
}

template<class T>
auto none_if_empty(const T& value) -> maybe<T>
{
    return value.empty() ? maybe<T>{none} : value;
}

template<class T>
auto default_if_none(const maybe<T>& value) -> T
{
    return value ? *value : T{};
}

struct input_t
{
    int argc;
    const char** argv;
};

struct p_result
{
    input_t input;
    maybe<std::string> value;
};

auto read_one(int argc, const char* argv[])
{
    if (!argc)
        return p_result{0, argv, none};
    return p_result{input_t{argc - 1, std::next(argv)}, argv[0]};
}

bool is_option(const maybe<std::string>& s)
{
    if (!s)
        return false;

    return s->size() > 2 && (*s)[0] == '-' && (*s)[1] == '-';
}

bool is_switch(const maybe<std::string>& s)
{
    if (!s)
        return false;

    return s->size() == 2 && (*s)[0] == '-';
}

auto try_read_value(int argc, const char** argv)
{
    if (!argc)
        return p_result{argc, argv, none};

    if (is_switch(argv[0]) || is_option(argv[0]))
        return p_result{argc, argv, none};

    return p_result{input_t{argc - 1, std::next(argv)}, argv[0]};
}

struct parsing_state
{
    options opts;
    p_result result;
};

auto parse_option(int argc, const char** argv)
{
    options opts;

        const auto opt = read_one(argc, argv);

        if (!opt.value)
            return parsing_state{opts, opt};

        if (is_switch(opt.value) || is_option(opt.value))
        {
            const auto value = try_read_value(opt.input.argc, opt.input.argv);

            if (value.value)
            {
                opts.named.emplace(*opt.value, *value.value);
            }
            else
            {
                opts.switches.emplace(*opt.value);
            }

            return parsing_state{opts, value};
        }

    throw "expected a switch";
}

auto parse_options(int argc, const char* argv[])
{
    options opts;

    const auto app = read_one(argc, argv);

    if (app.value)
    {
        while (true)
        {
            const auto r = parse_option(app.input.argc, app.input.argv);
            opts = opts + r.opts;
            if (!r.result.input.argc)
                break;
        }
    }

    return opts;
}

}
