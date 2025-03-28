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
    QPolygonF er;
    bool isShapefileLoaded = false;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    void switch_source();
    QPolygonF getPol()const {return building;}
    void loadPolygonFromTextfile(const QString &fileName);
    void loadPolygonFromShapefile(const QString &fileName);
    QPolygonF getBuilding()const{return building;}
    void setCH(QPolygonF &ch_){ch = ch_;}
    inline void setER(QPolygonF &er){this->er = er;}
    void clearPolygons();
    void clearResults();
    inline const std::vector<QPolygonF> getPolygons() const { return polygons; }
    void setResults(const std::vector<QPolygonF>& newResults);

signals:
};

#endif // DRAW_H
