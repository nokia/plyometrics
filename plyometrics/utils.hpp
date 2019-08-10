#pragma once

namespace plyometrics
{

struct range_t
{
    std::size_t from = 1, to = 1;
};

template<class... Args>
void swallow(Args&&...)
{
}

struct nothing
{
};

template<class type_sequence_type> struct visit_each_type_impl;

template<class... types>
struct visit_each_type_impl<std::tuple<types...>>
{
    template<class visitor_type>
    void visit(visitor_type&& visitor)
    {
        swallow([&]{ visitor.template accept<types>(); return 0; }()...);
    }
};

/**
 * Calls visitor_type::visit method for each type in tuple's type
 * without constructing anything.
 */
template<class type_sequence_type, class visitor_type>
void visit_each_type(visitor_type&& visitor)
{
    visit_each_type_impl<type_sequence_type> impl;
    impl.visit(visitor);
}

}
