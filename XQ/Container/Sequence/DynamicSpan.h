/**
  ******************************************************************************
  * @file           : DynamicSpan.h
  * @author         : 37237
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/17
  ******************************************************************************
  */
//
// Created by 37237 on 2024/6/17.
//

#ifndef MYCONTAINER_DYNAMICSPAN_H
#define MYCONTAINER_DYNAMICSPAN_H

#include "../../Memory/Allocator.h"
#include "ArrayList.h"

namespace XQ::Container::Sequence {

    template<typename T, typename Array = XQ::Container::Sequence::ArrayList<T, XQ::Memory::Allocator<T>>>
    class DynamicSpan {
    public:
        using value_type = T;
        using reference = T &;
        using array_type = Array;
        using size_type = std::size_t;

        DynamicSpan() = default;

        template<typename ...Args>
        requires ((std::is_convertible_v<Args, size_type> && ...))
        explicit DynamicSpan(Args ...args) {
            auto siz = (args * ...);
            _dimes = sizeof...(Args);
            _array.resize(siz);
            _siz = array_type{args..., 1};
            for (int i = _siz.size() - 2; i > 0; --i) {
                _siz[i] *= _siz[i + 1];
            }
            for (int i = 0; i < _siz.size() - 1; ++i) {
                _siz[i] = _siz[i + 1];
            }
            _siz.pop();
        }

        template<typename ...Args>
        requires ((std::is_convertible_v<Args, size_type> && ...))
        reference operator[](Args ...args) {
            auto pos = [&siz = this->_siz, &args...]<std::size_t... Is>(std::index_sequence<Is...>) {
                return ((siz[Is] * args) + ...);
            }(std::index_sequence_for<Args...>{});
            return _array[pos];
        }

    private:
        array_type _array;
        array_type _siz;
        size_type _dimes;
    };

} // XQ::Container::Sequence

#endif //MYCONTAINER_DYNAMICSPAN_H
