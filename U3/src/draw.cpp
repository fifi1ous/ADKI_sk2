#include "draw.h"
#include <QtGui>
#include <time.h>
#include <QDebug>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{

    // inicialize variables
    view_points = true;
    view_dt = true;
    view_contour_lines = true;
    view_aspect = true;
    view_slope = true;
    clicked = true;

    //Initalize random number generator
    srand(time(NULL));

}

 //Initalize colors
const QColor COLORWHEEL[12] = {
    QColor(255,   0,   0),    // 0°   - Red
    QColor(255, 125,   0),   // 30°  - Orange
    QColor(255, 255,   0),   // 60°  - Yellow
    QColor(125, 255,   0),   // 90°  - Yellow-green
    QColor(  0, 255,   0),   // 120° - Green
    QColor(  0, 255, 125),   // 150° - Spring green
    QColor(  0, 255, 255),   // 180° - Cyan
    QColor(  0, 125, 255),   // 210° - Sky blue
    QColor(  0,   0, 255),   // 240° - Blue
    QColor(125,   0, 255),   // 270° - Violet
    QColor(255,   0, 255),   // 300° - Magenta
    QColor(255,   0, 125)    // 330° - Rose
};

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

    // Draw aspect
    if (view_aspect)
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

            double aspect = t.getAspect();

            painter.setBrush(COLORWHEEL[selectColor(aspect)]);
            painter.setPen(Qt::transparent);
            painter.drawPolygon(vertices);
        }
    }
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
        painter.setBrush(Qt::GlobalColor::black);

        const int r = 5;
        for (QPoint3DF point: points)
        {
           painter.drawEllipse(point.x()-r,point.y()-r, 2*r,2*r);
        }
    }

    //Draw DT
    if (view_dt)
    {
        painter.setPen(Qt::GlobalColor::black);
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

    painter.end();
}

int Draw::selectColor(const double &aspect)
{
    QColor aspect_color;
    double normalized = aspect + M_PI;

    // Compute sector index: floor(normalized / sectorSize)
    const double sectorSize = 2 * M_PI / 12.0;
    int index = static_cast<int>(normalized / sectorSize);

    return index;
}

