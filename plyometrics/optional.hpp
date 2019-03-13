#pragma once

// std::optional is somehow new and in older compilers it's hidden
// under std::experimental

#ifdef __has_include
    #if __has_include(<optional>)

        #include <optional>

        namespace plyometrics
        {
            using std::optional;
            using std::nullopt;
        }

    #elif __has_include(<experimental/optional>)

        #include <experimental/optional>

        namespace plyometrics
        {
            using std::experimental::optional;
            using std::experimental::nullopt;
        }

    #else
        #error "can't locate any standard optional"
    #endif
#else
    #error "can't test for optional class"
#endif

