#include "draw.h"
#include <QtGui>
#include <time.h>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{

    QPoint3DF a(50,70);
    QPoint3DF b(150,50);
    QPoint3DF c(150,130);
    QPoint3DF d(50,150);

    points.push_back(a);
    points.push_back(b);
    points.push_back(c);
    points.push_back(d);

    //Initalize random number generator
    srand(time(NULL));
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get x, y coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    //Random height of the point
    double z = rand()%1000 + 100.0;

    //Create point
    QPoint3DF p(x, y, z);

    //Add point to cloud
    points.push_back(p);

    //Repaint screen
    repaint();
}


void Draw::paintEvent(QPaintEvent *event)
{
    //Draw
    QPainter painter(this);

    //Create object for drawing
    painter.begin(this);


    //Set graphic attributes, point
    painter.setPen(Qt::GlobalColor::black);
    painter.setBrush(Qt::GlobalColor::blue);

    //Draw points
    const int r = 5;
    for (QPoint3DF point: points)
    {
       painter.drawEllipse(point.x()-r,point.y()-r, 2*r,2*r);
    }

    //Draw DT
    painter.setPen(Qt::GlobalColor::green);
    for (Edge e: dt)
    {
        painter.drawLine(e.getStart(), e.getEnd());
    }

    //Draw contour lines
    painter.setPen(Qt::GlobalColor::darkGray);
    for (Edge e: contour_lines)
    {
        painter.drawLine(e.getStart(), e.getEnd());
    }


    /*

    //Draw polygon
    painter.drawPolygon(ch);


    //Set graphics for maer
    painter.setPen(Qt::GlobalColor::magenta);
    painter.setBrush(Qt::GlobalColor::transparent);

    //Draw polygon
    painter.drawPolygon(er);

    */
    //End draw
    painter.end();
}


