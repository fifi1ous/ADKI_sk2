#include "draw.h"
#include <QtGui>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{
    //     QPointF a(50,70);
    //     QPointF b(150,50);
    //     QPointF c(150,130);
    //     QPointF d(50,150);

    //     building.push_back(a);
    //     building.push_back(b);
    //     building.push_back(c);
    //     building.push_back(d);
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get x, y coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    //Add point to polygon
    //Create point
    QPointF p(x, y);

    //Add point to polygon
    building.push_back(p);

    //Repaint screen
    repaint();
}


void Draw::paintEvent(QPaintEvent *event)
{
    //Draw
    QPainter painter(this);

    //Create object for drawing
    painter.begin(this);

    //Set graphic attributes, polygon
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);

    //Draw building
    painter.drawPolygon(building);

    //Set graphics for CH
    painter.setPen(Qt::GlobalColor::cyan);
    painter.setPen(Qt::PenStyle::DashLine);
    painter.setBrush(Qt::GlobalColor::transparent);

    //Draw polygon
    painter.drawPolygon(ch);


    //Set graphics for maer
    painter.setPen(Qt::GlobalColor::magenta);
    painter.setBrush(Qt::GlobalColor::transparent);

    //Draw polygon
    painter.drawPolygon(maer);

    //End draw
    painter.end();
}

void Draw::loadPolygonFromTextfile(const QString &fileName)
{

}

void Draw::loadPolygonFromShapefile(const QString &fileName)
{

}
