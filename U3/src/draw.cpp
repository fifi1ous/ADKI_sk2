#include "draw.h"
#include <QtGui>
#include <time.h>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{

    // inicialize variables
    view_points = true;
    view_dt = true;
    view_contour_lines = true;
    view_aspect = true;
    view_slope = true;

    //Initalize random number generator
    srand(time(NULL));
}

bool Draw::clicked = true;


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

    // Change clicked
    clicked = true;
}


void Draw::paintEvent(QPaintEvent *event)
{
    //Draw
    QPainter painter(this);

    //Create object for drawing
    painter.begin(this);

    // Draw slope
    if (view_slope)
    {
        for (Triangle t: triangles)
        {

            QPoint3DF p1 = t.getP1();
            QPoint3DF p2 = t.getP2();
            QPoint3DF p3 = t.getP3();

            QPointF p1_(p1.x(),p1.y());
            QPointF p2_(p2.x(),p2.y());
            QPointF p3_(p3.x(),p3.y());

            QPolygonF vertices {p1_,p2_,p3_};

            double slope = t.getSlope();

            int color = 255 - 255/M_PI * slope;

            painter.setBrush(QColor(color,color,color));
            painter.setPen(Qt::transparent);
            painter.drawPolygon(vertices);
        }
    }

    //Draw points
    if (view_points)
    {
        //Set graphic attributes, point
        painter.setPen(Qt::GlobalColor::black);
        painter.setBrush(Qt::GlobalColor::blue);

        const int r = 5;
        for (QPoint3DF point: points)
        {
           painter.drawEllipse(point.x()-r,point.y()-r, 2*r,2*r);
        }
    }

    //Draw DT
    if (view_dt)
    {
        painter.setPen(Qt::GlobalColor::green);
        for (Edge e: dt)
        {
            painter.drawLine(e.getStart(), e.getEnd());
        }
    }

    //Draw contour lines
    if (view_contour_lines)
    {
        painter.setPen(Qt::GlobalColor::darkGray);
        for (Edge e: contour_lines)
        {
            painter.drawLine(e.getStart(), e.getEnd());
        }
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


