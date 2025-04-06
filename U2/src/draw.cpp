#include "draw.h"
#include <QMouseEvent>
#include <QtGui>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "../lib/Shapelib/shapefil.h"

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{

}

void Draw::mousePressEvent(QMouseEvent *e)
{
    // Handle double-click to finish polygon creation
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        // If there is building
        if (!building.isEmpty())
        {
            // Add the building to the list of polygons
            polygons.push_back(building);

            // Clear the building for a new building
            building.clear();
        }
        repaint();
        return;
    }
    // Get x, y coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    // Add point to polygon
    // Create point
    QPointF p(x, y);

    // Add point to polygon
    building.push_back(p);

    // Repaint screen
    repaint();
}


void Draw::paintEvent(QPaintEvent *event)
{
    // Draw
    QPainter painter(this);

    // Set graphic attributes of building
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);

    // Draw all stored buildings
    for (const auto& poly : polygons)
    {
        painter.drawPolygon(poly);
    }

    // This part of code was done by chatGPT - used from previous asignment
    // Draw the current polygon being created
    if (!building.isEmpty())
    {
        // Draw lines between points
        painter.setPen(QPen(Qt::blue, 2));  // Blue lines, 2 pixels wide
        for (int i = 0; i < building.size() - 1; ++i)
        {
            painter.drawLine(building[i], building[i + 1]);
        }

        // Draw points
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        int r = 1;  // Point radius
        for (const QPointF& point : building)
        {
            painter.drawEllipse(point.x() - r, point.y() - r, 2*r, 2*r);
        }
    }
    // Here ends the part which was done by chatGPT

    // Set graphics for er
    painter.setPen(Qt::GlobalColor::magenta);
    painter.setBrush(Qt::GlobalColor::transparent);

    // Draw all results
    for (const auto& result : results)
    {
        painter.drawPolygon(result);
    }

    // Draw Convex Hulls (CHs) with dashed lines
    QPen pen(outlineCH, 1.5, Qt::DashLine);
    painter.setPen(pen);
    QBrush brush(fillCH, Qt::SolidPattern);
    painter.setBrush(brush);

    // Draw all CHs
    for (const auto& ch : chs)
    {
        painter.drawPolygon(ch);
    }
}

void Draw::loadPolygonFromTextfile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open the file.");
        return;
    }

    // Clear previous polygons
    clearPolygons();

    QTextStream in(&file);
    // Clear existing polygons before loading new ones
    QVector<QPointF> building;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
        {
            // Process completed polygon when encountering empty line
            if (!building.isEmpty())
            {
                polygons.push_back(building);
                building.clear();
            }
        }
        else
        {
            // Parse coordinates from non-empty lines
            QStringList coordinates = line.split(",");
            if (coordinates.size() == 2)
            {
                bool ok1, ok2;
                double x = coordinates[0].toDouble(&ok1);
                double y = coordinates[1].toDouble(&ok2);

                if (ok1 && ok2)
                {
                    building.append(QPointF(x, y));
                }
            }
        }
    }

    // Process the last polygon if not empt
    if (!building.isEmpty())
    {
        polygons.push_back(building);
    }

    file.close();
    repaint();
}

void Draw::loadPolygonFromShapefile(const QString &fileName)
{
    // Open the shapefile
    SHPHandle hSHP = SHPOpen(fileName.toStdString().c_str(), "rb");
    if (hSHP == nullptr)
    {
        QMessageBox::warning(nullptr, "Error", "Cannot open shapefile.");
        return;
    }

    int nEntities, nShapeType;
    double adfMinBound[4], adfMaxBound[4]; // Bounding box
    SHPGetInfo(hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

    if (nEntities == 0)
    {
        QMessageBox::warning(nullptr, "Warning", "Shapefile contains no data.");
        SHPClose(hSHP);
        return;
    }

    // Clear previous polygons
    clearPolygons();

    // Bounding box (minimum and maximum coordinates)
    double minX = adfMinBound[0];
    double maxX = adfMaxBound[0];
    double minY = adfMinBound[1];
    double maxY = adfMaxBound[1];

    // Get widget dimensions
    double widgetWidth = width();
    double widgetHeight = height();

    // Calculate uniform scaling factor to maintain aspect ratio
    double scale = std::min(widgetWidth / (maxX - minX), widgetHeight / (maxY - minY));

    // Calculate translation offsets to center the polygons
    double offsetX = (widgetWidth - (maxX - minX) * scale) / 2 - minX * scale;
    double offsetY = (widgetHeight - (maxY - minY) * scale) / 2 + maxY * scale;

    for (int i = 0; i < nEntities; ++i)
    {
        SHPObject *psShape = SHPReadObject(hSHP, i);
        if (psShape == nullptr || psShape->nVertices == 0)
        {
            SHPDestroyObject(psShape);
            continue;
        }

        QPolygonF currentPolygon;

        for (int j = 0; j < psShape->nVertices; ++j)
        {
             // Apply uniform scaling and translation
            double x = psShape->padfX[j] * scale + offsetX;
            double y = -psShape->padfY[j] * scale + offsetY; // Invert Y-axis

            // Store the polygon and clear the temporary one
            currentPolygon.append(QPointF(x, y));
        }

        if (!currentPolygon.isEmpty())
        {
            // Close the polygon and add to drawable polygons
            currentPolygon.append(currentPolygon.first());
            polygons.push_back(currentPolygon);
        }

        SHPDestroyObject(psShape);
    }

    SHPClose(hSHP);

    repaint();
}

void Draw::changeColourCHOutline(const bool &status)
{
    //Change Colour based on status
    if(status)
    {
        outlineCH = Qt::darkGreen;
    }
    else
    {
        outlineCH = Qt::transparent;
    }
    repaint();
}

void Draw::changeColourCHFilling(const bool &status)
{
    //Change Colour based on status
    if(status)
    {
        fillCH  = Qt::green;
    }
    else
    {
        fillCH  = Qt::transparent;
    }
    repaint();
}
