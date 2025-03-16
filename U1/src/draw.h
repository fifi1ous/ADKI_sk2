#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPolygonF>
#include <QPointF>
#include <vector>
#include <QPainterPath>
#include <QMouseEvent>
#include "Polygon.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    QPointF q;
    QPolygonF currentPolygon;
    QPolygonF currentHole;
    Polygon_ curentCPolygon;
    QPainterPath curentPolygonWH;
    std::vector<QPainterPath> polygonsWH;
    std::vector<QPainterPath> selectedPolygonsWH;
    std::vector<Polygon_> polygonComplex;
    bool add_point;
    bool isShapefileLoaded;
    bool isPolygonReady = false;
    QRectF boundingBox;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void switch_source();
    void loadPolygonFromFile(const QString &fileName);
    void loadPolygonFromShapefile(const QString &fileName);
    QPointF getQ() const { return q; }
    const std::vector<Polygon_>& getPolygons() const { return polygonComplex; }
    const std::vector<QPainterPath>& getPolygonsWH() const { return polygonsWH; }
    void clearPolygons();
    void addSelectedPolygon(const QPainterPath& selection);
    void clearSelectedPolygons();
private:
    void mousePressEventLeft(QMouseEvent *e);
    void mousePressEventRight(QMouseEvent *e);
    void addPointToPath(QPointF p);
signals:
};

#endif // DRAW_H
