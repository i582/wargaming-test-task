#pragma once

#include <stdexcept>
#include <vcruntime.h>

namespace test_tasks
{
    template <typename ItemType>
    struct cyclic_queue_list_item
    {
        ItemType value;
        cyclic_queue_list_item* next;
    };


    template <typename ItemType, size_t Count>
    class cyclic_queue_list
    {
    public:
        using value_type        = ItemType;
        using list              = cyclic_queue_list_item<value_type>;
        using list_pointer      = list*;


    public:
        cyclic_queue_list()
        {
            _head = _items;
            _tail = _items;
            _count = 0;
            _size = Count;

            for (int i = 0; i < Count - 1; ++i)
                _items[i].next = &_items[i + 1];

            _items[Count - 1].next = &_items[0];
        }

        cyclic_queue_list(const cyclic_queue_list& right)
        {
            assign(right);
        }

        cyclic_queue_list(cyclic_queue_list&& right) noexcept
        {
            assign(right);
        }


    public:
        cyclic_queue_list& operator=(const cyclic_queue_list& right)
        {
            assign(right);
            return *this;
        }

        cyclic_queue_list& operator=(cyclic_queue_list&& right) noexcept
        {
            assign(right);
            return *this;
        }

        void push(ItemType&& item)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            _head->value = std::move(item);

            ++_count;
            _head = _head->next;
        }

        void push(ItemType& item)
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

        _NODISCARD size_t size() const noexcept
        {
            return _size;
        }

        _NODISCARD bool full() const noexcept
        {
            return _count == Count;
        }

        _NODISCARD bool empty() const noexcept
        {
            return _count == 0;
        }


    private:
        list _items[Count];
        list_pointer _head;
        list_pointer _tail;

        size_t _count;
        size_t _size;


    private:
        void assign(const cyclic_queue_list& right)
        {
            if (this == std::addressof(right))
                return;

            if (_size != right._size)
                throw std::logic_error("The size of the queue for copying is not equal to the size of the created queue!")

            _head = right._head;
            _tail = right._tail;
            _count = right._count;

            for (int i = 0; i < Count; ++i)
            {
                _items[i] = right._items[i];
            }
        }

        void assign(cyclic_queue_list&& right) noexcept
        {
            if (this == std::addressof(right))
                return;

            if (_size != right._size)
                throw std::logic_error("The size of the queue for copying is not equal to the size of the created queue!")

            _head = right._head;
            _tail = right._tail;
            _count = right._count;

            for (int i = 0; i < Count; ++i)
            {
                _items[i] = std::move(right._items[i]);
            }
        }

    };

}
