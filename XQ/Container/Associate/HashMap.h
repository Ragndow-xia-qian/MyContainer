/**
  ******************************************************************************
  * @file           : HashMap.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/2/21
  ******************************************************************************
  */
#ifndef MYCONTAINER_HASHMAP_H
#define MYCONTAINER_HASHMAP_H

// 未完工

#include <algorithm>

namespace XQ::Container::Associate {

    template<typename T1, typename T2>
    struct Pair {
        using first_type = T1;
        using second_type = T2;

        first_type first;
        second_type second;

        constexpr Pair(const Pair &) = default;

        constexpr Pair(Pair &&) = default;

        void swap(Pair &_x) {
            using std::swap;
            swap(this->first, _x.first);
            swap(this->second, _x.second);
        }
    };

    template<typename T1, typename T2>
    constexpr std::common_comparison_category_t<std::__detail::__synth3way_t<T1>,
            std::__detail::__synth3way_t<T2>>
    operator<=>(const Pair<T1, T2> &_x, const Pair<T1, T2> &_y) {
        if (auto _c = std::__detail::__synth3way(_x.first, _y.first); _c != 0) {
            return _c;
        }
        return std::__detail::__synth3way(_x.second, _y.second);
    }

    template<typename Key, typename Tp, typename HashFun = std::hash<Key>>
    class HashMap {
    public:
        using key_type = Key;
        using value_type = Tp;

    private:

    };

} // XQ::Container::Associate

#endif //MYCONTAINER_HASHMAP_H
