
#include <stdio.h>
#include <string>
#include "custom_container.h"
#include "custom_alloc.h"
#include <gtest/gtest.h>

std::string make_container_custom_alloc()
{
    std::string str_buf{};
    custom_int_container<custom_block_allocator<int, 5>, true> arr1;
    for (int idx = 0; idx < 5; idx++)
    {
        arr1.add_element(idx);
    }

    for (int idx = 0; idx < 5; idx++)
    {
        str_buf += arr1.get(idx);
        str_buf += " ";
    }
    
    return str_buf;
}

std::string make_container_std_alloc()
{
    std::string str_buf{};
    custom_int_container<std::allocator<int>> arr1;
    for (int idx = 0; idx < 5; idx++)
    {
        arr1.add_element(idx);
    }

    for (int idx = 0; idx < 5; idx++)
    {
        str_buf += arr1.get(idx);
        str_buf += " ";
    }
    
    return str_buf;
}


TEST(IP_LIST, custom_alloc)
{
    auto res = make_container_custom_alloc();
    std::string comp{};
    for (int idx = 0; idx < 5; idx++)
    {
        comp += idx;
        comp += " ";
    }
    

    EXPECT_EQ(res, comp);
}

TEST(IP_LIST, std_alloc)
{
    auto res = make_container_std_alloc();
    std::string comp{};
    for (int idx = 0; idx < 5; idx++)
    {
        comp += idx;
        comp += " ";
    }

    EXPECT_EQ(res, comp);
}
