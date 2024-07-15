/**
  ******************************************************************************
  * @file           : Allocator.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/2/20
  ******************************************************************************
  */
#ifndef MYCONTAINER_ALLOCATOR_H
#define MYCONTAINER_ALLOCATOR_H

#include <cstddef>
#include <new>
#include <utility>

namespace XQ::Memory {
    template<typename _Alloc>
    concept Alloc = requires(_Alloc x, int *p, std::size_t n) {
        x.allocate(n);
        x.deallocate(p, n);
        x.construct(p, *p);
        x.destroy(p, n);
    };

    template<typename T>
    class Allocator {
    public:
        using value_type = T;
        using pointer = T *;
        using size_type = std::size_t;

        constexpr static pointer allocate(size_type n) {
            return reinterpret_cast<pointer>(::operator new(sizeof(value_type) * n));
        }

        constexpr static void deallocate(pointer p, size_type n) {
            ::operator delete(p, n);
        }

        template<typename ...Args>
        constexpr static void construct(pointer p, Args &&... args) {
            new(p) value_type(std::forward<Args>(args)...);
        }

        void destroy(pointer p, size_type n = 1) {
            for (size_type i = 0; i < n; ++i) {
                (i + p)->~T();
            }
        }
    };

} // XQ::Memory

#endif //MYCONTAINER_ALLOCATOR_H
