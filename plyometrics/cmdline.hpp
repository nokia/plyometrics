#pragma once

#include "optional.hpp"

#include <map>
#include <sstream>
#include <set>

namespace plyometrics
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

struct input_t
{
    int argc;
    const char** argv;

    operator bool() const
    {
        return argc;
    }

    auto next() const
    {
        return input_t{argc - 1, std::next(argv)};
    }

    auto current() const
    {
        return std::string{argv[0]};
    }
};

struct p_result
{
    input_t input;
    maybe<std::string> value;
};

auto read_word(const input_t& input)
{
    if (!input)
        return p_result{input, none};
    return p_result{input.next(), input.current()};
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

auto try_read_value(const input_t& input)
{
    if (!input)
        return p_result{input, none};

    if (is_switch(input.current()) || is_option(input.current()))
        return p_result{input, none};

    return p_result{input.next(), input.current()};
}

struct parsing_state
{
    options opts;
    input_t input;
};

auto parse_option(const input_t& input)
{
    options opts;

    const auto opt = read_word(input);

    if (!opt.value)
        return parsing_state{opts, opt.input};

    if (is_switch(opt.value) || is_option(opt.value))
    {
        const auto value = try_read_value(opt.input);

        if (value.value)
        {
            opts.named.emplace(*opt.value, *value.value);
        }
        else
        {
            opts.switches.emplace(*opt.value);
        }

        return parsing_state{opts, value.input};
    }

    throw "expected a switch";
}

auto parse_options(int argc, const char* argv[])
{
    options opts;

    const auto app = read_word(input_t{argc, argv});

    if (app.value)
    {
        auto input = app.input;
        while (input)
        {
            const auto r = parse_option(input);
            input = r.input;
            opts = opts + r.opts;
        }
    }

    return opts;
}

}
