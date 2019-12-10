#include "plyometrics/plyometrics.hpp"

constexpr std::size_t complexity = 10000;

template<class... Args>
void swallow(Args...)
{
}

struct fancy_abstract_class
{
    virtual void foo() = 0;
    virtual ~fancy_abstract_class() = default;
};

struct fancy_class final : public fancy_abstract_class
{
    __attribute__((noinline)) void foo() override
    {
        asm volatile("");
    }
};

auto create_objects(std::size_t size)
{
    std::vector<std::unique_ptr<fancy_abstract_class>> ret(size);
    std::generate(ret.begin(), ret.end(), [] { return std::make_unique<fancy_class>(); });
    return ret;
}

template<std::size_t>
__attribute__((noinline)) void call(fancy_abstract_class& object)
{
    object.foo();
}

template<class Objects, std::size_t... Idx>
__attribute__((noinline)) void work(const Objects& objects, std::index_sequence<Idx...>)
{
    swallow((call<Idx>(*objects[Idx]), int{})...);
}

// Usually you don't want to compare pointers to the objects returned by typeid
// because it's not guaranteed that they will be always the same for the same type.
// For example, shared library might generate its own typeinfo. For us this is ok though
// because the worst thing will be that we miss first condition and do regular call.
#define HINTED_CALL(object, likely, call) \
    if (__builtin_expect(&typeid(object) == &typeid(likely), true)) \
        static_cast<likely&>(object).call; \
    else \
        object.call; \

template<std::size_t, class Likely>
__attribute__((noinline)) void hinted_call(fancy_abstract_class& object)
{
    HINTED_CALL(object, Likely, foo());
}

template<class Objects, std::size_t... Idx>
__attribute__((noinline)) void hinted_work(const Objects& objects, std::index_sequence<Idx...>)
{
    swallow((hinted_call<Idx, fancy_class>(*objects[Idx]), int{})...);
} 

NBENCHMARK(dynamic_branch_prediction)
{
    auto objects = create_objects(complexity);

    while (loop)
        work(objects, std::make_index_sequence<complexity>());
}

NBENCHMARK(hinted_branch_prediction)
{
    auto objects = create_objects(complexity);

    while (loop)
        hinted_work(objects, std::make_index_sequence<complexity>());
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}