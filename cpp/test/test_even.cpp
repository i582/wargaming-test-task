#include <gtest/gtest.h>
#include "../src/first/is_even.h"

TEST(test_is_even, test_is_even_simple)
{
    for (int i = 0; i < 100; ++i)
    {
        if (i % 2 == 0)
        {
            EXPECT_TRUE(test_tasks::is_even(i));
        }
        else
        {
            EXPECT_FALSE(test_tasks::is_even(i));
        }
    }
}