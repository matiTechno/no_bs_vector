template<typename T>
vector<T>::~vector()
{
    T* it = mem;
    for(std::size_t i = 0; i < size; ++i)
    {
        it->~T();
        ++it;
    }
    ::operator delete(mem);
}
