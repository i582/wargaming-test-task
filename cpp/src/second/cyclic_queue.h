#pragma once

#ifndef _CYCLIC_QUEUE_
#define _CYCLIC_QUEUE_

#include <yvals_core.h>
#include <xmemory>

namespace test_tasks
{

    template <typename ElementType, size_t Size>
    class cyclic_queue
    {
    public:
        using value_type        = ElementType;
        using index_t           = size_t;

        using alloc_traits      = std::allocator_traits<std::allocator<ElementType>>;


    public:
        cyclic_queue() noexcept
        {
            _head = 0;
            _tail = 0;
            _count = 0;
        }

        cyclic_queue(const cyclic_queue& right)
        {
            assign(right);
        }

        cyclic_queue(cyclic_queue&& right) noexcept
        {
            assign(std::move(right));
        }


    public:
        cyclic_queue& operator=(const cyclic_queue& right)
        {
            assign(right);
            return *this;
        }

        cyclic_queue& operator=(cyclic_queue&& right) noexcept
        {
            assign(std::move(right));
            return *this;
        }

        void push(const value_type& elem)
        {
            emplace(elem);
        }

        void push(value_type&& elem)
        {
            emplace(std::move(elem));
        }

        template <typename... Valty>
        void emplace(Valty&&... val)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            alloc_traits::construct(_alloc, _elems + _head, std::forward<Valty>(val)...);

            ++_count;
            _head = next_index(_head);
        }

        _NODISCARD value_type pop()
        {
            if (empty())
                throw std::logic_error("Queue is empty!");

            value_type elem = std::move(_elems[_tail]);

            --_count;
            _tail = next_index(_tail);
            return elem;
        }

        _NODISCARD size_t count() const noexcept
        {
            return _count;
        }

        _NODISCARD _CONSTEXPR17 size_t size() const noexcept
        {
            return Size;
        }

        _NODISCARD bool full() const noexcept
        {
            return _count == Size;
        }

        _NODISCARD bool empty() const noexcept
        {
            return _count == 0;
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


    private:
        value_type _elems[Size];
        index_t _head;
        index_t _tail;

        size_t _count;

        std::allocator<value_type> _alloc;


    private:
        void assign(const cyclic_queue& right)
        {
            if (this == std::addressof(right))
                return;

            _head = right._head;
            _tail = right._tail;
            _count = right._count;

            for (int i = 0; i < Size; ++i)
            {
                _elems[i] = right._elems[i];
            }
        }

        void assign(cyclic_queue&& right) noexcept
        {
            if (this == std::addressof(right))
                return;

            _head = right._head;
            _tail = right._tail;
            _count = right._count;

            right._head = 0;
            right._tail = 0;
            right._count = 0;

            for (int i = 0; i < Size; ++i)
            {
                // use move_if_noexcept, because the assign function is marked as noexcept.
                _elems[i] = std::move_if_noexcept(right._elems[i]);
            }
        }

        _NODISCARD _CONSTEXPR17 index_t next_index(index_t ind) const noexcept
        {
            ++ind;
            return ind >= Size ? 0 : ind;
        }

    };

}

#endif // _CYCLIC_QUEUE_