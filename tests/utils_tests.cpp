#include "catch.hpp"

#include "plyometrics/utils.hpp"

#include <utility>

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
