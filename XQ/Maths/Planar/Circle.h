/**
  ******************************************************************************
  * @file           : Circle.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/4/30
  ******************************************************************************
  */

#ifndef MYCONTAINER_CIRCLE_H
#define MYCONTAINER_CIRCLE_H

#include "../../Container/Sequence/ArrayList.h"

#include "Point.h"
#include "Line.h"

namespace XQ::Maths::Planar {

    template<typename T, typename P = Point<T>>
    class Circle {
    public:
        using value_type = T;
        using point_type = P;

        Circle() = default;

        explicit Circle(const value_type &r, const point_type &o = {T{}, T{}}) : _centre(o), _radius(r) {}

        [[nodiscard]] point_type centre() const {
            return _centre;
        }

        [[nodiscard]] value_type radius() const {
            return _radius;
        }

        [[nodiscard]] point_type point(double rad) const {
            return {_centre.x + cos(rad) * _radius, _centre.y + sin(rad) * _radius};
        }

        [[nodiscard]] int getLineCircleIntersection(const Line<T, P, Vector2D<T, P>> &l, XQ::Container::Sequence::ArrayList<point_type> &res) const {
            point_type A = l.start(), B = l.start() + l.direction();
            double a = (B.x - A.x) * (B.x - A.x) + (B.y - A.y) * (B.y - A.y);
            double b = 2 * (B.x - A.x) * (A.x - _centre.x) + 2 * (B.y - A.y) * (A.y - _centre.y);
            double c =  (A.x - _centre.x) * (A.x - _centre.x) + (A.y - _centre.y) * (A.y - _centre.y) - _radius * _radius;
            double delta = b * b - 4 * a * c;
            if (delta < 0) {
                return 0;
            }
            if (delta == 0) {
                double t = -b / (2 * a);
                res.push_back(A + (B - A) * t);
                return 1;
            }
            double t1 = (-b + sqrt(delta)) / (2 * a);
            double t2 = (-b - sqrt(delta)) / (2 * a);
            res.push_back(A + (B - A) * t1);
            res.push_back(A + (B - A) * t2);
            return 2;
        }

    private:
        point_type _centre;
        value_type _radius;
    };

    template<typename T>
    [[nodiscard]] int getTangents(const Point<T> &p, const Circle<T, Point<T>> &c, XQ::Container::Sequence::ArrayList<Point<T>> &res) {
        Vector2D<T, Point<T>> u = c.centre() - p;
        double d = u.modulu();
        if (sgn(d - c.radius()) < 0) {
            return 0;
        }
        if (sgn(d - c.radius()) == 0) {
            res.push_back(u.rotate(M_PI / 2));
            return 1;
        }
        double rad = asin(c.radius() / d);
        res.push_back(u.rotate(-rad));
        res.push_back(u.rotate(rad));
        return 2;
    }

    template<typename T>
    [[nodiscard]] int getTangents(const Circle<T, Point<T>> &c1,
                                  const Circle<T, Point<T>> &c2,
                                  XQ::Container::Sequence::ArrayList<Point<T>> &res1,
                                  XQ::Container::Sequence::ArrayList<Point<T>> &res2) {
        auto C1 = c1;
        auto C2 = c2;
        if (C1.radius() < C2.radius()) {
            using std::swap;
            swap(C1, C2);
            swap(res1, res2);
        }
        double d = Vector2D<T, Point<T>>(C1.centre() - C2.centre()).modulu();
        T rdiff = C1.radius() - C2.radius();
        T rsum = C1.radius() + C2.radius();
        if (sgn(d - rdiff) < 0) {
            return 0;
        }
        if (sgn(d) == 0 && sgn(rdiff) == 0) {
            return -1;
        }
        double base = angle(Vector2D<T, Point<T>>(C1.centre() - C2.centre()), {1, 0});
        if (sgn(d - rdiff) == 0) {
            res1.push_back(C1.point(base));
            res2.push_back(C2.point(base));
            return 1;
        }
        double rad = acos(rdiff / d);
        res1.push_back(C1.point(base + rad));
        res2.push_back(C2.point(base + rad));
        res1.push_back(C2.point(base - rad));
        res2.push_back(C2.point(base - rad));
        if (sgn(d - rsum) < 0) {
            return 2;
        }
        if (sgn(d - rsum) == 0) {
            res1.push_back(C1.point(base));
            res2.push_back(C2.point(M_PI + base));
            return 3;
        }
        rad = acos(rsum / d);
        res1.push_back(C1.point(base + rad));
        res2.push_back(C2.point(M_PI + base + rad));
        res1.push_back(C2.point(base - rad));
        res2.push_back(C2.point(M_PI + base - rad));
        return 4;
    }

} // XQ::Maths::Planar

#endif //MYCONTAINER_CIRCLE_H
