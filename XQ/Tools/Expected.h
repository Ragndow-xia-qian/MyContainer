/**
  ******************************************************************************
  * @file           : Expected.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/3
  ******************************************************************************
  */

#ifndef MYCONTAINER_EXPECTED_H
#define MYCONTAINER_EXPECTED_H

#include <type_traits>
#include <expected>

namespace XQ::Tools {
    template<typename T, typename E>
    class Expected;
    
    template<typename E>
    class UnExpected {
    public:
        using error_type = E;
        
        UnExpected() = default;
        
        UnExpected(const UnExpected &) = default;
        
        UnExpected(UnExpected &&oth) noexcept: _error(std::move(oth._error)) {}
        
        explicit UnExpected(const E &e): _error(e) {}

    private:
        template<typename T, typename R>
        friend class Expected;
        
        error_type _error;
    };

    template<typename T, typename E>
    class Expected {
    public:
        using value_type = T;
        using error_type = E;
        using reference = T &;
        using pointer = T *;
        using const_pointer = const T *;
        using error_reference = E &;
        using const_reference = const T &;
        using const_error_reference = const E &;

        Expected() = default;

        Expected(const Expected &) = default;

        Expected(Expected &&oth) noexcept: _data(std::move(_data)), _has_value(oth._has_value) {}

        Expected(const T &val) : _data(val) {}

        Expected(const UnExpected<E> &error): _data(error), _has_value(false) {}

        [[nodiscard]] bool has_value() const {
            return _has_value;
        }

        [[nodiscard]] const_reference value() const {
            if (_has_value) {
                return _data._value;
            }
            throw std::bad_expected_access("This doesn't have value");
        }

        [[nodiscard]] const_error_reference error() const {
            return _data._error._data._error;
        }

        template<typename U>
        [[nodiscard]] const_reference value_or(U &&default_value) const {
            if (_has_value) {
                return _data._value;
            }
            return std::forward<U>(default_value);
        }

        template<typename F>
        Expected and_then(F &&func) const {
            if (_has_value) {
                return Expected<T, E>(func(_data._value));
            }
            return *this;
        }

        template<typename F>
        Expected &transform(F &&func) {
            if (_has_value) {
                _data._value = func(_data._value);
            }
            return *this;
        }

        template<typename F>
        Expected or_else(F &&func) const {
            if (!_has_value) {
                return Expected<T, E>(func(_data._error));
            }
            return *this;
        }

        template<typename F>
        Expected &transform_error(F &&func) {
            if (!_has_value) {
                _data._error = func(_data._error);
            }
            return *this;
        }

        const_pointer operator->() const {
            return &this->value();
        }

    private:
        union {
            value_type _value;
            UnExpected<error_type> _error;
        } _data;
        
        bool _has_value{true};
    };

    void test() {
        struct A {
            int a, b;

            void test() const {}
        };
        Expected<A, char> e(A{1, 1});
        e->test();
    }
} // XQ::Tools

#endif //MYCONTAINER_EXPECTED_H
