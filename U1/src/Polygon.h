#ifndef POLYGON_H
#define POLYGON_H

#include <QPolygonF>
#include <vector>

struct Polygon_ {
    QPolygonF outer;
    std::vector<QPolygonF> holes;
};

#endif // POLYGON_H

