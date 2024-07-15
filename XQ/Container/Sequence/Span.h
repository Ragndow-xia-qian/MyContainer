/**
  ******************************************************************************
  * @file           : Span.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/1
  ******************************************************************************
  */

#ifndef MYCONTAINER_SPAN_H
#define MYCONTAINER_SPAN_H

#include <algorithm>
#include <type_traits>
#include <tuple>
#include <iterator>

namespace XQ::Container::Sequence {
    template<typename T, std::size_t d, std::size_t ...dimes>
    class Span {
    public:
        using value_type = T;
        using pointer = T *;
        using iterator = Span<T, dimes...> *;
        using reference = T &;
        using const_reference = const T &;
        static constexpr auto dime = 1 + sizeof...(dimes);

        template<typename F, typename ...Args, typename = std::enable_if_t<std::is_integral_v<F>>>
        reference operator[](F f, Args ...args) {
            return _data[f]
            [args...];
        }

        template<typename F, typename = std::enable_if_t<std::is_integral_v<F>>>
        Span<T, dimes...> &operator[](F f) {
            return _data[f];
        }

        template<typename F, typename ...Args, typename = std::enable_if_t<std::is_integral_v<F>>>
        const_reference operator[](F f, Args ...args) const {
            return _data[f]
            [args...];
        }

        template<typename F, typename = std::enable_if_t<std::is_integral_v<F>>>
        const Span<T, dimes...> &operator[](F f) const {
            return _data[f];
        }

        [[nodiscard]] iterator begin() {
            return std::begin(_data);
        }

        [[nodiscard]] iterator end() {
            return std::end(_data);
        }

        [[nodiscard]] auto begin() const {
            return std::begin(_data);
        }

        [[nodiscard]] auto end() const {
            return std::end(_data);
        }

    private:
        Span<T, dimes...> _data[d];
    };

    template<typename T, std::size_t d>
    class Span<T, d> {
    public:
        using value_type = T;
        using pointer = T *;
        using iterator = pointer;
        using const_iterator = const T *;
        using reference = T &;
        static constexpr auto dime = 1;

        Span() = default;

        Span(std::initializer_list<T> list) {
            for (std::size_t i = 0; i < std::min(list.size(), d); ++i) {
                _data[i] = *(list.begin() + i);
            }
        }

        template<typename F, typename = std::enable_if_t<std::is_integral_v<F>>>
        reference operator[](F f) {
            return _data[f];
        }

        [[nodiscard]] iterator begin() {
            return std::begin(_data);
        }

        [[nodiscard]] iterator end() {
            return std::end(_data);
        }

        [[nodiscard]] const_iterator begin() const {
            return std::begin(_data);
        }

        [[nodiscard]] const_iterator end() const {
            return std::end(_data);
        }

    private:
        value_type _data[d];
    };
} // XQ::Container::Sequence

#endif //MYCONTAINER_SPAN_H
