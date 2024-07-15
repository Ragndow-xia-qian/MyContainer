/**
  ******************************************************************************
  * @file           : Vector2D.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/4/30
  ******************************************************************************
  */

#ifndef MYCONTAINER_VECTOR2D_H
#define MYCONTAINER_VECTOR2D_H

#include <cmath>
#include "Point.h"

namespace XQ::Maths::Planar {
    template<typename T, typename P = Point<T>> requires requires(P p) {
        P{T{}, T{}};
        p.x;
        p.y;
    }
    struct Vector2D {
    public:
        using value_type = T;
        using point_type = P;

        Vector2D() = default;

        Vector2D(const Vector2D &) = default;

        Vector2D(Vector2D &&) noexcept = default;

        Vector2D(value_type x, value_type y) : _pos{x, y} {}

        explicit Vector2D(const point_type &p) : Vector2D(p.x, p.y) {}

        [[nodiscard]] value_type x() const {
            return _pos.x;
        }

        [[nodiscard]] value_type y() const {
            return _pos.y;
        }

        void setX(value_type x) {
            _pos.x = x;
        }

        void setY(value_type y) {
            _pos.y = y;
        }

        [[nodiscard]] double modulu() const;

        [[nodiscard]] Vector2D rotate(double rad) const {
            double sin_rad = std::sin(rad);
            double cos_rad = std::cos(rad);
            return {_pos.x * cos_rad - _pos.y * sin_rad, _pos.x * sin_rad + _pos.y * cos_rad};
        }

        [[nodiscard]] bool collinear(const Vector2D &rhs) const;

    private:
        point_type _pos;
    };

    template<typename T>
    Vector2D<T> operator+(const Vector2D<T> &lhs, const Vector2D<T> &rhs) {
        return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
    }

    template<typename T>
    Point<T> operator+(const Point<T> &lhs, const Vector2D<T> &rhs) {
        return {lhs.x + rhs.x(), lhs.y + rhs.y()};
    }

    template<typename T>
    Point<T> operator+(const Point<T> &lhs, const Point<T> &rhs) {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }

    template<typename T>
    Vector2D<T> operator-(const Vector2D<T> &lhs, const Vector2D<T> &rhs) {
        return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
    }

    template<typename T>
    Point<T> operator-(const Point<T> &lhs, const Vector2D<T> &rhs) {
        return {lhs.x - rhs.x(), lhs.y - rhs.y()};
    }

    template<typename T>
    Point<T> operator-(const Point<T> &lhs, const Point<T> &rhs) {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }

    template<typename T>
    Vector2D<double> operator*(const Vector2D<T> &lhs, double k) {
        return {lhs.x() * k, lhs.y() * k};
    }

    template<typename T>
    T dot(const Vector2D<T> &lhs, const Vector2D<T> &rhs) {
        return lhs.x() * rhs.x() + lhs.y() * rhs.y();
    }

    template<typename T, typename P>
    requires requires(P p) {
        P{T{}, T{}};
        p.x;
        p.y;
    }
    double Vector2D<T, P>::modulu() const {
        return std::sqrt(dot(*this, *this));
    }

    template<typename T>
    T cross(const Vector2D<T> &lhs, const Vector2D<T> &rhs) {
        return lhs.x() * rhs.y() + lhs.y() * rhs.x();
    }

    template<typename T, typename P>
    requires requires(P p) {
        P{T{}, T{}};
        p.x;
        p.y;
    }
    bool Vector2D<T, P>::collinear(const Vector2D &rhs) const {
        return sgn(cross(*this, rhs)) == 0;
    }

    template<typename T>
    double angle(const Vector2D<T> &lhs, const Vector2D<T> &rhs) {
        return std::acos(lhs * rhs / lhs.modulu() / rhs.modulu());
    }

    template<typename T>
    double AreaTriangle(Point<T> A, Point<T> B, Point<T> C) {
        Vector2D<T> pA(A), pB(B), pC(C);
        return std::fabs(cross(pB - pA, pC - pA)) / 2;
    }
} // XQ::Maths::Planar

#endif //MYCONTAINER_VECTOR2D_H
