#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>

#include <tuple>
#include "../lib/Eigen/Dense"
#include "../lib/Eigen/Core"

class Algorithms
{
public:
    //Algorithms() = delete; // Prevent instantiation of object of this class

    static QPolygonF createMAER(const QPolygonF &pol);
    static QPolygonF createERPCA(const QPolygonF &pol);
    static QPolygonF createERLE(const QPolygonF &pol);
    static QPolygonF createERWA(const QPolygonF &pol);
private:
    static double get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);
    static double getArea(const QPolygonF &pol);
    static QPolygonF resize(const QPolygonF &pol, const QPolygonF &mmbox);
    static QPolygonF rotate(const QPolygonF &pol, double sigma);
    static QPolygonF createCH(const QPolygonF &pol);
    static std::tuple<QPolygonF, double> minMaxBox(const QPolygonF &pol);
};

#endif // ALGORITHMS_H
