/**
  ******************************************************************************
  * @file           : Point.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/4/30
  ******************************************************************************
  */

#ifndef MYCONTAINER_POINT_H
#define MYCONTAINER_POINT_H

#include <type_traits>

namespace XQ::Maths::Planar {
    template<typename T>
    requires requires(T x, T y) {
        x + y;
        x - y;
        x * y;
    }
    struct Point {
        using value_type = T;

        value_type x, y;
    };

    [[nodiscard]] int sgn(double x) {
        if (x < 1e-6) {
            return 0;
        }
        return x > 0 ? 1 : -1;
    }
} // XQ::Maths::Planar

#endif //MYCONTAINER_POINT_H
