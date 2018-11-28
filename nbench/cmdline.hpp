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

    bool has;
    T data;
};

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

template<class T>
auto none_if_empty(const T& value) -> maybe<T>
{
    return value.empty() ? maybe<T>{none} : value;
}

auto read_until_whitespace(const std::string& data_left, const std::string& read_so_far = "") -> parse_result
{
    if (data_left.empty())
    {
        return parse_result{none_if_empty(read_so_far), data_left};
    }

    if (data_left[0] == ' ')
    {
        return parse_result{none_if_empty(read_so_far), data_left.substr(1)};
    }

    return read_until_whitespace(data_left.substr(1), read_so_far + data_left[0]);
}

auto parse_option(const std::string& data, options opts = options{}) -> maybe<options>
{
    if (data.empty())
    {
        return opts;
    }

    if (data[0] == '-')
    {
        auto name = read_until_whitespace(data.substr(1));
        auto value = read_until_whitespace(name.data_left);

        if (name.result and value.result)
        {
            opts.options.emplace(*name.result, *value.result);
        }
        else if (name.result)
        {
            opts.switches.emplace(*name.result);
        }
        else
            throw 1;

        std::cout << "name: " << *name.result << "=" << *value.result << std::endl;

        return parse_option(value.data_left, opts);
    }

    throw 2;
}

std::map<std::string, std::string> parse(int argc, const char* argv[])
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

    return {};
}

}
