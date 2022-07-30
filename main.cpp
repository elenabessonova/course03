

#include <iostream>
#include "custom_alloc.h"
#include "custom_container.h"

#define MAP_ELEMENTS 10

template<typename T>
void print_map(T &cont)
{
    for (auto &el : cont)
    {
        std::cout << el.first << " " << el.second << std::endl;
        
    }
    std::cout << std::endl;
}


int main()
{
    try
    {
//        auto v = std::vector<int, custom_block_allocator<int, 10>>{};
//         v.reserve(10);
//        for (int i = 0; i < 10; ++i)
//        {
//            v.emplace_back(i);
//            std::cout << "vector size = " << v.size() << std::endl;
//        }

        std::map<int, int> m1;
    
        
        int tmp_p = 1;
        for (int idx = 0; idx < MAP_ELEMENTS; ++idx)
        {
            tmp_p *= idx + 1;
            m1[idx] = tmp_p;
        }
        
        std::cout << "Map with std::allocator" << std::endl;
        print_map(m1);
        
        
        auto m2 = std::map<
            int,
            int,
            std::less<int>,
            custom_block_allocator<
                std::pair<
                    const int, int>, MAP_ELEMENTS>>{};
    
        tmp_p = 1;
        for (int idx = 0; idx < MAP_ELEMENTS; ++idx)
        {
            tmp_p *= idx + 1;
            m2[idx] = tmp_p;
        }
        
        std::cout << "Map with custom allocator" << std::endl;
        print_map(m2);
        
        
        std::cout << "custom_int_container with std::allocator" << std::endl;
        custom_int_container<std::allocator<int>> arr;

        for (int idx = 0; idx < 10; idx++)
        {
            arr.add_element(idx);
        }

        for (size_t idx = 0; idx < arr.size(); idx++)
        {
            std::cout << arr.get(idx) << " ";
        }
        std::cout << std::endl;
        
        
        std::cout << "Custom container with custom allocator" << std::endl;
        custom_int_container<custom_block_allocator<int, MAP_ELEMENTS>, true> arr1;
        for (int idx = 0; idx < 10; idx++)
        {
            arr1.add_element(idx);
        }

        for (size_t idx = 0; idx < arr1.size(); idx++)
        {
            std::cout << arr1.get(idx) << " ";
        }
        std::cout << std::endl;
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    
    return 0;
}
