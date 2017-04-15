cmake_minimum_required (VERSION 2.6)
project(no_bs_vector)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
# -03 produces warning
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(./src)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)
