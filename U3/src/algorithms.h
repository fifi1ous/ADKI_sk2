#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>
#include <list>
#include "qpoint3df.h"
#include "edge.h"
#include "sortpointsbyx.h"


class Algorithms
{
public:
    Algorithms();
    short getPointAndLinePosition(const QPoint3DF &p,const QPoint3DF &p1,const QPoint3DF &p2);
    double get2LinesAngle(const QPoint3DF &p1, const QPoint3DF &p2, const QPoint3DF &p3, const QPoint3DF &p4);
    int findDelaunayPoint(const QPoint3DF &p1, const QPoint3DF &p2,const std::vector<QPoint3DF> &points);
    int findNearestPoint(const QPoint3DF &p,const std::vector<QPoint3DF> &points);
    double get2DDistance(const QPoint3DF &p1, const QPoint3DF &p2);
    std::vector<Edge> DT(const std::vector<QPoint3DF> &points);
    void updateAEL(const Edge &e, std::list<Edge> &ael);
    QPoint3DF countourLinePoint(QPoint3DF p1,QPoint3DF p2, double z);
    std::vector<Edge> createContourLines(const std::vector<Edge> &dt, const double zmin, const double zmax, const double dz);
};

#endif // ALGORITHMS_H
