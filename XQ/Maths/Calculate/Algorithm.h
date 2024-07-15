/**
  ******************************************************************************
  * @file           : Algorithm.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/24
  ******************************************************************************
  */
#ifndef MYCONTAINER_ALGORITHM_H
#define MYCONTAINER_ALGORITHM_H

#include <cstdint>
#include <type_traits>

namespace XQ::Maths::Algorithm {
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    [[nodiscard]] decltype(auto) pow(T base, T index, T mod = static_cast<T>(-1)) {
        T res = 1;
        while (index != 0) {
            if (index & 1) {
                res = res * base;
                if (mod != static_cast<T>(-1)) {
                    res = res % mod;
                }
            }
            base = base * base;
            if (mod != static_cast<T>(-1)) {
                base = base % mod;
            }
            index = index >> 1;
        }
    }

    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    [[nodiscard]] decltype(auto) gcd(T x, T y) {
        if (x < y) {
            x ^= y ^= x ^= y;
        }
        if (y == 0) {
            return x;
        }
        if ((x & 1) && (y & 1)) {
            return gcd(y, x - y);
        }
        if (x & 1) {
            return gcd(x, y >> 1);
        }
        if (y & 1) {
            return gcd(x >> 1, y);
        }
        return gcd(x >> 1, y >> 1) << 1;
    }

    namespace ModInt {
        template<typename Int = int64_t, typename = std::enable_if_t<std::is_integral_v<Int>>>
        class IntMod998244353 {
        public:
            constexpr static Int mod = 998244353;

            IntMod998244353() = default;

            IntMod998244353(Int val) : _data((val % mod + mod) % mod) {}

            IntMod998244353(const IntMod998244353 &) = default;

            IntMod998244353(IntMod998244353 &&) noexcept = default;

            IntMod998244353 &operator=(const IntMod998244353 &) = default;

            IntMod998244353 &operator=(IntMod998244353 &&) noexcept = default;

            operator Int() {
                return _data;
            }

            IntMod998244353 operator+(IntMod998244353 rhs) {
                return {_data + rhs._data};
            }

            IntMod998244353 operator-(IntMod998244353 rhs) {
                return {_data - rhs._data};
            }

            IntMod998244353 operator*(IntMod998244353 rhs) {
                return {_data * rhs._data};
            }

            IntMod998244353 operator/(IntMod998244353 rhs) {
                return {_data * pow(rhs._data, mod - 2, mod)};
            }

        private:
            Int _data{};
        };

        template<typename Int = int64_t, typename = std::enable_if_t<std::is_integral_v<Int>>>
        class IntMod1000000007 {
        public:
            constexpr static Int mod = 1e9 + 7;

            IntMod1000000007() = default;

            IntMod1000000007(Int val) : _data((val % mod + mod) % mod) {}

            IntMod1000000007(const IntMod1000000007 &) = default;

            IntMod1000000007(IntMod1000000007 &&) noexcept = default;

            IntMod1000000007 &operator=(const IntMod1000000007 &) = default;

            IntMod1000000007 &operator=(IntMod1000000007 &&) noexcept = default;

            operator Int() {
                return _data;
            }

            IntMod1000000007 operator+(IntMod1000000007 rhs) {
                return {_data + rhs._data};
            }

            IntMod1000000007 operator-(IntMod1000000007 rhs) {
                return {_data - rhs._data};
            }

            IntMod1000000007 operator*(IntMod1000000007 rhs) {
                return {_data * rhs._data};
            }

            IntMod1000000007 operator/(IntMod1000000007 rhs) {
                return {_data * pow(rhs._data, mod - 2, mod)};
            }

        private:
            Int _data{};
        };
    } // ModInt
} // XQ::Maths::Algorithm

#endif //MYCONTAINER_ALGORITHM_H
