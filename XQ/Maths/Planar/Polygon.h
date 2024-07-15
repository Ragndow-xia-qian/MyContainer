/**
  ******************************************************************************
  * @file           : Polygon.h
  * @author         : 夏浅
  * @brief          : None
  * @attention      : None
  * @date           : 2024/4/30
  ******************************************************************************
  */

#ifndef MYCONTAINER_POLYGON_H
#define MYCONTAINER_POLYGON_H

#include <algorithm>

#include "../../Container/Sequence/ArrayList.h"

#include "Point.h"
#include "Vector2D.h"
#include "LineSegment.h"

namespace XQ::Maths::Planar {

    template<typename T, typename P = Point<T>, typename A = XQ::Container::Sequence::ArrayList<P>>
    class Polygon {
    public:
        using value_type = T;
        using size_type = A::size_type;
        using point_type = P;
        using array_type = A;

        Polygon() = default;

        explicit Polygon(array_type a) : _vertex(std::move(a)) {}

        Polygon(const Polygon &rhs) : _vertex(rhs._vertex) {}

        Polygon(Polygon &&rhs) noexcept: _vertex(std::move(rhs._vertex)) {}

        Polygon &operator=(Polygon rhs) {
            _vertex = std::move(rhs._vertex);
        }

        [[nodiscard]] point_type &operator[](size_type idx) {
            return _vertex[idx];
        }

        [[nodiscard]] const point_type &operator[](size_type idx) const {
            return const_cast<Polygon *>(this)->operator[](idx);
        }

        void add(const point_type &p) {
            _vertex.append(p);
        }

        void pop() {
            _vertex.pop();
        }

        [[nodiscard]] double area() const {
            double res = 0;
            for (int i = 1; i < _vertex.size() - 1; ++i) {
                res += cross(Vector2D<T, P>(_vertex[i] - _vertex[0]), Vector2D<T, P>(_vertex[i + 1] - _vertex[0]));
            }
            return res / 2;
        }

        [[nodiscard]] int about(const point_type &p) const {
            int wn = 0;
            for (int i = 0; i < _vertex.size(); ++i) {
                auto now = _vertex[i], nxt = _vertex[(i + 1) % _vertex.size()];
                if (LineSegment<T, P>(now, nxt).about(p)) {
                    return -1;
                }
                int k = sgn(cross(
                        Vector2D<T, P>(nxt) - Vector2D<T, P>(now),
                        Vector2D<T, P>(p) - Vector2D<T, P>(now)));
                int d1 = sgn(now.y - p.y);
                int d2 = sgn(nxt.y - p.y);
                if (k > 0 && d1 <= 0 && d2 > 0) {
                    ++wn;
                }
                if (k < 0 && d2 <= 0 && d1 > 1) {
                    --wn;
                }
            }
            return wn == 0 ? 0 : 1;
        }

        static void convecHull(const array_type &points, Polygon &res) {
            std::sort(points.begin(), points.end(), [](const point_type &lhs, const point_type &rhs) {
                return lhs.x < rhs.x || (sgn(lhs.x - rhs.x) == 0 && lhs.y < rhs.y);
            });
            for (int i = 0; i < points.size(); ++i) {
                while (res._vertex.size() > 1 &&
                       cross(Vector2D<T, P>(res[res._vertex.size() - 1]) - Vector2D<T, P>(res[res._vertex.size() - 2]),
                             points[i] - res[res._vertex.size() - 2]) <= 0) {
                    res.pop();
                }
                res.add(points[i]);
            }
            int k = res._vertex.size();
            for (int i = points.size() - 2; i >= 0; --i) {
                while (res._vertex.size() > k &&
                       cross(res[res._vertex.size() - 1] - res[res._vertex.size() - 2],
                             points[i] - res[res._vertex.size() - 2]) <= 0) {
                    res.pop();
                }
                res.add(points[i]);
            }
            if (points.size() > 1) {
                res.pop();
            }
        }

    private:
        array_type _vertex;
    };

} // XQ::Maths::Planar

#endif //MYCONTAINER_POLYGON_H
