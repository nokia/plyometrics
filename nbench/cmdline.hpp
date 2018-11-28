#pragma once

#include <map>
#include <sstream>

namespace nbench
{

struct none_t{};
constexpr auto none = none_t{};

template<class T>
struct maybe
{
    maybe(none_t) : has(false), data{}
    {
    }

    maybe(T data) : has(true), data(data)
    {
    }

    operator bool() const
    {
        return has;
    }

    T& operator*()
    {
        return data;
    }

    const T& operator*() const
    {
        return data;
    }

    template<class F>
    auto map(F f) -> decltype(f(std::declval<T>()))
    {
        return has ? f(data) : none;
    }

    bool has;
    T data;
};

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

using maybe_result = maybe<parse_result>;

struct options
{
    std::set<std::string> switches;
    std::map<std::string, std::string> options;
};

auto operator+(const options& a, const options& b)
{
    options r = a;
    std::copy(b.switches.begin(), b.switches.end(), std::inserter(r.switches, r.switches.end()));
    std::copy(b.options.begin(), b.options.end(), std::inserter(r.options, r.options.end()));
    return r;
}

template<class T>
auto none_if_empty(const T& value) -> maybe<T>
{
    return value.empty() ? maybe<T>{none} : value;
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
    return none_if_empty(data) >>= [](auto data)
    {
        if (data[0] == '-')
        {
            options opts;

            const auto name = read_until_whitespace(data.substr(1));
            const auto value = read_until_whitespace(name.data_left);

            if (name.result and value.result)
                opts.options.emplace(*name.result, *value.result);
            else if (name.result)
                opts.switches.emplace(*name.result);
            else
                throw 1;

            return opts + parse_option(value.data_left);
        }
    };
}

auto parse(int argc, const char* argv[])
{
    std::stringstream ss;

    for (int i = 0; i < argc; i++)
        ss << argv[i] << " ";

    const auto app = read_until_whitespace(ss.str());
    const auto opts = parse_option(app.data_left);

    for (const auto& s: opts.data.switches)
    {
        std::cout << "switch: " << s << std::endl;
    }

    for (const auto& s: opts.data.options)
    {
        std::cout << "opt: " << s.first << " " << s.second << std::endl;
    }

    return opts;
}

}
