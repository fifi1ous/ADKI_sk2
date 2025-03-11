#ifndef SORTPOINTSBYX_H
#define SORTPOINTSBYX_H

#include <QtGui>

class sortPointsByX
{
public:
    sortPointsByX();
    bool operator() (const QPointF &p1, const QPointF &p2);

};

#endif // SORTPOINTSBYX_H
