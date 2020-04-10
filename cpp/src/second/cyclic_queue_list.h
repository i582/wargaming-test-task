#pragma once
#ifndef _CYCLIC_QUEUE_LIST_
#define _CYCLIC_QUEUE_LIST_


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


    template <typename ItemType, size_t Size>
    class cyclic_queue_list
    {
        using value_type            = ItemType;
        using list                  = cyclic_queue_list_item<value_type>;
        using list_pointer          = list*;
        using const_list_pointer    = const list* const;

        using index                 = size_t;


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
                // use move_if_noexcept, because the assign function is marked as noexcept.
                _items[i] = std::move_if_noexcept(right._items[i]);
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
                _items[i].next = &_items[i + 1];
            }
            _items[Size - 1].next = &_items[0];


            // some pointer magic
            // calculate shift for start of array
            index right_index_head = get_index_for_pointer(right._items, right._head);
            index right_index_tail = get_index_for_pointer(right._items, right._tail);


            // and here set new pointers
            _head = _items + right_index_head;
            _tail = _items + right_index_tail;
            _count = right._count;
        }


        _NODISCARD static index get_index_for_pointer(const_list_pointer rel_list, const_list_pointer ptr) noexcept
        {
            return static_cast<index>(static_cast<int>(ptr - rel_list) / sizeof(ptr));
        }

    };

}

#endif // _CYCLIC_QUEUE_LIST_