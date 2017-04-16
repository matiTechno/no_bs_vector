#pragma once

#include <cstring>
#include <string>

class Test_class
{
public:
    friend void swap(Test_class& t1, Test_class& t2)
    {
        using std::swap;
        swap(t1.lol, t2.lol);
        swap(t1.mem, t2.mem);
    }
    Test_class(int, float, bool):
        Test_class(){}
    Test_class():
        mem(new char[100]){}
    Test_class(const std::string& string):
        Test_class()
    {lol = string;}
    ~Test_class()
    {delete[] mem;}
    Test_class(const Test_class& rhs):
        Test_class()
    {
        std::memcpy(mem, rhs.mem, sizeof(char) * 100);
        lol = rhs.lol;
    }
    Test_class(Test_class&& rhs) noexcept:
        lol(std::move(rhs.lol)),
        mem(rhs.mem)
    {rhs.mem = nullptr;}
    Test_class& operator=(Test_class rhs) noexcept
    {
        swap(*this, rhs);
        return *this;
    }
    std::string lol{"lol"};

private:
    char* mem;
};
