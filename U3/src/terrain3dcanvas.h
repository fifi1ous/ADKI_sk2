#ifndef TERRAIN3DCANVAS_H
#define TERRAIN3DCANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <vector>
#include "qpoint3df.h"

class Terrain3DCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit Terrain3DCanvas(QWidget *parent = nullptr);
    void setPoints(const std::vector<QPoint3DF> &points);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    std::vector<QPoint3DF> points;
    double angleX;
    double angleY;
    double scale;
    QPoint lastMousePos;

    QPoint projectPoint(const QPoint3DF &p) const;
};

#endif // TERRAIN3DCANVAS_H
