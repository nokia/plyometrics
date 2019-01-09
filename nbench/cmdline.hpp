#pragma once

#include <map>
#include <sstream>
#include <set>
#include <optional>

namespace nbench
{

constexpr auto none = std::nullopt;

template<class T>
using maybe = std::optional<T>;

template<class T>
auto operator+(const T& a, const maybe<T>& b) -> maybe<T>
{
    return b ? a + *b : a;
}

template<class T, class F>
auto operator>>=(const maybe<T>& m, F f) -> decltype(f(std::declval<T>()))
{
    return m ? f(*m) : none;
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

    std::set<std::string> switches;
    std::map<std::string, std::string> named;
};

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

auto parse_options(int argc, const char* argv[])
{
    std::stringstream ss;

    for (int i = 0; i < argc; i++)
        ss << argv[i] << " ";

    const auto app = read_until_whitespace(ss.str());
    const auto opts = parse_option(app.data_left);

    if (opts)
    {
        for (const auto& s: opts->switches)
        {
            std::cout << "switch: " << s << std::endl;
        }

        for (const auto& s: opts->named)
        {
            std::cout << "opt: " << s.first << " " << s.second << std::endl;
        }
    }

    return default_if_none(opts);
//return options{};
}

}
