#include "plyometrics/plyometrics.hpp"

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

struct regular
{
    template<std::size_t>
    __attribute__((noinline)) static void call(fancy_abstract_class& object)
    {
        object.foo();
    }

    template<class Objects, std::size_t... Idx>
    __attribute__((noinline)) static void work(const Objects& objects, std::index_sequence<Idx...>)
    {
        swallow((call<Idx>(*objects[Idx]), int{})...);
    }
};

struct hinted
{
    template<std::size_t, class Likely>
    __attribute__((noinline)) static void hinted_call(fancy_abstract_class& object)
    {
        // Usually you don't want to compare pointers to the objects returned by typeid
        // because it's not guaranteed that they will be always the same for the same type.
        // For example, shared library might generate its own typeinfo. For us this is ok though
        // because the worst thing will be that we miss first condition and do regular call.
        #define HINTED_CALL(object, likely, call) \
            if (__builtin_expect(&typeid(object) == &typeid(likely), true)) \
                static_cast<likely&>(object).call; \
            else \
                object.call; \

        HINTED_CALL(object, Likely, foo());

        #undef HINTED_CALL
    }

    template<class Objects, std::size_t... Idx>
    __attribute__((noinline)) static void work(const Objects& objects, std::index_sequence<Idx...>)
    {
        swallow((hinted_call<Idx, fancy_class>(*objects[Idx]), int{})...);
    } 
};

using branch_predictor_spec = plyometrics::spec::with_types<regular, hinted>::with_range<1, 2, 14>;

NBENCHMARK_P(dynamic_branch_prediction, branch_predictor_spec)
{
    auto objects = create_objects(loop.number());
    auto work = loop.type();

    while (loop)
        work.work(objects, std::make_index_sequence<loop.cnumber()>());
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}