#pragma once
#ifndef _CYCLIC_QUEUE_LIST_
#define _CYCLIC_QUEUE_LIST_

#include <yvals_core.h>
#include <xmemory>

namespace test_tasks
{

    template <typename ItemType>
    class cyclic_queue_list_item
    {
        using list_item     = cyclic_queue_list_item;
        using value_type    = ItemType;


    public:
        value_type value;
        list_item* next;


    public:
        cyclic_queue_list_item() = default;

        cyclic_queue_list_item(const list_item& right)
        {
            assign(right);
        }

        cyclic_queue_list_item(list_item&& right) noexcept
        {
            assign(std::forward<list_item>(right));
        }


    public:
        list_item& operator=(const list_item& right)
        {
            assign(right);
            return *this;
        }

        list_item& operator=(list_item&& right) noexcept
        {
            assign(std::forward<list_item>(right));
            return *this;
        }


    private:
        void assign(const list_item& right)
        {
            value = right.value;
            next = nullptr;
        }

        void assign(list_item&& right) noexcept
        {
            // use move_if_noexcept, because the assign function is marked as noexcept.
            value = std::move_if_noexcept(right.value);
            next = nullptr;
            right.next = nullptr;
        }
    };


    template <typename ElementType, size_t Size>
    class cyclic_queue_list
    {
        using value_type            = ElementType;
        using list                  = cyclic_queue_list_item<value_type>;
        using list_pointer          = list*;
        using const_list_pointer    = const list* const;

        using index_t               = size_t;

        using alloc_traits          = std::allocator_traits<std::allocator<ElementType>>;

    public:
        cyclic_queue_list()
        {
            _head = _elems;
            _tail = _elems;
            _count = 0;

            for (int i = 0; i < Size - 1; ++i)
                _elems[i].next = &_elems[i + 1];

            _elems[Size - 1].next = &_elems[0];
        }

        cyclic_queue_list(const cyclic_queue_list& right)
        {
            assign(right);
        }

        cyclic_queue_list(cyclic_queue_list&& right) noexcept
        {
            assign(std::forward<cyclic_queue_list>(right));
        }


    public:
        cyclic_queue_list& operator=(const cyclic_queue_list& right)
        {
            assign(right);
            return *this;
        }

        cyclic_queue_list& operator=(cyclic_queue_list&& right) noexcept
        {
            assign(std::forward<cyclic_queue_list>(right));
            return *this;
        }

        void push(const value_type& elem)
        {
            emplace(elem);
        }

        void push(value_type&& elem) noexcept
        {
            emplace(std::move_if_noexcept(elem));
        }

        template <typename Valty>
        void emplace(Valty&& val)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            _head->value = std::forward<Valty>(val);

            ++_count;
            _head = _head->next;
        }

        _NODISCARD value_type pop()
        {
            if (empty())
                throw std::logic_error("Queue is empty!");

            value_type elem = std::move(_tail->value);

            --_count;
            _tail = _tail->next;
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
        list _elems[Size];
        list_pointer _head;
        list_pointer _tail;

        size_t _count;

        std::allocator<value_type> _alloc;

    private:
        void assign(const cyclic_queue_list& right)
        {
            if (this == std::addressof(right))
                return;

            for (int i = 0; i < Size; ++i)
            {
                _elems[i] = right._elems[i];
            }

            assign_setup(right);
        }

        void assign(cyclic_queue_list&& right) noexcept
        {
            if (this == std::addressof(right))
                return;

            for (int i = 0; i < Size; ++i)
            {
                // use move_if_noexcept, because the assign function is marked as noexcept.
                _elems[i] = std::move_if_noexcept(right._elems[i]);
            }

            assign_setup(right);

            right._head = nullptr;
            right._tail = nullptr;
            right._count = 0;
        }

        void assign_setup(const cyclic_queue_list& right) noexcept
        {
            // reset pointer for list
            for (int i = 0; i < Size - 1; ++i)
            {
                _elems[i].next = &_elems[i + 1];
            }
            _elems[Size - 1].next = &_elems[0];


            // some pointer magic
            // calculate shift for start of array
            index_t right_index_head = get_index_for_pointer(right._elems, right._head);
            index_t right_index_tail = get_index_for_pointer(right._elems, right._tail);


            // and here set new pointers
            _head = _elems + right_index_head;
            _tail = _elems + right_index_tail;
            _count = right._count;
        }


        _NODISCARD static index_t get_index_for_pointer(const_list_pointer rel_list, const_list_pointer ptr) noexcept
        {
            return static_cast<index_t>(static_cast<int>(ptr - rel_list) / sizeof(ptr));
        }

    };

}

#endif // _CYCLIC_QUEUE_LIST_