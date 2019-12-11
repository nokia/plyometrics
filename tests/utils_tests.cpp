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

template<class, std::size_t>
struct append_to_index_sequence;

template<std::size_t V, std::size_t... Idx>
struct append_to_index_sequence<std::index_sequence<Idx...>, V>
{
    using type = std::index_sequence<Idx..., V>;
};

TEST_CASE("append index_sequence")
{
    using base = std::index_sequence<1, 2>;
    using expected = std::index_sequence<1, 2, 3>;
    using computed = append_to_index_sequence<base, 3>::type;
    CHECK(demangle<expected>() == demangle<computed>());
}

template<bool Last, std::size_t Current, std::size_t To, class Is>
struct geometric_range_impl
{
    using type = typename geometric_range_impl<
        (Current > To),
        Current * 2,
        To,
        typename append_to_index_sequence<Is, Current>::type
    >::type;
};

template<std::size_t Current, std::size_t To, class Is>
struct geometric_range_impl<true, Current, To, Is>
{
    using type = Is;
};

template<std::size_t To>
using geometric_range = typename geometric_range_impl<false, 1, To, std::index_sequence<>>::type;

TEST_CASE("static geometric range")
{
    using expected = std::index_sequence<1, 2, 4, 8, 16>;
    using computed = geometric_range<10>;
    CHECK(demangle<expected>() == demangle<computed>());
}