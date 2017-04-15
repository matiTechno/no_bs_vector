#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "vector.hpp"

TEST_CASE("size and capacity", "[vector]")
{
    mtech::vector<int> vec(6, 55);

    REQUIRE(vec.capacity() >= 6);
    REQUIRE(vec.size() == 6);

    SECTION("reserving bigger")
    {
        vec.reserve(100);

        REQUIRE(vec.capacity() >= 100);
        REQUIRE(vec.size() == 6);
    }

    SECTION("reserving smaller")
    {
        REQUIRE(vec.capacity() >= 6);
        REQUIRE(vec.size() == 6);
    }
}
