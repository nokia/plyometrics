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

    bool has;
    T data;
};

struct parse_result
{
    std::string result;
    std::string data_left;
};

using maybe_result = maybe<parse_result>;

struct options
{
    std::set<std::string> switches;
    std::map<std::string, std::string> options;
};

auto read_until_whitespace(const std::string& data_left, const std::string& read_so_far = "") -> maybe_result
{
    if (data_left.empty())
    {
        return parse_result{read_so_far, data_left};
    }

    if (data_left[0] == ' ')
    {
        return parse_result{read_so_far, data_left.substr(1)};
    }

    return read_until_whitespace(data_left.substr(1), read_so_far + data_left[0]);
}

auto parse_option(const std::string& data, options opts = options{}) -> maybe<options>
{
    if (data.empty())
    {
        return none;
    }

    if (data[0] == '-')
    {
        auto name = read_until_whitespace(data.substr(1));
        auto value = read_until_whitespace(name.data.data_left);

        if (name and value)
        {
            opts.options.emplace(name.data.result, value.data.result);
        }
        else if (name)
        {
            opts.switches.emplace(name.data.result);
        }
        else
            throw 1;

        std::cout << "name: " << name.data.result << "=" << value.data.result << std::endl;

        return parse_option(value.data.data_left, opts);
    }

    throw 2;
}

std::map<std::string, std::string> parse(int argc, const char* argv[])
{
    std::stringstream ss;

    for (int i = 0; i < argc; i++)
        ss << argv[i] << " ";

    const auto app = read_until_whitespace(ss.str());
    const auto opts = parse_option(app.data.data_left);

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
