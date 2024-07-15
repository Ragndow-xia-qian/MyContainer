/**
  ******************************************************************************
  * @file           : Variant.h
  * @author         : 37237
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/21
  ******************************************************************************
  */

#ifndef MYCONTAINER_VARIANT_H
#define MYCONTAINER_VARIANT_H

#include <iostream>
#include <vector>

#include <type_traits>
#include <utility>

namespace XQ::Tools {
    template<std::size_t id, typename U, typename T, typename ...Ts>
    struct Position {
        constexpr static auto pos = std::is_same_v<U, T> ? id : Position<id + 1, U, Ts...>::pos;
    };

    template<std::size_t id, typename U, typename T>
    struct Position<id, U, T> {
        constexpr static auto pos = std::is_same_v<U, T> ? id : -1;
    };

    template<typename find_type, typename ...Ts>
    constexpr static auto find_idx_by_type = Position<0, find_type, Ts...>::pos;

    template<typename ...>
    constexpr static bool is_all_trivial_v = false;

    template<typename Tp, typename... Ts>
    constexpr static bool is_all_trivial_v<Tp, Ts...> = std::is_trivial_v<Tp> && is_all_trivial_v<Ts...>;

    template<typename Tp>
    constexpr static bool is_all_trivial_v<Tp> = std::is_trivial_v<Tp>;

    template<typename T, typename ...Tps>
    class VariantStorage {
    public:
        template<std::size_t id>
        auto &get() {
            if constexpr (id == 0) {
                return _data._value;
            } else {
                return _data._next.template get<id - 1>();
            }
        }

        template<typename Tp>
        auto &get() {
            if constexpr (std::is_same_v<Tp, T>) {
                return _data._value;
            } else {
                return _data._next.template get<Tp>();
            }
        }

        template<typename Tp>
        void assign(Tp &&rhs) {
            using type = std::remove_cvref_t<Tp>;
            if constexpr (std::is_same_v<type, Tp>) {
                *new(&_data._value) type = std::forward<Tp>(rhs);
            } else {
                _data._next.assign(std::forward<Tp>(rhs));
            }
        }

        union A {
            T _value;
            VariantStorage<Tps...> _next;

            A() {}
            ~A() {}
        };
        A _data;
    };

    template<typename T>
    class VariantStorage<T> {
    public:
        template<std::size_t id>
        auto &get() {
            static_assert(id == 0);
            return _data._value;
        }

        template<typename Tp>
        auto &get() {
            static_assert(std::is_same_v<T, Tp>);
            return _data._value;
        }

        template<typename Tp>
        void assign(Tp &&rhs) {
            using type = std::remove_cvref_t<Tp>;
            *new(&_data._value) type = std::forward<Tp>(rhs);
        }

        union A {
            T _value;

            A() {}
            ~A() {}
        };
        A _data;
    };

    template<size_t id, typename head, typename... Ts>
    void destroy_variant_value(VariantStorage<head, Ts...> &v) {
        if constexpr (id == 0) {
            v._data._value.~head();
        } else {
            destroy_variant_value<id - 1>(v._data._next);
        }
    }

    template<typename ...Args>
    class Variant {
    public:
        Variant() = default;

        Variant(const Variant<Args...> &rhs) = default;

        Variant &operator=(const Variant<Args...> &rhs) = default;

        Variant(Variant<Args...> &&rhs) noexcept = default;

        Variant &operator=(Variant<Args...> &&rhs) noexcept = default;

        template<typename T>
        requires(!std::is_same_v<Variant, std::remove_cvref_t<T>>)
        Variant(T &&rhs) {
            _assign(std::forward<T>(rhs));
            _type_idx = find_idx_by_type<T, Args...>;
        }

        template<typename T>
        requires(!std::is_same_v<Variant, std::remove_cvref_t<T>>)
        Variant &operator=(T &&rhs) {
            _destroy();
            _assign(std::forward<T>(rhs));
            _type_idx = find_idx_by_type<T, Args...>;
            return *this;
        }

        ~Variant() {
            if constexpr (!_is_all_trivial) {
                _destroy();
            }
        }

        template<std::size_t id>
        auto &get() {
            return _storage.template get<id>();
        }

        template<typename type>
        auto &get() {
            return _storage.template get<type>();
        }

    private:
        VariantStorage<Args...> _storage{};
        std::size_t _type_idx{static_cast<std::size_t>(-1)};
        constexpr static bool _is_all_trivial{is_all_trivial_v<Args...>};
        constexpr static auto _size = sizeof...(Args);
        constexpr static std::size_t null_type{static_cast<std::size_t>(-1)};

        template<typename T>
        void _assign(T &&rhs) {
            _storage.assign(std::forward<T>(rhs));
        }

        template<typename T>
        static void _destroy_variant_value(void *vptr) {
            auto &v = *static_cast<VariantStorage<Args...> *> (vptr);
            destroy_variant_value<find_idx_by_type<T, Args...>>
                    (v);
        }

        void _destroy() {
            if (_type_idx != null_type) {
                destroy_func[_type_idx](&_storage);
            }
        }

        using destroy_func_type = void (*)(void *);
        constexpr static destroy_func_type destroy_func[] = {_destroy_variant_value<Args>...};
    };

    void test() {

    }

} // XQ::Tools

#endif //MYCONTAINER_VARIANT_H
