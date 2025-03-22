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
        QPointF a(59,33);
        QPointF b(263,56);
        QPointF c(236,185);
        QPointF d(65,149);

        building.push_back(a);
        building.push_back(b);
        building.push_back(c);
        building.push_back(d);

        // Simulate double-click after 0.5s delay
        QTimer::singleShot(500, this, [this]() {
            // Simulate the first mouse press event
            QMouseEvent *firstClick = new QMouseEvent(QEvent::MouseButtonPress, QPointF(100, 100), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            mousePressEvent(firstClick);

            // Simulate the second mouse press event (double-click)
            QMouseEvent *secondClick = new QMouseEvent(QEvent::MouseButtonDblClick, QPointF(100, 100), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
            mousePressEvent(secondClick);
        });
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

    //For testing
    qDebug() << "x = "<< x << ", y = " << y << "\n" ;


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
