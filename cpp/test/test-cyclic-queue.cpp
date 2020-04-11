#include <gtest/gtest.h>
#include "simple_class.h"
#include "../src/second/cyclic_queue.h"

TEST(test_cyclic_queue, test_cyclic_queue_with_pod)
{
    test_tasks::impl_on_array::statically::cyclic_queue<int, 5> queue1;

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

TEST(test_cyclic_queue, test_cyclic_queue_with_class_object)
{
    test_tasks::impl_on_array::statically::cyclic_queue<SimpleClass, 5> queue1;

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

    test_tasks::impl_on_array::statically::cyclic_queue<SimpleClass, 5> queue2(queue1);

    EXPECT_EQ(queue1.pop().name(), queue2.pop().name());
    EXPECT_EQ(queue1.pop().name(), queue2.pop().name());


    // move constructor
    test_tasks::impl_on_array::statically::cyclic_queue<SimpleClass, 5> queue3(std::move(queue1));

    // queue1 was moved, so pop throw exception
    EXPECT_ANY_THROW(queue1.pop());

    EXPECT_EQ(queue1.count(), 0);
}

