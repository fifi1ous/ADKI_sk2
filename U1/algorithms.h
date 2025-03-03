#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QPointF>
#include <QPolygonF>

class Algorithms
{
public:
    static short analyzeRayCrossing(const QPointF &q, const QPolygonF &pol);
    static short analyzeWindingNumber(const QPointF &q, const QPolygonF &pol);
private:
    static double calculateDistance(const QPointF &p1, const QPointF &p2);
    static double calculateCosineValue(double l_qi, double l_qi1, double l_ii1);
    static double calculateDeterminant(const QPointF &p1, const QPointF &p2, const QPointF &q);
};

#endif // ALGORITHMS_H
