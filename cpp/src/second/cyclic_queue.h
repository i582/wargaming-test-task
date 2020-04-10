#pragma once

#ifndef _CYCLIC_QUEUE_
#define _CYCLIC_QUEUE_


namespace test_tasks
{

    template <typename ItemType, size_t Size>
    class cyclic_queue
    {
    public:
        using value_type        = ItemType;
        using index             = size_t;


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
            assign(std::forward<cyclic_queue>(right));
        }


    public:
        cyclic_queue& operator=(const cyclic_queue& right)
        {
            assign(right);
            return *this;
        }

        cyclic_queue& operator=(cyclic_queue&& right) noexcept
        {
            assign(std::forward<cyclic_queue>(right));
            return *this;
        }

        void push(value_type&& item)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            _items[_head] = std::move(item);

            ++_count;
            _head = next_index(_head);
        }

        void push(const value_type& item)
        {
            if (full())
                throw std::logic_error("Queue is full!");

            _items[_head] = item;

            ++_count;
            _head = next_index(_head);
        }

        _NODISCARD value_type pop()
        {
            if (empty())
                throw std::logic_error("Queue is empty!");

            value_type item = std::move(_items[_tail]);

            --_count;
            _tail = next_index(_tail);
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
        value_type _items[Size];
        index _head;
        index _tail;

        size_t _count;


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
                _items[i] = right._items[i];
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
                _items[i] = std::move_if_noexcept(right._items[i]);
            }
        }

        _NODISCARD _CONSTEXPR17 index next_index(index ind) const noexcept
        {
            ++ind;
            return ind >= Size ? 0 : ind;
        }

    };

}

#endif // _CYCLIC_QUEUE_