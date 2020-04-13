#pragma once
#include "iostream"

template <typename T>
struct MyAllocator
{
    using value_type        = T;
    using base              = std::allocator<T>;

    using alloc_traits      = std::allocator_traits<base>;


public:
    static size_t count_allocate;
    static size_t count_deallocate;
    static size_t count_construct;
    static size_t count_destroy;


public:
    static value_type* allocate(size_t size)
    {
        ++count_allocate;
        std::cout << "allocate memory" << std::endl;
        return std::allocator<value_type>().allocate(size);
    }

    static void deallocate(value_type* pointer, size_t size)
    {
        ++count_deallocate;
        std::cout << "deallocate memory" << std::endl;
        std::allocator<value_type>().deallocate(pointer, size);
    }

    template <class... types>
    static void construct(value_type* pointer, types&&... args)
    {
        ++count_construct;
        std::cout << "construct" << std::endl;
        alloc_traits::construct(std::allocator<value_type>(), pointer, std::forward<types>(args)...);
    }

    static void destroy(value_type* pointer)
    {
        ++count_destroy;
        std::cout << "destroy" << std::endl;
        alloc_traits::destroy(std::allocator<value_type>(), pointer);
    }

};

template<typename T>
size_t MyAllocator<T>::count_allocate = 0;
template<typename T>
size_t MyAllocator<T>::count_deallocate = 0;
template<typename T>
size_t MyAllocator<T>::count_construct = 0;
template<typename T>
size_t MyAllocator<T>::count_destroy = 0;
