#pragma once

#include <stdexcept>
#include <vcruntime.h>

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
            next = right.next;
        }

        void assign(list_item&& right) noexcept
        {
            value = std::move(right.value);
            next = std::move(right.next);
        }
    };


    template <typename ItemType, size_t Size>
    class cyclic_queue_list
    {
        using value_type        = ItemType;
        using list              = cyclic_queue_list_item<value_type>;
        using list_pointer      = list*;


    public:
        cyclic_queue_list()
        {
            _head = _items;
            _tail = _items;
            _count = 0;

            for (int i = 0; i < Size - 1; ++i)
                _items[i].next = &_items[i + 1];

            _items[Size - 1].next = &_items[0];
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

        void push(value_type&& item)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            _head->value = std::move(item);

            ++_count;
            _head = _head->next;
        }

        void push(const value_type& item)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            _head->value = item;

            ++_count;
            _head = _head->next;
        }

        _NODISCARD value_type pop()
        {
            if (empty())
                throw std::logic_error("Queue is empty!");

            value_type item = std::move(_tail->value);

            --_count;
            _tail = _tail->next;
            return item;
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


    private:
        list _items[Size];
        list_pointer _head;
        list_pointer _tail;

        size_t _count;


    private:
        void assign(const cyclic_queue_list& right)
        {
            if (this == std::addressof(right))
                return;

            for (int i = 0; i < Size; ++i)
            {
                _items[i] = right._items[i];
            }

            assign_setup(right);
        }

        void assign(cyclic_queue_list&& right) noexcept
        {
            if (this == std::addressof(right))
                return;

            for (int i = 0; i < Size; ++i)
            {
                _items[i] = std::move(right._items[i]);
            }

            assign_setup(right);

            right._head = 0;
            right._tail = 0;
            right._count = 0;
        }

        void assign_setup(const cyclic_queue_list& right) noexcept
        {
            // reset pointer for list
            for (int i = 0; i < Size - 1; ++i)
            {
                _items[i].next = &_items[i + 1];
            }
            _items[Size - 1].next = &_items[0];


            // some pointer magic
            // calculate shift for start of array
            size_t right_index_head = static_cast<int>(right._head - &right._items[0]) / sizeof(right._head);
            size_t right_index_tail = static_cast<int>(right._tail - &right._items[0]) / sizeof(right._tail);

            // and here set new pointers
            _head = _items + right_index_head;
            _tail = _items + right_index_tail;
            _count = right._count;
        }

    };

}
