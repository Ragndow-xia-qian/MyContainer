/**
  ******************************************************************************
  * @file           : LineSegment.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/4/30
  ******************************************************************************
  */

#ifndef MYCONTAINER_LINESEGMENT_H
#define MYCONTAINER_LINESEGMENT_H

#include "Point.h"
#include "Line.h"

namespace XQ::Maths::Planar {

    template<typename T, typename P = Point<T>> requires requires(P p) {
        P{T{}, T{}};
        p.x;
        p.y;
    }
    class LineSegment {
    public:
        using value_type = T;
        using point_type = P;

        LineSegment() = default;

        explicit LineSegment(const Line<T, P, Vector2D<T, P>> &l, const Vector2D<T, P> &p = {T{}, T{}}) :
                _begin(l._start), _end(l._start + p) {}

        LineSegment(const point_type &begin, const point_type &end) : _begin(begin), _end(end) {}

        double distanceToSegment(const point_type &p) {
            point_type a = _begin, b = _end;
            if (sgn(dot(p - a, b - a)) < 0) {
                return Vector2D<T, P>(p - a).modulu();
            }
            if (sgn(dot(p - b, a - b)) < 0) {
                return Vector2D<T, P>(p - b).modulu();
            }
            return fabs(cross(b - a, p - a) / Vector2D<T, P>(a - b).modulu());
        }

        bool about(const point_type &p) {
            point_type a = _begin, b = _end;
            return sgn(cross(a - p, b - p)) == 0 && sgn(dot(a - p, b - p)) < 0;
        }

    private:
        P _begin, _end;
    };

    template<typename T>
    bool segmentProperIntersection(const LineSegment<T, Point<T>> &lhs, const LineSegment<T, Point<T>> &rhs) {
        Vector2D<T, Point<T>> a = lhs._begin, b = lhs._end;
        Vector2D<T, Point<T>> p = rhs._begin, q = rhs._end;
        double c1 = cross(b - a, p - a);
        double c2 = cross(b - a, q - a);
        double c3 = cross(q - p, a - p);
        double c4 = cross(q - p, b - p);
        return sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0;
    }

} // XQ::Maths::Planar

#endif //MYCONTAINER_LINESEGMENT_H
