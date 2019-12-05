#pragma once

// std::optional is somehow new and in older compilers it's hidden
// under std::experimental

#if __cpp_lib_optional

#include <optional>

namespace plyometrics
{
    using std::optional;
    using std::nullopt;
}

#else

#include <experimental/optional>

namespace plyometrics
{
    using std::experimental::optional;
    using std::experimental::nullopt;
}

#endif

