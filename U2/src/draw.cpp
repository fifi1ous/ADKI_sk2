#include "draw.h"
#include <QtGui>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{
    q.setX(0);
    q.setY(0);
    add_point = false;

}


void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get x, y coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    //Set x,y to q
    if (add_point)
    {
        q.setX(x);
        q.setY(y);
    }

    //Add point to polygon
    else
    {
        //Create point
        QPointF p(x, y);

        //Add point to polygon
        pol.push_back(p);
    }

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

    //Draw polygon
    painter.drawPolygon(pol);

    //Set graphic attributes, point
    painter.setPen(Qt::GlobalColor::black);
    painter.setBrush(Qt::GlobalColor::blue);

    //Draw point
    const int r = 5;
    painter.drawEllipse(q.x()-r,q.y()-r, 2*r,2*r);

    //End draw
    painter.end();
}


void Draw::switch_source()
{
    //Input q or polygon vertex
    add_point = !add_point;
}


void Draw::loadPolygonFromTextfile(const QString &fileName)
{

}

void Draw::loadPolygonFromShapefile(const QString &fileName)
{

}
