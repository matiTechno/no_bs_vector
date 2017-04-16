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

    mtech::vector<Test_class> vec(100);

    SECTION("copy constructor")
    {
        auto new_vec = vec;
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
    }
    SECTION("move constructor")
    {
        auto new_vec = std::move(vec);
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
        REQUIRE(vec.size() == 0);
    }
    SECTION("copy assignment")
    {
        mtech::vector<Test_class> new_vec;
        new_vec = vec;
        REQUIRE(new_vec.capacity() >= 100);
        REQUIRE(new_vec.size() == 100);
    }
    SECTION("copy assignment, bigger to smaller")
    {
        mtech::vector<Test_class> new_vec(1000);
        new_vec = vec;
        REQUIRE(new_vec.capacity() >= 1000);
        REQUIRE(new_vec.size() == 100);
    }
    SECTION("move assignment")
    {
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
TEST_CASE("element access")
{
    mtech::vector<int> vec{1, 2, 3, 4, 5, 10, 666};
    REQUIRE(*vec.data() == 1);
    REQUIRE(vec.front() == 1);
    REQUIRE(vec.back() == 666);
    REQUIRE(vec[3] == 4);
}
TEST_CASE("modifiers")
{
    mtech::vector<Test_class> vec{Test_class("lolz"), Test_class("hello"), Test_class("wtf"), Test_class("bravo")};

    SECTION("clear")
    {
        vec.clear();
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() >= 4);
    }
    SECTION("pop back")
    {
        vec.pop_back();
        REQUIRE(vec.size() == 3);
        REQUIRE(vec.capacity() >= 4);
        REQUIRE(vec.back().lol == "wtf");
    }
    SECTION("push back")
    {
        vec.push_back(Test_class("new"));
        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() >= 5);
        REQUIRE(vec.back().lol == "new");
    }
    SECTION("emplace back")
    {
        vec.emplace_back("emplace");
        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() >= 5);
        REQUIRE(vec.back().lol == "emplace");
    }
    SECTION("insert")
    {
        auto it = vec.insert(vec.begin() + 1, Test_class("xd"));
        REQUIRE(it == vec.begin() + 1);
        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() >= 5);
        REQUIRE(it->lol == "xd");
        REQUIRE(it == vec.begin() + 1);

        auto it2 = vec.insert(vec.begin(), Test_class("xd2"));
        REQUIRE(it2 == vec.begin());
        REQUIRE(it2->lol == "xd2");

        auto it3 = vec.insert(vec.end(), Test_class("xd3"));
        REQUIRE(it3 == vec.end() - 1);
        REQUIRE(it3->lol == "xd3");

        REQUIRE(vec.size() == 7);
        REQUIRE(vec.capacity() >= 7);
    }
    SECTION("emplace")
    {
        auto it = vec.emplace(vec.begin() + 1, "xd");
        REQUIRE(it == vec.begin() + 1);
        REQUIRE(vec.size() == 5);
        REQUIRE(vec.capacity() >= 5);
        REQUIRE(it->lol == "xd");
        REQUIRE(it == vec.begin() + 1);

        auto it2 = vec.emplace(vec.begin(), "xd2");
        REQUIRE(it2 == vec.begin());
        REQUIRE(it2->lol == "xd2");

        auto it3 = vec.emplace(vec.end(), "xd3");
        REQUIRE(it3 == vec.end() - 1);
        REQUIRE(it3->lol == "xd3");

        REQUIRE(vec.size() == 7);
        REQUIRE(vec.capacity() >= 7);
    }
    SECTION("erase")
    {
        auto it = vec.erase(vec.end() - 1);
        REQUIRE(vec.size() == 3);
        REQUIRE(it == vec.end());
        REQUIRE((it - 1)->lol == "wtf");

        auto it2 = vec.erase(vec.begin());
        REQUIRE(it2 == vec.begin());
        REQUIRE(it2->lol == "hello");
        REQUIRE(vec.size() == 2);

        REQUIRE(vec.capacity() >= 4);
    }
    SECTION("erase all")
    {
        auto it = vec.erase(vec.begin(), vec.end() - 1);
        REQUIRE(vec.size() == 0);
        REQUIRE(it == vec.begin());
        REQUIRE(it == vec.end());
        REQUIRE(vec.capacity() >= 4);
    }
    SECTION("erase some")
    {
        auto it = vec.erase(vec.begin() + 1, vec.begin() + 2);
        REQUIRE(vec.size() == 2);
        REQUIRE(it == vec.begin() + 1);
        REQUIRE(it->lol == "bravo");
        REQUIRE(vec.begin()->lol == "lolz");
        REQUIRE(vec.capacity() >= 4);
    }
}
