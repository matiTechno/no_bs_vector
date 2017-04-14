add_executable(main_test main.cpp catch.hpp)
add_test(NAME my_test COMMAND main_test)
