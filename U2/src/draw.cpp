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

    //Set graphics for er
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
    painter.drawPolygon(er);

    painter.end();
}

void Draw::loadPolygonFromTextfile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open the file.");
        return;
    }

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

void Draw::clearPolygons()
{
    building.clear();
    polygons.clear();
    repaint();
}

void Draw::clearResults()
{
    results.clear();
    repaint();
}
