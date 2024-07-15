/**
  ******************************************************************************
  * @file           : Line.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/4/30
  ******************************************************************************
  */

#ifndef MYCONTAINER_LINE_H
#define MYCONTAINER_LINE_H

#include "Point.h"
#include "Vector2D.h"

namespace XQ::Maths::Planar {

    template<typename T, typename P = Point<T>, typename V = Vector2D<T, P>> requires requires(P p, V v) {
        P{T{}, T{}};
        V(p);
        p.x;
        p.y;
        v.collinear(v);
        v.modulu();
    }
    class Line {
    public:
        using value_type = T;
        using point_type = P;
        using vector_type = V;

        explicit Line(const vector_type &v = {}, const point_type &p = {T{}, T{}}) : _start{p}, _direction(v) {}

        [[nodiscard]] point_type start() const {
            return _start;
        }

        [[nodiscard]] vector_type direction() const {
            return _direction;
        }

        void setStart(const point_type &p) {
            _start = p;
        }

        void setDirection(const vector_type &v) {
            _direction = v;
        }

        [[nodiscard]] bool about(const point_type &p) const {
            return V(p - _start).collinear(_direction);
        }

        [[nodiscard]] double distanceToLine(const point_type &p) const {
            point_type a = _start, b = _start + _direction;
            vector_type v1 = b - a, v2 = p - a;
            return fabs(cross(v1, v2)) / v1.modulu();
        }

        [[nodiscard]] point_type getLineProjection(const point_type &p) const {
            point_type a = _start, b = _start + _direction;
            vector_type v = b - a;
            return a + v * (dot(v, p - a) / dot(v, v));
        }

    private:
        point_type _start;
        vector_type _direction;
    };

    template<typename T>
    Point<T> getLineIntersection(const Line<T> &lhs, const Line<T> &rhs) {
        Vector2D<T> u = lhs.start() - rhs.start();
        double t = cross(rhs.direction(), u) / cross(lhs.direction(), rhs.direction());
        return lhs.start() + u * t;
    }

} // XQ::Maths::Planar

#endif //MYCONTAINER_LINE_H
