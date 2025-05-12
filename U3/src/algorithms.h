#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>
#include <list>
#include "qpoint3df.h"
#include "edge.h"
#include "sortpointsbyx.h"
#include "triangle.h"


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
    void analyzeSlope (const std::vector<Edge> &dt, std::vector<Triangle> &triangles, const bool &click);
    double computeAspect(const QPoint3DF &p1, const QPoint3DF &p2,const QPoint3DF &p3);
    void analyzeAspect(const std::vector<Edge> &dt, std::vector<Triangle> &triangles, const bool &click);

    void edgesToTriangle(const std::vector<Edge> &dt, std::vector<Triangle> &triangles);

    // Terrain shape generators
    static std::vector<QPoint3DF> generateHill(int n, int width, int height, int cx, int cy, int rx, int ry, int maxZ);
    static std::vector<QPoint3DF> generateValley(int n, int width, int height, int cx, int cy, int rx, int ry, int depth);
    static std::vector<QPoint3DF> generateRidge(int n, int width, int height, int x1, int y1, int x2, int y2, int maxZ);
    static std::vector<QPoint3DF> generateBench(int n, int width, int height, int stepStartX, int stepEndX, int depthZ);
    static std::vector<QPoint3DF> generateSaddle(int n, int width, int height, int cx, int cy, int scaleX, int scaleY);


};

#endif // ALGORITHMS_H
