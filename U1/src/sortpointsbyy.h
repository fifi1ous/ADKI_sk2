#ifndef SORTPOINTSBYY_H
#define SORTPOINTSBYY_H

#include <QtGui>


class sortPointsByY
{
public:
    sortPointsByY();

    bool operator() (const QPointF &p1, const QPointF &p2);
};

#endif // SORTPOINTSBYY_H
