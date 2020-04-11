#pragma once

#ifndef _CYCLIC_QUEUE_DYNAMIC_ARRAY_
#define _CYCLIC_QUEUE_DYNAMIC_ARRAY_

#include <yvals_core.h>
#include <xmemory>
#include <algorithm>

namespace test_tasks
{

    template <typename ElementType, class Alloc = std::allocator<ElementType>>
    class dynamic_cyclic_queue
    {
    public:
        using value_type        = ElementType;
        using elem_pointer      = value_type*;
        using index_t           = size_t;

        using allocator         = Alloc;
        using alloc_traits      = std::allocator_traits<allocator>;


    public:
        dynamic_cyclic_queue() noexcept
        {
            _elems = nullptr;
            _head = 0;
            _tail = 0;
            _count = 0;
            _size = 0;

            _alloc = std::allocator<value_type>();
        }

        explicit dynamic_cyclic_queue(size_t size, const allocator& alloc = std::allocator<value_type>())
        {
            _head = 0;
            _tail = 0;
            _count = 0;
            _size = size;

            // std::allocator default
            _alloc = alloc;

            // allocate
            _elems = _alloc.allocate(_size);
        }

        dynamic_cyclic_queue(const dynamic_cyclic_queue& right)
        {
            _elems = nullptr;
            _head = 0;
            _tail = 0;
            _count = 0;
            _size = 0;

            assign(right);
        }

        dynamic_cyclic_queue(dynamic_cyclic_queue&& right) noexcept
        {
            assign(std::move(right));
        }

        ~dynamic_cyclic_queue()
        {
            if (_elems == nullptr)
                return;

            alloc_traits::destroy(_alloc, _elems);
           _alloc.deallocate(_elems, _size);
        }


    public:
        dynamic_cyclic_queue& operator=(const dynamic_cyclic_queue& right)
        {
            assign(right);
            return *this;
        }

        dynamic_cyclic_queue& operator=(dynamic_cyclic_queue&& right) noexcept
        {
            assign(std::move(right));
            return *this;
        }

        void push(value_type&& elem)
        {
            emplace(std::move(elem));
        }

        void push(const value_type& elem)
        {
            emplace(elem);
        }

        _NODISCARD value_type pop()
        {
            if (empty())
                throw std::logic_error("Queue is empty!");

            value_type elem = std::move(_elems[_tail]);

            --_count;
            _tail = next_index(_tail, _size);
            return elem;
        }

        _NODISCARD size_t count() const noexcept
        {
            return _count;
        }

        _NODISCARD size_t size() const noexcept
        {
            return _size;
        }

        _NODISCARD bool full() const noexcept
        {
            return _count == _size;
        }

        _NODISCARD bool empty() const noexcept
        {
            return _count == 0;
        }

        void resize(size_t new_size)
        {
            if (new_size == _size)
                return;

            // allocate new memory
            elem_pointer new_elems = _alloc.allocate(new_size);
            // move all elements
            move(_elems, _size, _tail, _count, new_elems, new_size);

            // if elements exist
            if (_elems != nullptr)
            {
                // destroy their
                alloc_traits::destroy(_alloc, _elems);
                // and deallocate memory
                _alloc.deallocate(_elems, _size);
            }

            // set pointer to new array
            _elems = new_elems;

            // reset index
            _head = std::min(_count, new_size);
            _tail = 0;
            _size = new_size;
        }

        template <typename... Valty>
        void emplace(Valty&&... val)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            alloc_traits::construct(_alloc, _elems + _head, std::forward<Valty>(val)...);

            ++_count;
            _head = next_index(_head, _size);
        }

        void clear() noexcept
        {
            if (_elems == nullptr)
                return;

            alloc_traits::destroy(_alloc, _elems);
            _head = 0;
            _tail = 0;
            _count = 0;
        }

        _NODISCARD allocator get_allocator() const noexcept
        {
            return static_cast<allocator>(_alloc);
        }


    private:
        elem_pointer _elems;
        index_t _head;
        index_t _tail;

        size_t _size;
        size_t _count;

        allocator _alloc;


    private:
        size_t move(elem_pointer from, size_t from_size, index_t tail_index, size_t count_copy,
                    elem_pointer to,   size_t to_size) noexcept
        {
            index_t index = tail_index;
            size_t count_element_for_move = std::min(std::min(from_size, to_size), count_copy);

            for (size_t i = 0; i < count_element_for_move; ++i)
            {
                alloc_traits::construct(_alloc, to + i, std::move_if_noexcept(from[index]));
                index = next_index(index, from_size);
            }

            return count_element_for_move;
        }

        size_t copy(elem_pointer from, size_t from_size, index_t tail_index, size_t count_copy,
                    elem_pointer to,   size_t to_size)
        {
            index_t index = tail_index;
            size_t count_element_for_copy = std::min(std::min(from_size, to_size), count_copy);

            for (size_t i = 0; i < count_element_for_copy; ++i)
            {
                alloc_traits::construct(_alloc, to + i, from[index]);
                index = next_index(index, from_size);
            }

            return count_element_for_copy;
        }


    private:
        void assign(const dynamic_cyclic_queue& right)
        {
            if (this == std::addressof(right))
                return;

            if (_size == right._size)
            {
                // copy all elements from right
                auto count_copy = copy(right._elems, right._size, right._tail, right._count, _elems, _size);

                // if both queue were full, copied equal size, so head should be 0
                _head = count_copy == _size ? 0 : count_copy;
                _tail = 0;
                _count = count_copy;

                return;
            }

            // if elements exist
            if (_elems != nullptr)
            {
                // destroy their
                alloc_traits::destroy(_alloc, _elems);
                // and deallocate memory
                _alloc.deallocate(_elems, _size);
            }

            // set allocator from right before deallocate
            _alloc = right._alloc;

            // allocate new memory with size equal right
            _elems = _alloc.allocate(right._size);
            _size = right._size;

            // copy all elements from right
            auto count_copy = copy(right._elems, right._size, right._tail, right._count, _elems, _size);

            // if both queue were full, copied equal size, so head should be 0
            _head = count_copy == _size ? 0 : count_copy;
            _tail = 0;
            _count = count_copy;
        }

        void assign(dynamic_cyclic_queue&& right) noexcept
        {
            if (this == std::addressof(right))
                return;

            _elems = right._elems;
            _head = right._head;
            _tail = right._tail;
            _count = right._count;
            _size = right._size;

            right._elems = nullptr;
            right._head = 0;
            right._tail = 0;
            right._count = 0;
            right._size = 0;
        }

        _NODISCARD static index_t next_index(index_t index, size_t size) noexcept
        {
            ++index;
            return index >= size ? 0 : index;
        }

    };

}

#endif // _CYCLIC_QUEUE_DYNAMIC_ARRAY_