#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>
class Algorithms
{
public:
    Algorithms();
    static double get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);

    static QPolygonF createCH(const QPolygonF &pol);

};

#endif // ALGORITHMS_H
