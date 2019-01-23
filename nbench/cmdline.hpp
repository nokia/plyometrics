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

struct parse_result
{
    maybe<std::string> result;
    std::string data_left;
};

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

auto read_until_whitespace(const std::string& data) -> parse_result
{
    if (data.empty())
        return parse_result{none, data};

    if (data[0] == ' ')
        return parse_result{none, data.substr(1)};

    const auto leaf = read_until_whitespace(data.substr(1));
    return parse_result{data.substr(0, 1) + leaf.result, leaf.data_left};
}

auto parse_option(const std::string& data) -> maybe<options>
{
    if (!data.empty() && data[0] == '-')
    {
        options opts;

        const auto name = read_until_whitespace(data.substr(1));
        const auto value = read_until_whitespace(name.data_left);

        if (name.result and value.result)
            opts.named.emplace(*name.result, *value.result);
        else if (name.result)
            opts.switches.emplace(*name.result);
        else
            throw 1;

        return opts + parse_option(value.data_left);
    }

    return none;
}

struct p_result
{
    int argc;
    const char** argv;
    maybe<std::string> value;
};

auto read_one(int argc, const char* argv[])
{
    if (!argc)
        return p_result{0, argv, none};
    return p_result{argc - 1, std::next(argv), argv[0]};
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

    return p_result{argc - 1, std::next(argv), argv[0]};
}

auto parse_options(int argc, const char* argv[])
{
    options opts;

    const auto app = read_one(argc, argv);

    if (app.value)
    {
        const auto opt = read_one(app.argc, app.argv);

        if (!opt.value)
            return opts;

        if (is_switch(opt.value) || is_option(opt.value))
        {
            const auto value = try_read_value(opt.argc, opt.argv);
            if (value.value)
            {
                opts.named.emplace(*opt.value, *value.value);
            }
            else
            {
                opts.switches.emplace(*opt.value);
            }
        }
    }

    return opts;
}

}
