#ifndef SORTPOINTSBYX_H
#define SORTPOINTSBYX_H

#include <QtGui>

class sortPointsByX
{
public:
    sortPointsByX();
    bool  operator()(const QPointF &p1, const QPointF &p2) {return p1.x() < p2.x();}

};

#endif // SORTPOINTSBYX_H
