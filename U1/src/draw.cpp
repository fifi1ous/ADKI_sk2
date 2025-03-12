#include "draw.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include "../lib/shapefil.h"

Draw::Draw(QWidget *parent)
    : QWidget{parent}, add_point{false}
{
    q.setX(0);
    q.setY(0);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        if (!currentPolygon.isEmpty())
        {
            polygons.push_back(currentPolygon); // Add actual polygon to the vector
            currentPolygon.clear(); // Clear actual polygon
        }
        repaint(); // Repaint screan
        return;
    }

    //Add point to polygon

    //Get x, y coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    //set X y to q
    if(add_point)
    {
        q.setX(x);
        q.setY(y);
    }


    //add point to polygon
    else
    {
        QPointF p(x, y);
        currentPolygon.push_back(p);
    }

    //Repaint screen
    repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Draw all stored polygons
    for (const auto& polygon : polygons)
    {
        painter.setPen(Qt::GlobalColor::red);
        painter.setBrush(Qt::GlobalColor::yellow);
        painter.drawPolygon(polygon);
    }

    // Draw selected polygons with swapped colors
    for (const auto& polygon : selectedPolygons)
    {
        painter.setPen(Qt::GlobalColor::yellow);
        painter.setBrush(Qt::GlobalColor::red);
        painter.drawPolygon(polygon);
    }

    // Draw actual polygon
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);
    painter.drawPolygon(currentPolygon);

    //Set graphic attributes, point
    painter.setPen(Qt::GlobalColor::black);
    painter.setBrush(Qt::GlobalColor::blue);
    int r = 5; // velikost bodu v pixelech

    //Begin draw
    painter.drawEllipse(q.x()-r, q.y()-r, 2*r, 2*r);

    //End draw
    painter.end();
}

void Draw::switch_source()
{
    // Save the current polygon to the vector before switching
    if (!currentPolygon.isEmpty())
    {
        polygons.push_back(currentPolygon);
        currentPolygon.clear();
    }

    //input q or polygon vertex
    add_point = !add_point;
}

void Draw::loadPolygonFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open file for reading");
        return;
    }

    QTextStream in(&file);
    // Clearing the canvas before new polygons are loaded
    polygons.clear(); // Clear the existing polygons
    currentPolygon.clear(); // Clear the actual polygon

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.isEmpty())
        {
            // If the line is empty, it indicates the end of a polygon
            if (!currentPolygon.isEmpty())
            {
                polygons.push_back(currentPolygon);
                currentPolygon.clear();
            }
        }
        else
        {
            QStringList coordinates = line.split(",");
            if (coordinates.size() == 2)
            {
                bool ok1, ok2;
                double x = coordinates[0].toDouble(&ok1);
                double y = coordinates[1].toDouble(&ok2);
                if (ok1 && ok2)
                {
                    currentPolygon.append(QPointF(x, y));
                }
            }
        }
    }

    // Add the last polygon if not empty
    if (!currentPolygon.isEmpty())
    {
        polygons.push_back(currentPolygon);
    }

    file.close();
    repaint(); // Repaint the widget to show the new polygons
}

void Draw::clearPolygons()
{
    polygons.clear();
    currentPolygon.clear();
    repaint();
}

void Draw::addSelectedPolygon(const QPolygonF& polygon)
{
    selectedPolygons.push_back(polygon);
    repaint(); // Repaint the widget to show the selected polygons
}

void Draw::clearSelectedPolygons()
{
    selectedPolygons.clear();
    repaint(); // Repaint the widget to clear the selected polygons
}
