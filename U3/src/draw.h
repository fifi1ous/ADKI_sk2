#ifndef DRAW_H
#define DRAW_H

#include <QWidget>

class Draw : public QWidget
{
    Q_OBJECT

private:
    QPolygonF building;
    QPolygonF ch;
    QPolygonF er;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    QPolygonF getBuilding()const {return building;}
    void setCH(QPolygonF &ch_){ch = ch_;}
    void setER(QPolygonF &er_){er = er_;}

signals:
};

#endif // DRAW_H
