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
    template<class visitor_type, class... args_types>
    void visit(visitor_type&& visitor, args_types&&... args)
    {
        swallow((visitor.template accept<types>(std::forward<args_types>(args)...), int{})...);
    }
};

/**
 * Calls visitor_type::visit method for each type in tuple's type
 * without constructing anything.
 */
template<class type_sequence_type, class visitor_type, class... args_types>
void visit_each_type(visitor_type&& visitor, args_types&&... args)
{
    visit_each_type_impl<type_sequence_type> impl;
    impl.visit(visitor, std::forward<args_types>(args)...);
}

namespace details
{

template<class, std::size_t>
struct append_to_index_sequence;

template<std::size_t V, std::size_t... Idx>
struct append_to_index_sequence<std::index_sequence<Idx...>, V>
{
    using type = std::index_sequence<Idx..., V>;
};

template<std::size_t Max, std::size_t Current, std::size_t Multiplier, class Is>
struct power_of_2_sequence_impl
{
    using type = typename power_of_2_sequence_impl<
        Max - 1,
        Current * Multiplier,
        Multiplier,
        typename append_to_index_sequence<Is, Current>::type
    >::type;
};

template<std::size_t Current, std::size_t Multiplier, class Is>
struct power_of_2_sequence_impl<0, Current, Multiplier, Is>
{
    using type = Is;
};

} // namespace details

template<std::size_t Base, std::size_t Multiplier, std::size_t Max> 
using power_of_2_sequence = typename details::power_of_2_sequence_impl<Max, Base, Multiplier, std::index_sequence<>>::type;

}
