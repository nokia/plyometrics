#include "catch.hpp"

#include "plyometrics/cmdline.hpp"

TEST_CASE("switch is recognized")
{
    const char* argv[] = {"app", "-a"};
    const auto options = plyometrics::parse_options(2, argv);
    CAPTURE(options);

    CHECK(options.has_switch("-a"));
}

TEST_CASE("long switch is recognized")
{
    const char* argv[] = {"app", "--long"};
    const auto options = plyometrics::parse_options(2, argv);
    CAPTURE(options);

    CHECK(options.has_switch("--long"));
}

TEST_CASE("named option is recognized")
{
    const char* argv[] = {"app", "-a", "a value"};
    const auto options = plyometrics::parse_options(3, argv);
    CAPTURE(options);

    CHECK(*options.option("-a") == "a value");
}

TEST_CASE("long named option is recognized")
{
    const char* argv[] = {"app", "--long", "a value"};
    const auto options = plyometrics::parse_options(3, argv);
    CAPTURE(options);

    CHECK(*options.option("--long") == "a value");
}

TEST_CASE("combined options are suported")
{
    const char* argv[] = {"app", "-a", "-b",
                                 "-c", "c value",
                                 "--long_a",
                                 "--long_b", "long_b value"};
    const auto options = plyometrics::parse_options(8, argv);
    CAPTURE(options);

    CHECK(options.has_switch("-a"));
    CHECK(options.has_switch("-b"));
    CHECK(*options.option("-c") == "c value");
    CHECK(options.has_switch("--long_a"));
    CHECK(*options.option("--long_b") == "long_b value");
}
