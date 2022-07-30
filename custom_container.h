#ifndef custom_container_h
#define custom_container_h

#include "iostream"
#include "custom_alloc.h"

#define ELEMENTS_IN_CONTAINER 10

template <typename Alloc = std::allocator<int>, bool IsBlockAllocator = false>
class custom_int_container
{
  
    int* __data;
    std::size_t __capacity{0};
    std::size_t __size{0};
    Alloc __allocator;
    bool __allocated{false};

public:

    friend class iterator;
    using Atraits = std::allocator_traits<Alloc>;
    
    custom_int_container([[maybe_unused]]Alloc alloc = Alloc()) : __data(nullptr)
    {
       
    }
    ~custom_int_container()
    {
        destroy(__size);
        deallocate(__capacity);
    }
    
    void allocate(std::size_t capacity)
    {
        __capacity = capacity;
        __data = Atraits::allocate(__allocator, capacity);
    }

    void deallocate(std::size_t capacity)
    {
        Atraits::deallocate(__allocator, __data, capacity);
        __capacity = 0;
        __size = 0;
    }
   
    void construct(std::size_t size, const int& value)
    {
        __size = size;
        Atraits::construct(__allocator, __data + size - 1, value);
    }

    constexpr void destroy(std::size_t size)
    {
        for (std::size_t idx = 0; idx < size; ++idx)
        {
            Atraits::destroy(__allocator, __data + idx);
        }
        __size = 0;
        
    }
    
    void add_element(const int& data)
    {
        if (IsBlockAllocator && !__allocated)
        {
            // Для блочного аллокатора выделяем сразу блок
            allocate(MAX_ELEMENT_AMOUNT);
            ++__size;
            __allocated = true;
        }
        else
        {
            if (__capacity < ++__size)
            {
                int * tmp = nullptr;
                if (__data && __capacity)
                {
                    // Если аллоцированной памяти не хватает
                    // скопируем имеющиеся данные в другую память, переаллоцируем, положим данные обратно
                    // Чистим все вовремя
                    tmp = Atraits::allocate(__allocator, __capacity);
                    for (std::size_t idx = 0; idx < __size - 1; ++idx)
                    {
                        Atraits::construct(__allocator, tmp + idx, *(__data + idx));
                    }

                    Atraits::deallocate(__allocator, __data, __capacity);
                    for (std::size_t idx = 0; idx < __size - 1; ++idx)
                    {
                        Atraits::destroy(__allocator, __data + idx);
                    }
                }
                // Выделяем памяти с запасом, чтобы сократить операции аллокации/деаллокации
                allocate(__size * 2);

                if (tmp)
                {
                    for (std::size_t idx = 0; idx < __size - 1; ++idx)
                    {
                        Atraits::construct(__allocator, __data + idx, *(tmp + idx));
                    }

                    Atraits::deallocate(__allocator, tmp, __capacity);
                    for (std::size_t idx = 0; idx < __size - 1; ++idx)
                    {
                        Atraits::destroy(__allocator, tmp + idx);
                    }
                }
            }
        }
        construct(__size, data);
    }
    
    int get(size_t idx) const
    {
        return *(__data + idx);
    }
    
    int *get_data() const
    {
        return __data;
    }
    
    size_t size() const
    {
        return __size;
    }
    
    void print()
    {
        for (std::size_t idx = 0; idx < __size; ++idx)
            std::cout << *(__data + idx) << " ";
        std::cout << std::endl;
    }
    
};

#endif /* custom_container_h */

 
