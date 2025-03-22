#include "draw.h"
#include <QMouseEvent>
#include <QtGui>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

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
    // Handle double-click to finish polygon creation
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        // Pokud existuje nějaký aktuální polygon
        if (!building.isEmpty())
        {
            // Přidání aktuálního polygonu do seznamu polygonů
            polygons.push_back(building);

            // Vyprázdnění aktuálního polygonu pro nový začátek
            building.clear();
        }
        repaint();
        return;
    }
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


// void Draw::paintEvent(QPaintEvent *event)
// {
//     //Draw
//     QPainter painter(this);

//     //Create object for drawing
//     painter.begin(this);

//     //Set graphic attributes, polygon
//     painter.setPen(Qt::GlobalColor::red);
//     painter.setBrush(Qt::GlobalColor::yellow);

//     //Draw building
//     painter.drawPolygon(building);

//     // Draw all stored polygons
//     for (const auto& poly : polygons) {
//         painter.drawPolygon(poly);
//     }

//     //Set graphics for CH
//     painter.setPen(Qt::GlobalColor::cyan);
//     painter.setPen(Qt::PenStyle::DashLine);
//     painter.setBrush(Qt::GlobalColor::transparent);

//     //Draw polygon
//     painter.drawPolygon(ch);

//     //Set graphics for maer
//     painter.setPen(Qt::GlobalColor::magenta);
//     painter.setBrush(Qt::GlobalColor::transparent);

//     //Draw polygon
//     painter.drawPolygon(maer);

//     //End draw
//     painter.end();
// }
void Draw::paintEvent(QPaintEvent *event)
{
    //Draw
    QPainter painter(this);

    //Create object for drawing
    painter.begin(this);

    //Set graphic attributes of building
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);

    //Draw building
    painter.drawPolygon(building);

    //Draw all stored buildings
    for (const auto& poly : polygons) {
        painter.drawPolygon(poly);
    }

    //Set graphics for maer
    painter.setPen(Qt::GlobalColor::magenta);
    painter.setBrush(Qt::GlobalColor::transparent);

    //Draw all results
    for (const auto& result : results) {
        painter.drawPolygon(result);
    }

    //Set graphic atributes for Convex Hull (CH)
    painter.setPen(Qt::GlobalColor::cyan);
    painter.setPen(Qt::PenStyle::DashLine);
    painter.setBrush(Qt::GlobalColor::transparent);
    painter.drawPolygon(ch);

    //Set graphic atributes for Minimum Bounding Rectangle (MAER)
    painter.setPen(Qt::GlobalColor::magenta);
    painter.setBrush(Qt::GlobalColor::transparent);
    painter.drawPolygon(maer);

    painter.end();
}

void Draw::loadPolygonFromTextfile(const QString &fileName)
{

}

void Draw::loadPolygonFromShapefile(const QString &fileName)
{

}

void Draw::clearPolygons()
{
    building.clear();
    polygons.clear();
    repaint();
}

void Draw::clearResults()
{
    maer.clear();
    repaint();
}
