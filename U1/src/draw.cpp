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
    q.setX(-5);
    q.setY(-5);
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if (!curentPolygonWH.isEmpty())
        {
            polygonComplex.push_back(curentCPolygon);
            curentCPolygon.outer.clear();
            curentCPolygon.holes.clear();
        }

        polygonsWH.push_back(curentPolygonWH);
        curentPolygonWH.clear();
        currentHole.clear();
        mousePressEventLeft(e);

    }else if (e->button() == Qt::RightButton)
    {
        if(!isPolygonReady || !currentPolygon.isEmpty())
        {
            QMessageBox::warning(this, "Information", "Please finish polygon before adding holes");

        }
        else
        {
            mousePressEventRight(e);
        }
    }

    //Repaint screen
    repaint();
}
void Draw::mousePressEventLeft(QMouseEvent *e)
{

    // Cretes new polygon wh
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        if (!currentPolygon.isEmpty())
        {
            polygons.push_back(currentPolygon); // Add actual polygon to the vector
            curentCPolygon.outer = currentPolygon;

            currentPolygon.push_back(currentPolygon.first());
            curentPolygonWH.addPolygon(currentPolygon);
            index++;
            isPolygonReady = true;
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
}
void Draw::mousePressEventRight(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        if (!currentHole.isEmpty())
        {
            //holes.push_back(currentHole); // Add actual polygon to the vector
            curentCPolygon.holes.push_back(currentHole);
            currentHole.push_back(currentHole.first());
            curentPolygonWH.addPolygon(currentHole);
            currentHole.clear();          // Clear actual polygon
        }
        repaint(); // Repaint screan
        return;
    }

    double x = e->pos().x();
    double y = e->pos().y();

    QPointF p(x, y);
    currentHole.push_back(p);
}



void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Draw all stored polygons

    for (const auto& polygon : polygonsWH)
    {
        painter.setPen(Qt::GlobalColor::red);
        painter.setBrush(Qt::GlobalColor::yellow);
        painter.drawPath(polygon);
    }

    // Draw selected polygons with swapped colors
    for (const auto& polygon : selectedPolygons)
    {
        painter.setPen(Qt::GlobalColor::yellow);
        painter.setBrush(Qt::GlobalColor::red);
        painter.drawPolygon(polygon);
    }

    // Draw actual polygon and its holes
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);
    painter.drawPath(curentPolygonWH);

    // Draw the current polygon being created
    if (!currentPolygon.isEmpty())
    {
        // Draw lines between points
        painter.setPen(QPen(Qt::blue, 2));  // Blue lines, 2 pixels wide
        for (int i = 0; i < currentPolygon.size() - 1; ++i)
        {
            painter.drawLine(currentPolygon[i], currentPolygon[i + 1]);
        }

        // Draw points
        painter.setPen(Qt::blue);
        painter.setBrush(Qt::blue);
        int r = 2;  // Point radius
        for (const QPointF& point : currentPolygon)
        {
            painter.drawEllipse(point.x() - r, point.y() - r, 2*r, 2*r);
        }
    }

    // Draw the current hole being created
    if (!currentHole.isEmpty())
    {
        // Draw lines between points for the hole
        painter.setPen(QPen(Qt::green, 2));  // Green lines, 2 pixels wide
        for (int i = 0; i < currentHole.size() - 1; ++i)
        {
            painter.drawLine(currentHole[i], currentHole[i + 1]);
        }

        // Draw points for the hole
        painter.setPen(Qt::green);
        painter.setBrush(QColor(Qt::green));
        int r = 2;  // Point radius
        for (const QPointF& point : currentHole)
        {
            painter.drawEllipse(point.x() - r, point.y() - r, 2*r, 2*r);
        }
    }


    // Draw the q point
    painter.setPen(Qt::black);
    painter.setBrush(Qt::blue);
    int r = 5;
    painter.drawEllipse(q.x()-r, q.y()-r, 2*r, 2*r);

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
    curentPolygonWH.clear();
    polygonsWH.clear();
    currentHole.clear();
    polygonComplex.clear();
    curentCPolygon.outer.clear();
    curentCPolygon.holes.clear();
    index = 0;
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

void Draw::loadPolygonFromShapefile(const QString &fileName)
{

    // Open the shapefile
    SHPHandle hSHP = SHPOpen(fileName.toStdString().c_str(), "rb");
    if (hSHP == nullptr)
    {
        QMessageBox::warning(this, "Error", "Cannot open shapefile.");
        return;
    }

    int nEntities, nShapeType;
    double adfMinBound[4], adfMaxBound[4]; // Bounding box
    SHPGetInfo(hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound);

    if (nEntities == 0)
    {
        QMessageBox::warning(this, "Warning", "Shapefile contains no data.");
        SHPClose(hSHP);
        return;
    }

    // Clear previous polygons
    polygons.clear();
    currentPolygon.clear();

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
    double offsetY = (widgetHeight - (maxY - minY) * scale) / 2 + maxY * scale; // Invert Y-axis

    for (int i = 0; i < nEntities; ++i)
    {
        SHPObject *psShape = SHPReadObject(hSHP, i);
        if (psShape == nullptr)
        {
            continue;
        }

        currentPolygon.clear();

        if (psShape->nVertices == 0)
        {
            SHPDestroyObject(psShape);
            continue;
        }

        for (int j = 0; j < psShape->nVertices; ++j)
        {
            double x = psShape->padfX[j];
            double y = psShape->padfY[j];

            // Apply uniform scaling and translation
            double transformedX = x * scale + offsetX;
            double transformedY = -y * scale + offsetY; // Invert Y-axis

            currentPolygon.append(QPointF(transformedX, transformedY));
        }

        // Store the polygon and clear the temporary one
        polygons.push_back(currentPolygon);
        currentPolygon.clear();

        SHPDestroyObject(psShape);
    }

    SHPClose(hSHP);

    isShapefileLoaded = true;

    repaint();
}
