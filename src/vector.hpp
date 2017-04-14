#pragma once

#include <cstddef>
#include <iterator>
#include <initializer_list>

namespace mtech
{

template<typename T>
class vector
{
public:
    class iterator;
    class const_iterator;

    vector();
    explicit vector(std::size_t count, const T& value = T());
    vector(std::initializer_list<T> init);

    vector(const vector& rhs);
    vector(vector&& rhs);
    vector& operator=(const vector& rhs);
    vector& operator=(vector&& rhs);
    ~vector();

    // iterators
    iterator begin();
    const iterator cbegin() const;
    iterator end();
    const iterator cend() const;

    // capacity
    void reserve(std::size_t new_cap);
    void shrink_to_fit();
    bool empty() const;
    std::size_t size() const;
    std::size_t capacity() const;

    // element access
    T* data();
    const T* data() const;
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T& at(std::size_t pos);
    const T& at(std::size_t pos) const;
    T& operator[](std::size_t pos);
    const T& operator[](std::size_t pos) const;

    // modifiers
    void clear();
    // insert
    // emplace
    // erase
    // push_back
    // emplace_back
    // pop_back
    // resize
    // swap ?

private:
    T* mem;
    std::size_t m_capacity;
    std::size_t m_size;
};

template<typename T>
class vector<T>::const_iterator
{
public:
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef std::random_access_iterator_tag iterator_category;

private:
};

template<typename T>
class vector<T>::iterator: public const_iterator
{
public:

private:
};

#include "vector_impl.hpp"

}
