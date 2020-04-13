#include "iostream"
#include <gtest/gtest.h>
#include "simple_class.h"
#include "../src/second/cyclic_queue_dynamic_array.h"

TEST(test_dynamic_dynamic_cyclic_queue, test_dynamic_dynamic_cyclic_queue_with_pod)
{
    test_tasks::impl_on_array::dynamically::cyclic_queue<int> queue1(5);

    // queue is empty, so throw exception
    EXPECT_ANY_THROW(queue1.pop());

    queue1.push(100);
    auto res = queue1.pop();

    EXPECT_EQ(res, 100);

    queue1.push(1);
    queue1.push(2);
    queue1.push(3);
    queue1.push(4);
    queue1.push(5);

    // queue is full, so throw exception
    EXPECT_ANY_THROW(queue1.push(100));

    res = queue1.pop();
    EXPECT_EQ(res, 1);
    res = queue1.pop();
    EXPECT_EQ(res, 2);
    res = queue1.pop();
    EXPECT_EQ(res, 3);
    res = queue1.pop();
    EXPECT_EQ(res, 4);
    res = queue1.pop();
    EXPECT_EQ(res, 5);

    EXPECT_TRUE(queue1.empty());


    queue1.push(1);
    queue1.push(2);
    queue1.push(3);
    queue1.pop();
    queue1.pop();
    queue1.pop();

    queue1.push(4);

    queue1.pop();

    queue1.push(5);

    queue1.push(1);
    queue1.pop();

    queue1.push(2);
    queue1.pop();

    queue1.push(3);

    res = queue1.pop();
    EXPECT_EQ(res, 2);
    res = queue1.pop();
    EXPECT_EQ(res, 3);


    queue1.push(1);
    queue1.push(2);
    queue1.push(3);

    EXPECT_EQ(queue1.count(), 3);
}

TEST(test_dynamic_dynamic_cyclic_queue, test_dynamic_dynamic_cyclic_queue_with_class_object)
{
    test_tasks::impl_on_array::dynamically::cyclic_queue<SimpleClass> queue1(5);

    SimpleClass el("Ivan");
    SimpleClass el1("Alexey");
    SimpleClass el2("Olga");

    queue1.push(el);
    queue1.push(el1);
    queue1.push(el);
    queue1.push(el2);

    EXPECT_EQ(queue1.count(), 4);

    auto res = queue1.pop();
    EXPECT_EQ(res.name(), "Ivan");
    res = queue1.pop();
    EXPECT_EQ(res.name(), "Alexey");
    res = queue1.pop();
    EXPECT_EQ(res.name(), "Ivan");
    res = queue1.pop();
    EXPECT_EQ(res.name(), "Olga");

    queue1.push(SimpleClass("Evgeniy"));
    res = queue1.pop();
    EXPECT_EQ(res.name(), "Evgeniy");


    EXPECT_TRUE(queue1.empty());


    // copy constructor

    queue1.push(el);
    queue1.push(el1);
    queue1.push(el);
    queue1.push(el2);

    test_tasks::impl_on_array::dynamically::cyclic_queue<SimpleClass> queue2(queue1);

    EXPECT_EQ(queue1.pop().name(), queue2.pop().name());
    EXPECT_EQ(queue1.pop().name(), queue2.pop().name());


    // move constructor
    test_tasks::impl_on_array::dynamically::cyclic_queue<SimpleClass> queue3(std::move(queue1));

    // queue1 was moved, so pop throw exception
    EXPECT_ANY_THROW(queue1.pop());

    EXPECT_EQ(queue1.count(), 0);


    test_tasks::impl_on_array::dynamically::cyclic_queue<SimpleClass> queue4(0);

    queue4.resize(10);

    const string people = "People";
    string people2 = "People2";

    queue4.emplace("Ivan");
    queue4.emplace(people);
    queue4.emplace(std::move(people2));

    EXPECT_EQ(queue4.pop().name(), "Ivan");
    EXPECT_EQ(queue4.pop().name(), "People");
    EXPECT_EQ(queue4.pop().name(), "People2");


    EXPECT_TRUE(queue4.get_allocator() == std::allocator<SimpleClass>());


    queue4.emplace("Ivan");
    queue4.emplace(people);

    queue4.clear();

    queue4.emplace("Ivan");
    queue4.emplace(people);

    EXPECT_EQ(queue4.pop().name(), "Ivan");
    EXPECT_EQ(queue4.pop().name(), "People");
}


#include "my_allocator.h"

TEST(test_dynamic_dynamic_cyclic_queue, test_dynamic_dynamic_cyclic_queue_with_other_allocator)
{
    using MAi = MyAllocator<int>;


    test_tasks::impl_on_array::dynamically::
    cyclic_queue<int, MyAllocator<int>> queue1(5, MyAllocator<int>());

    queue1.push(100);
    queue1.push(100);
    queue1.push(100);
    queue1.push(100);
    queue1.push(100);



    EXPECT_EQ(MAi::count_allocate, 1);
    EXPECT_EQ(MAi::count_construct, 5);


    queue1.resize(10);


    EXPECT_EQ(MAi::count_allocate, 1 + 1);
    EXPECT_EQ(MAi::count_construct, 5 + 5);
    EXPECT_EQ(MAi::count_destroy, 0 + 5);
    EXPECT_EQ(MAi::count_deallocate, 0 + 1);
}