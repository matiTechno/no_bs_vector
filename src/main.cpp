#include <iostream>
#include "vector.hpp"
#include "test_class.hpp"
#include <vector>
#include <chrono>
#include <string>
#include <iomanip>

class Timer
{
public:
    Timer(const std::string& name, const std::string& action, std::size_t times):
        name(name),
        action(action),
        times(times)
    {
        time1 = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        auto time2 = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration<float>(time2 - time1).count();
        std::cout << std::left
                  << std::setw(10) << name
                  << std::setw(40) << action
                  << std::setw(15) << times
                  << "time(s): " << time << std::endl;
    }

private:
    std::string name, action;
    std::size_t times;
    std::chrono::time_point<std::chrono::high_resolution_clock> time1;
};

template<typename T1, typename T2>
void compare(const std::string& name1, const std::string& name2, const std::string& action,
             std::size_t times, T1&& func1, T2&& func2)
{
    std::cout << std::endl;
    {
        Timer t(name1, action, times);
        (void)t;
        for(std::size_t i = 0; i < times; ++i)
            func1();
    }
    {
        Timer t(name2, action, times);
        (void)t;
        for(std::size_t i = 0; i < times; ++i)
            func2();
    }
}

void compare_push_back_copy(std::size_t i);
void compare_push_back_copy_reserved(std::size_t i);
void compare_push_back_move(std::size_t i);
void compare_emplace_back(std::size_t i);
void compare_insert_at_begin_reserved(std::size_t i);
void compare_erase_at_begin(std::size_t i);
void compare_copy_constructor(std::size_t i, std::size_t vec_size);
void compare_copy_assignment(std::size_t lhs_size, std::size_t rhs_size);
void compare_incremented_reserve(std::size_t i, std::size_t vec_size);

int main()
{
    compare_push_back_copy(1'000'000);
    compare_push_back_copy_reserved(1'000'000);
    compare_push_back_move(1'000'000);
    compare_emplace_back(1'000'000);
    compare_insert_at_begin_reserved(10000);
    compare_erase_at_begin(10000);
    compare_copy_constructor(10, 100000);
    compare_copy_assignment(100, 10000);
    compare_copy_assignment(10000, 100);
    compare_incremented_reserve(100, 10000);
    std::cout << std::endl;
    return 0;
}

// ### impl

void compare_push_back_copy(std::size_t i)
{
    mtech::vector<Test_class> mtech_vector;
    std::vector<Test_class> std_vector;
    compare("mtech", "stl", "push back copy", i,
            [&mtech_vector]()
    {
        Test_class obj;
        mtech_vector.push_back(obj);
    }
    ,[&std_vector]()
    {
        Test_class obj;
        std_vector.push_back(obj);
    }
    );
}

void compare_push_back_copy_reserved(std::size_t i)
{
    mtech::vector<Test_class> mtech_vector;
    mtech_vector.reserve(i);
    std::vector<Test_class> std_vector;
    std_vector.reserve(i);
    compare("mtech", "stl", "push back copy reserved", i,
            [&mtech_vector]()
    {
        Test_class obj;
        mtech_vector.push_back(obj);
    }
    ,[&std_vector]()
    {
        Test_class obj;
        std_vector.push_back(obj);
    }
    );
}

void compare_push_back_move(std::size_t i)
{
    mtech::vector<Test_class> mtech_vector;
    std::vector<Test_class> std_vector;
    compare("mtech", "stl", "push back move", i,
            [&mtech_vector]()
    {
        mtech_vector.push_back(Test_class());
    }
    ,[&std_vector]()
    {
        std_vector.push_back(Test_class());
    }
    );
}

void compare_emplace_back(std::size_t i)
{
    mtech::vector<Test_class> mtech_vector;
    std::vector<Test_class> std_vector;
    compare("mtech", "stl", "emplace back", i,
            [&mtech_vector]()
    {
        mtech_vector.emplace_back(10, 5.f, true);
    }
    ,[&std_vector]()
    {
        std_vector.emplace_back(10.f, 5.f, true);
    }
    );
}

void compare_insert_at_begin_reserved(std::size_t i)
{
    mtech::vector<Test_class> mtech_vector;
    mtech_vector.reserve(i);
    std::vector<Test_class> std_vector;
    std_vector.reserve(i);
    compare("mtech", "stl", "insert at begin reserved", i,
            [&mtech_vector]()
    {
        mtech_vector.insert(mtech_vector.begin(), Test_class());
    }
    ,[&std_vector]()
    {
        std_vector.insert(std_vector.begin(), Test_class());
    }
    );
}

void compare_erase_at_begin(std::size_t i)
{
    mtech::vector<Test_class> mtech_vector(i);
    std::vector<Test_class> std_vector(i);
    compare("mtech", "stl", "erase at begin", i,
            [&mtech_vector]()
    {
        mtech_vector.erase(mtech_vector.begin());
    }
    ,[&std_vector]()
    {
        std_vector.erase(std_vector.begin());
    }
    );
}

void compare_copy_constructor(std::size_t i, std::size_t vec_size)
{
    mtech::vector<Test_class> mtech_vector(vec_size);
    std::vector<Test_class> std_vector(vec_size);
    compare("mtech", "stl", "copy constructor [size = " + std::to_string(vec_size) + ']', i,
            [&mtech_vector]()
    {
        auto new_vec = mtech_vector;
        (void)new_vec;
    }
    ,[&std_vector]()
    {
        auto new_vec = std_vector;
        (void)new_vec;
    }
    );
}

void compare_copy_assignment(std::size_t lhs_size, std::size_t rhs_size)
{
    std::cout << std::endl;
    mtech::vector<Test_class> mtech_lhs(lhs_size), mtech_rhs(rhs_size);
    std::vector<Test_class> std_lhs(lhs_size), std_rhs(rhs_size);
    {
        Timer t("mtech", "copy assignment: [" + std::to_string(lhs_size) + "] = [" + std::to_string(rhs_size) + ']', 1);
        (void)t;
        mtech_lhs = mtech_rhs;
    }
    {
        Timer t("stl", "copy assignment: [" + std::to_string(lhs_size) + "] = [" + std::to_string(rhs_size) + ']', 1);
        (void)t;
        std_lhs = std_rhs;
    }
}

void compare_incremented_reserve(std::size_t i, std::size_t vec_size)
{
    std::size_t count1 = vec_size, count2 = vec_size;
    mtech::vector<Test_class> mtech_vector(vec_size);
    std::vector<Test_class> std_vector(vec_size);
    compare("mtech", "stl", "incremented reserve [size = " + std::to_string(vec_size) + ']', i,
            [&mtech_vector, &count1]()
    {
        mtech_vector.reserve(count1);
        ++count1;
    }
    ,[&std_vector, &count2]()
    {
        std_vector.reserve(count2);
        ++count2;
    }
    );
}
