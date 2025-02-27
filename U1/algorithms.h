#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>

class Algorithms
{
public:
    Algorithms();
    static short analyzePointAndPolygonPosition(const QPointF &q, const QPolygonF &pol);
};

#endif // ALGORITHMS_H
