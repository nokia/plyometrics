#include "catch.hpp"

#include "plyometrics/utils.hpp"

#include <utility>
#include <type_traits>
#include <cxxabi.h>

using namespace plyometrics;

struct type_visitor
{
    template<class T> void accept();

    bool got_int = false;
    bool got_float = false;
    bool got_char = false;
};

template<> void type_visitor::accept<int>()
{
    got_int = true;
}

template<> void type_visitor::accept<float>()
{
    got_float = true;
}

template<> void type_visitor::accept<char>()
{
    got_char = true;
}

TEST_CASE("tuple foreach")
{
    using my_tuple = std::tuple<int, float, char>;
    type_visitor visitor;

    visit_each_type<my_tuple>(visitor);

    CHECK(visitor.got_int);
    CHECK(visitor.got_float);
    CHECK(visitor.got_char);
}

// static stuff tests

template<class T>
std::string demangle()
{
    int status;
    char* realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    std::string ret = realname;
    free(realname);
    return ret;
}

TEST_CASE("append index_sequence")
{
    using base = std::index_sequence<1, 2>;
    using expected = std::index_sequence<1, 2, 3>;
    using computed = details::append_to_index_sequence<base, 3>::type;
    CHECK(demangle<expected>() == demangle<computed>());
}

TEST_CASE("static geometric range")
{
    using expected = std::index_sequence<1, 2, 4, 8>;
    using computed = power_of_2_sequence<1, 2, 4>;
    CHECK(demangle<expected>() == demangle<computed>());
}

TEST_CASE("static geometric range with minimum value")
{
    using expected = std::index_sequence<1, 10, 100, 1000>;
    using computed = power_of_2_sequence<1, 10, 4>;
    CHECK(demangle<expected>() == demangle<computed>());
}