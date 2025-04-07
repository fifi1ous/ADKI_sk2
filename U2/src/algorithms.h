#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>

#include <tuple>
#include <vector>
#include "../lib/Eigen/Dense"
#include "../lib/Eigen/Core"

class Algorithms
{
public:
    static QPolygonF createMAER(const QPolygonF &pol);
    static QPolygonF createERPCA(const QPolygonF &pol);
    static QPolygonF createERLE(const QPolygonF &pol);
    static QPolygonF createERWA(const QPolygonF &pol);
    static QPolygonF createERWB(const QPolygonF &pol);
    static QPolygonF createCHJS(const QPolygonF &pol);
    static QPolygonF createCHGS(const QPolygonF &pol);
    static void exportFile(const std::vector<QPolygonF> &results,const QString &fileName);

private:
    static double get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4);
    static double getArea(const QPolygonF &pol);
    static QPolygonF resize(const QPolygonF &pol, const QPolygonF &mmbox);
    static QPolygonF rotate(const QPolygonF &pol, double sigma);
    static std::tuple<QPolygonF, double> minMaxBox(const QPolygonF &pol);
    static double getDistance(const QPointF &p1, const QPointF &p2);
    static QPointF findPivotGS(const QPolygonF &pol);
    static std::vector<double> anglesWithPoints(const QPolygonF &pol, const QPointF &q);
    static void sortAnglesPoints(const QPointF &q, std::vector<double> &angles, QPolygonF &pol_);
    static double pointLineDistance(const QPointF& A, const QPointF& B, const QPointF& P);
    static short findSide(const QPointF& a, const QPointF& b, const QPointF& p);
};

#endif // ALGORITHMS_H
