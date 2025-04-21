#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpoint3df.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    std::vector<QPoint3DF> points;
    std::vector<Edge> dt;
    std::vector<Edge> contour_lines;


public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);

    std::vector<QPoint3DF> getPoints()const {return points;}
    std::vector<Edge> getDT()const {return dt;}
    void setDT(const std::vector<Edge> &dt_){dt = dt_;}
    void setCL(const std::vector<Edge> &contour_lines_){contour_lines = contour_lines_;}


signals:
};

#endif // DRAW_H
