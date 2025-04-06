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
    std::vector<QPolygonF> chs;
    QPolygonF currentPolygon;
    QPolygonF building;
    QPolygonF ch;
    QPolygonF er;
    bool isShapefileLoaded = false;

    QColor outlineCH = Qt::darkGreen;
    QColor fillCH = Qt::transparent;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *event) override;
    void switch_source();
    QPolygonF getPol()const {return building;}
    void loadPolygonFromTextfile(const QString &fileName);
    void loadPolygonFromShapefile(const QString &fileName);
    QPolygonF getBuilding()const{return building;}
    inline void setCH(QPolygonF &ch){this->ch = ch;}
    inline void setER(QPolygonF &er){this->er = er;}
    inline void clearPolygons(){ building.clear(); polygons.clear(); repaint(); }
    inline void clearResults(){ results.clear(); repaint(); }
    inline void clearCHs(){chs.clear(); repaint(); }
    inline const std::vector<QPolygonF> getPolygons() const { return polygons; }
    void setResults(const std::vector<QPolygonF>& newResults);
    inline void setConvexHulls(const std::vector<QPolygonF>& newCHs) {chs = newCHs; repaint(); }
    void changeColourCHOutline(const bool &status);
    void changeColourCHFilling(const bool &status);

signals:
};

#endif // DRAW_H
