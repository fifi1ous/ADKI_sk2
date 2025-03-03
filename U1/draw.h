#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <QPolygonF>
#include <QPointF>

class Draw : public QWidget
{
    Q_OBJECT

private:
    QPointF q;
    QPolygonF pol;
    bool add_point;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
    void switch_source();
    void loadPolygonFromFile(const QString &fileName);
    QPointF getQ() const {return q;}
    QPolygonF getPol() const {return pol;}

signals:
};

#endif // DRAW_H
