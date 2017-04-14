#pragma once

#include <cstddef>
#include <iterator>
#include <initializer_list>
#include <new>
#include "assert.h"

namespace mtech
{

// to do: case of allocation failure
// ...... object creation failure
// testing
// emplecing and exception safety
// some definitions are outside class to avoid syntax bugs
// (I don't care about inlining stuff for now)
// why scope resolution operator is not needed for iterator as parameter type
// in vector functions outside class definition
template<typename T>
class vector
{
public:
    class iterator;
    class const_iterator;

    vector():
        m_capacity(1),
        m_size(0)
    {
        mem = static_cast<T*>(operator new(m_capacity * sizeof(T)));
    }
    explicit vector(std::size_t count, const T& value = T()):
        vector()
    {
        reserve(count);
        for(std::size_t i = 0; i < count; ++i)
            push_back(value);
    }

    vector(std::initializer_list<T> init):
        vector()
    {
        reserve(init.size());
        for(auto& item: init)
            push_back(std::move(item));
    }

    vector(const vector& rhs):
        vector()
    {
        reserve(rhs.m_capacity);
        for(auto it = rhs.cbegin(); it != rhs.cend(); ++it)
            push_back(*it);
    }

    vector(vector&& rhs):
        mem(rhs.mem),
        m_capacity(rhs.m_capacity),
        m_size(rhs.m_size)
    {
        rhs.m_size = 0;
        rhs.mem = nullptr;
    }

    vector& operator=(const vector& rhs)
    {
        if(this == &rhs)
            return *this;

        if(rhs.m_capacity > m_capacity)
            reserve(rhs.m_capacity);

        std::size_t i = 0;
        for(; i < m_size && rhs.m_size; ++i)
            operator[](i) = rhs[i];
        for(; i < rhs.m_size; ++i)
            push_back(rhs[i]);
        for(; i < m_size; ++i)
            operator[](i).~T();

        return *this;
    }

    vector& operator=(vector&& rhs)
    {
        if(this == &rhs)
            return *this;

        this->~vector();
        mem = rhs.mem;
        m_capacity = rhs.m_capacity;
        m_size = rhs.m_size;

        rhs.m_size = 0;
        rhs.mem = nullptr;

        return *this;
    }

    ~vector()
    {
        for(auto it = begin(); it != end(); ++it)
            it->~T();
        ::operator delete(mem);
    }

    // iterators
    iterator begin()
    {return iterator(mem);}

    const_iterator cbegin() const
    {return const_iterator(mem);}

    iterator end()
    {return iterator(mem + m_size);}

    const_iterator cend() const
    {return const_iterator(mem + m_size);}

    // capacity
    void reserve(std::size_t new_cap)
    {
        if(m_capacity >= new_cap)
            return;

        T* temp_mem = static_cast<T*>(operator new(new_cap * sizeof(T)));

        for(std::size_t i = 0; i < m_size; ++i)
            new(temp_mem + i) T(std::move(operator[](i)));

        this->~vector();
        mem = temp_mem;
        m_capacity = new_cap;
    }

    bool empty() const
    {return !m_size;}
    std::size_t size() const
    {return m_size;}
    std::size_t capacity() const
    {return m_capacity;}

    // element access
    T* data()
    {return mem;}

    const T* data() const
    {return mem;}

    T& front()
    {return *mem;}

    const T& front() const
    {return *mem;}

    T& back()
    {return *(mem + m_size - 1);}

    const T& back() const
    {return *(mem + m_size - 1);}

    T& operator[](std::size_t pos)
    {return *(mem + pos);}

    const T& operator[](std::size_t pos) const
    {return *(mem + pos);}

    // modifiers
    void clear()
    {
        for(auto it = begin(); it != end(); ++it)
            it->~T();
        m_size = 0;
    }

    template<typename U>
    iterator insert(const_iterator pos, U&& item);

    // decltype(auto) just 4fun
    template<typename ...Args>
    decltype(auto) emplace(const_iterator pos, Args&&... params);

    iterator erase(const_iterator pos);

    iterator erase(const_iterator first, const_iterator last);

    template<typename U>
    void push_back(U&& item)
    {
        if_full();
        new(mem + m_size) T(std::forward<U>(item));
        ++m_size;
    }

    template<typename ...Args>
    void emplace_back(Args&&... params)
    {
        if_full();
        new(mem + m_size) T(std::forward<Args>(params)...);
        ++m_size;
    }

