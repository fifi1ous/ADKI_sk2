#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPolygonF>
#include <vector>
#include <QPainterPath>
#include <QMouseEvent>

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPolygonF> polygons;
    std::vector<QPolygonF> results;
    QPolygonF currentPolygon;
    QPolygonF building;
    QPolygonF ch;
    QPolygonF maer;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void switch_source();
    QPolygonF getPol()const {return building;}
    void loadPolygonFromTextfile(const QString &fileName);
    void loadPolygonFromShapefile(const QString &fileName);
    QPolygonF getBuilding()const{return building;}
    void setCH(QPolygonF &ch_){ch = ch_;}
    void setMAER(QPolygonF &maer_){maer = maer_;}
    void clearPolygons();
    void clearResults();
    const std::vector<QPolygonF> getPolygons() const { return polygons; }
    void setResults(const std::vector<QPolygonF>& newResults);

signals:
};

#endif // DRAW_H
