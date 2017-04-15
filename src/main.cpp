#include <iostream>
#include "vector.hpp"
#include <string>
#include <chrono>
#include <vector>
#include <memory>

class My_class
{
public:
    int x = 55;
    short zz = 1000, xx;
    std::string string = "hello";
    bool lol;
    bool xw = false;
    double d1, d2, d3, d4;
};

class Lol
{
public:
    Lol(const std::string& string, int x, bool y):
        string(string),
        x(x),
        y(y),
        vec(100, 100)
    {
        y = !y;
        ptr = std::make_shared<double>(100);
    }
    std::string string;
    int x;
    bool y;
    std::vector<int> vec;
    std::shared_ptr<double> ptr;
};

int main()
{
    mtech::vector<My_class> mtech_vector;
    mtech_vector.reserve(100);
    std::vector<My_class> stl_vector;
    stl_vector.reserve(100);
    {
        auto time = std::chrono::high_resolution_clock::now();

        for(std::size_t i = 0; i < 1000; ++i)
            mtech_vector.push_back(My_class());

        auto new_time = std::chrono::high_resolution_clock::now();
        std::cout << "mtech time(s): " << std::chrono::duration<float>(new_time - time).count() << std::endl;
    }
    {
        auto time = std::chrono::high_resolution_clock::now();

        for(std::size_t i = 0; i < 1000; ++i)
            stl_vector.push_back(My_class());

        auto new_time = std::chrono::high_resolution_clock::now();
        std::cout << "stl time(s): " << std::chrono::duration<float>(new_time - time).count() << std::endl;
    }

    auto new_vec1 = mtech_vector;
    auto new_vec2 = std::move(new_vec1);
    new_vec2.insert(new_vec2.begin() + 1, My_class());


    mtech::vector<int> ivec{10, 20, 30, 50, 60};
    ivec.insert(ivec.begin() + 1, 666);
    ivec.emplace(ivec.end() - 1, 69);
    for(auto& i: ivec)
        std::cout << i << std::endl;

    // test if all functions compile
    {
        mtech::vector<Lol> test;
        test = mtech::vector<Lol>(1000, Lol("x", 5, true));
        test = mtech::vector<Lol>{Lol("x", 5, true), Lol("x", 5, true), Lol("x", 5, true), Lol("x", 5, true)};
        auto new_vec = test;
        test = new_vec;
        test = std::move(new_vec);
        auto it1 = test.begin();
        it1 = test.end();
        auto it2 = test.cbegin();
        it2 = test.cend();
        test.reserve(1000);
        test.empty();
        test.size();
        test.capacity();
        auto* x = test.data();
        (void)x;
        x = &test.back();
        const auto* y = test.data();
        (void)y;
        y = &test.back();
        y = &test[0];
        x = &test[0];
        y = &test.front();
        x = &test.front();
        test.insert(test.begin() + 2, Lol("x", 5, true));
        test.emplace(test.begin() + 2, "x", 5, true);
        test.erase(test.begin());
        test.erase(test.begin(), test.end() - 3);
        test.push_back(Lol("lol", 10, false));
        test.emplace_back("xxx", 20, true);
        test.pop_back();
        auto b(it1 == it2);
        (void)b;
        b = it1 != it2;
        b = it1 < it2;
        b = it1 > it2;
        b = it1 <= it2;
        b = it1 >= it2;
        ++it2;
        --it2;
        it2 += 5;
        it2 - 5;
        it2 + 5;
        it2 -= 5;
        it2 - 5;
        it2 - it1;
        *it2;
        auto z = test.begin()->x;
        ++z;
        test.clear();
    }
    std::cout << "Hello World!" << std::endl;
    return 0;
}