    void pop_back()
    {
        (mem + m_size - 1)->~T();
        --m_size;
    }

private:
    T* mem;
    std::size_t m_capacity;
    std::size_t m_size;

    void if_full()
    {
        if(m_size == m_capacity)
            reserve(m_capacity * 2 + 1);
    }

    void shift_to_end_by_one(const_iterator last_excluded);
};

template<typename T>
class vector<T>::const_iterator
{
public:
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef const T* pointer;
    typedef const T& reference;
    typedef std::random_access_iterator_tag iterator_category;

    const_iterator() = default;
    const_iterator(T* ptr):
        ptr(ptr)
    {}

    bool operator==(const const_iterator& rhs) const
    {return ptr == rhs.ptr;}
    bool operator!=(const const_iterator& rhs) const
    {return ptr != rhs.ptr;}
    bool operator<(const const_iterator& rhs) const
    {return ptr < rhs.ptr;}
    bool operator>(const const_iterator& rhs) const
    {return ptr > rhs.ptr;}
    bool operator<=(const const_iterator& rhs) const
    {return ptr <=rhs.ptr;}
    bool operator>=(const const_iterator& rhs) const
    {return ptr >= rhs.ptr;}
    const_iterator& operator++()
    {++ptr; return *this;}
    const_iterator& operator--()
    {--ptr; return *this;}
    const_iterator& operator+=(std::size_t count)
    {ptr += count; return *this;}
    const_iterator operator+(std::size_t count) const
    {return ptr + count;}
    const_iterator& operator-=(std::size_t count)
    {ptr -= count; return *this;}
    const_iterator operator-(std::size_t count) const
    {return ptr - count;}
    std::ptrdiff_t operator-(const const_iterator& rhs)
    {return this->ptr - rhs.ptr;}
    const T& operator*() const
    {return *ptr;}
    const T* operator->() const
    {return ptr;}

protected:
    T* ptr;
};

template<typename T>
class vector<T>::iterator: public const_iterator
{
public:
    typedef T* pointer;
    typedef T& reference;

    iterator() = default;
    iterator(T* ptr):
        const_iterator(ptr)
    {}
    explicit iterator(const_iterator it):
        const_iterator(it)
    {}

    iterator& operator++()
    {++this->ptr;return *this;}
    iterator& operator--()
    {--this->ptr;return *this;}
    iterator& operator+=(std::size_t count)
    {this->ptr += count; return *this;}
    iterator operator+(std::size_t count) const
    {return this->ptr + count;}
    iterator& operator-=(std::size_t count)
    {this->ptr -= count; return *this;}
    iterator operator-(std::size_t count) const
    {return this->ptr - count;}
    T& operator*()
    {return const_cast<T&>(const_iterator::operator*());}
    T* operator->()
    {return const_cast<T*>(const_iterator::operator->());}
};

template<typename T>
void vector<T>::shift_to_end_by_one(const_iterator last_excluded)
{
    new(&*end()) T(std::move(*(end() - 1)));
    ++m_size;

    for(auto it = end() - 2; it != last_excluded; --it)
        *it = std::move(*(it - 1));
}

template<typename T>
template<typename U>
typename vector<T>::iterator vector<T>::insert(const_iterator pos, U&& item)
{
    if(pos == end())
    {
        if_full();
        push_back(std::forward<U>(item));
        return end() - 1;
    }
    auto diff = pos - begin();
    if_full();
    auto new_pos = begin() + diff;
    shift_to_end_by_one(new_pos);
    *new_pos = T(std::forward<U>(item));
    return new_pos;
}

template<typename T>
template<typename ...Args>
decltype(auto) vector<T>::emplace(const_iterator pos, Args&&... params)
{
    if(pos == end())
    {
        if_full();
        emplace_back(std::forward<Args>(params)...);
        return end() - 1;
    }
    auto diff = pos - begin();
    if_full();
    auto new_pos = begin() + diff;
    shift_to_end_by_one(new_pos);
    new_pos->~T();
    new(&*new_pos) T(std::forward<Args>(params)...);
    return new_pos;
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator pos)
{
    for(auto it = iterator(pos); it != end() - 1; ++it)
        *it = std::move(*(it + 1));
    (end() - 1)->~T();
    --m_size;

    return iterator(pos);
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
{
    auto to_del = last - first + 1;
    for(auto it = iterator(first); it != end() - to_del; ++it)
        *it = std::move(*(it + to_del));
    (end() - 1)->~T();
    m_size -= to_del;

    return iterator(first);
}

} // mtech
