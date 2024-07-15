/**
  ******************************************************************************
  * @file           : Hash.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/3/22
  ******************************************************************************
  */
#ifndef MYCONTAINER_HASH_H
#define MYCONTAINER_HASH_H

// 未完工

#include <type_traits>

namespace XQ::Container::Associate {

    template<typename T, typename = void>
    struct Hash;

    template<typename T>
    struct Hash<T, std::enable_if_t<std::is_integral_v<T>>> {
        std::size_t operator()(T value) {
            return value;
        }
    };

} // XQ::Container::Associate

#endif //MYCONTAINER_HASH_H
