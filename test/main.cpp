#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "vector.hpp"
#include "test_class.hpp"

// todo: test exception guarantees

TEST_CASE("constructing and assigning vectors")
{
    SECTION("default constructor")
    {
        mtech::vector<Test_class> vec;
        REQUIRE(vec.capacity() == 1);
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.empty() == true);
    }
    SECTION("constructor with init size")
    {
        mtech::vector<Test_class> vec(100);
        REQUIRE(vec.capacity() >= 100);
        REQUIRE(vec.size() == 100);
    }
    SECTION("constructor with initializer list")
    {
        mtech::vector<Test_class> vec{Test_class(), Test_class(), Test_class()};
        REQUIRE(vec.capacity() >= 3);
        REQUIRE(vec.size() == 3);
    }
    SECTION("copy constructor")
    {
        mtech::vector<Test_class> vec(100);
        auto new_vec = vec;
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
    }
    SECTION("move constructor")
    {
        mtech::vector<Test_class> vec(100);
        auto new_vec = std::move(vec);
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
        REQUIRE(vec.size() == 0);
    }
    SECTION("copy assignment")
    {
        mtech::vector<Test_class> vec(100);
        mtech::vector<Test_class> new_vec;
        new_vec = vec;
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
    }
    SECTION("move assignment")
    {
        mtech::vector<Test_class> vec(100);
        mtech::vector<Test_class> new_vec;
        new_vec = std::move(vec);
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
        REQUIRE(vec.size() == 0);
    }
}
TEST_CASE("capacity functions")
{
    mtech::vector<Test_class> vec(100);

    SECTION("increasing capacity")
    {
        vec.reserve(1000);
        REQUIRE(vec.size() == 100);
        REQUIRE(vec.capacity() >= 1000);
    }
    SECTION("reserve with smaller capacity")
    {
        vec.reserve(10);
        REQUIRE(vec.size() == 100);
        REQUIRE(vec.capacity() >= 100);
    }
    SECTION("empty function")
    {
        REQUIRE(vec.empty() == false);
    }
}
TEST_CASE("iterators")
{
    mtech::vector<int> vec{69, 666, 55, 1};
    REQUIRE(*vec.begin() == 69);
    REQUIRE(*(vec.end() - 1) == 1);
    REQUIRE(vec.begin() - vec.end() == -4);
    REQUIRE(*(vec.begin() + 1) == 666);
    REQUIRE(vec.begin() + vec.size() == vec.end());

    SECTION("empty vector")
    {
        mtech::vector<int> vec{};
        REQUIRE(vec.begin() == vec.end());
        auto it = vec.insert(vec.end(), 8);
        REQUIRE(it == vec.begin());
        REQUIRE(*it == 8);
    }
}
