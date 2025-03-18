#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>

#include <tuple>
#include "../lib/Eigen/Dense"
#include "../lib/Eigen/Core"

class Algorithms
{
public:
    Algorithms();

    static double get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);
    static QPolygonF createCH(const QPolygonF &pol);
    static std::tuple<QPolygonF, double> minMaxBox(const QPolygonF &pol);
    static QPolygonF rotate(const QPolygonF &pol, double sigma);
    static double getArea(const QPolygonF &pol);
    static QPolygonF resize(const QPolygonF &pol, const QPolygonF &mmbox);
    static QPolygonF createMAER(const QPolygonF &pol);
    static QPolygonF createERPCA(const QPolygonF &pol);

};

#endif // ALGORITHMS_H
