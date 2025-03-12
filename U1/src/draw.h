#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPolygonF>
#include <QPointF>
#include <vector>

class Draw : public QWidget
{
    Q_OBJECT

private:
    QPointF q;
    QPolygonF currentPolygon;
    std::vector<QPolygonF> polygons;
    std::vector<QPolygonF> selectedPolygons;
    bool add_point;
    bool isShapefileLoaded;
    QRectF boundingBox;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void switch_source();
    void loadPolygonFromFile(const QString &fileName);
    void loadPolygonFromShapefile(const QString &fileName);
    QPointF getQ() const { return q; }
    const std::vector<QPolygonF>& getPolygons() const { return polygons; }
    void clearPolygons();
    void addSelectedPolygon(const QPolygonF& polygon);
    void clearSelectedPolygons();

signals:
};

#endif // DRAW_H
