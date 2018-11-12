#include <vector>
#include <functional>

namespace nbench
{

struct loop
{
    operator bool()
    {
        return true;
    }
};

template<class F>
void benchmark(F&& f)
{
    loop l;
    f(l);
}

}
