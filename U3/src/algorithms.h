#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>
#include <list>
#include "qpoint3df.h"
#include "edge.h"
#include "sortpointsbyx.h"
#include "traingle.h"


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
    QPoint3DF countourLinePoint(const QPoint3DF &p1,const QPoint3DF &p2, double z);
    std::vector<Edge> createContourLines(const std::vector<Edge> &dt, const double zmin, const double zmax, const double dz);
    double computeSlope(const QPoint3DF &p1, const QPoint3DF &p2,const QPoint3DF &p3);
    void analyzeSlope (const std::vector<Edge> &dt, std::vector<Traingle> &triangles);
    double computeAspect(const QPoint3DF &p1, const QPoint3DF &p2,const QPoint3DF &p3);
    void analyzeAspect(const std::vector<Edge> &dt, std::vector<Traingle> &triangles);

    void edgesToTriangle(const std::vector<Edge> &dt, std::vector<Traingle> &triangles);
};

#endif // ALGORITHMS_H
