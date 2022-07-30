#ifndef custom_alloc_h
#define custom_alloc_h


#include <iostream>
#include <map>
#include <vector>


static size_t MAX_ELEMENT_AMOUNT = 20;

class exception : public std::exception
{
public:
    exception(const std::string &msg):
        __msg(msg) {;}

    const char *what() const noexcept override
    {
        return __msg.c_str();
    }

protected:
    std::string __msg;
};

template <typename T, size_t COUNT>
class custom_block_allocator
{
public:

    using value_type = T;

    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;

    template <typename U>
    struct rebind
    {
        using other = custom_block_allocator<U, COUNT>;
    };

    custom_block_allocator() = default;
    ~custom_block_allocator() = default;

    template <typename U, size_t C>
    custom_block_allocator(const custom_block_allocator<U, C> &)
    {}

    T *allocate(std::size_t n)
    {
        if (COUNT > MAX_ELEMENT_AMOUNT  || n > MAX_ELEMENT_AMOUNT)
            throw exception("Max amount of elements in allocator was exceeded");
        T *p;
        
        if (__allocations_count)
        {
            p = __current_ptr + __allocations_count;
        }
        else
        {
           p = reinterpret_cast<T *>(std::malloc(n * COUNT * sizeof(T)));
            __current_ptr = p;
        }
        
        ++__allocations_count;

        if (!p)
            throw std::bad_alloc();
        return p;
    }

    void deallocate(T *p, [[maybe_unused]]std::size_t n)
    {
        if (!__set_clean)
        {
            std::free(p);
            __set_clean = true;
        }
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        new (p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *p)
    {
        p->~U();
    }
    
private:
    size_t __allocations_count{0};
    bool __set_clean = false;
    T *__current_ptr;
};

#endif /* custom_alloc_h */
