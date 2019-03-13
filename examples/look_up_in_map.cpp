#include "plyometrics/nbench.hpp"

#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using containers_spec = plyometrics::spec::with_range<1, 128>;
using associative_containers_spec = containers_spec::with_types<std::map<int, int>, std::unordered_map<int, int>>;
using sequence_containers_spec = containers_spec::with_types<std::vector<int>, std::list<int>, std::set<int>, std::unordered_set<int>>;

NBENCHMARK_P(lookup_associative_containers, associative_containers_spec)
{
    auto sequence_data = plyometrics::sequence_range(loop.number());
    auto container = loop.type();
    for(auto i:sequence_data)
        container.emplace(i,i);

    while (loop)
        for(auto i : sequence_data)
            plyometrics::use(container[i]);
}

NBENCHMARK_P(constructing_sequence_containers, sequence_containers_spec)
{
    auto sequence_data = plyometrics::sequence_range(loop.number());

    while (loop)
        plyometrics::use(loop.type(sequence_data.begin(), sequence_data.end()));
}

NBENCHMARK_P(iterating_though_sequence_containers, sequence_containers_spec)
{
    auto sequence_data = plyometrics::sequence_range(loop.number());
    auto container = loop.type(sequence_data.begin(), sequence_data.end());

    while (loop)
        for(auto i : container)
            plyometrics::use(i);
}

int main(int argc, const char* argv[])
{
    plyometrics::run_all(argc, argv);
}
