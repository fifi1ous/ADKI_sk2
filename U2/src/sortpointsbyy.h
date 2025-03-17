#ifndef SORTPOINTSBYY_H
#define SORTPOINTSBYY_H

#include <QtGui>

class sortPointsByY
{
public:
    sortPointsByY();
    bool  operator()(const QPointF &p1, const QPointF &p2) {return p1.y() < p2.y();}
};

#endif // SORTPOINTSBYY_H
