#include "draw.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

#include "../lib/shapefil.h"

Draw::Draw(QWidget *parent)
    : QWidget{parent}, add_point{false}
{
    // Defaul coordinates of the screen
    q.setX(-5);
    q.setY(-5);
}


void Draw::mousePressEvent(QMouseEvent *e)
{
    // Handle mouse button press events
    if (e->button() == Qt::LeftButton)
    {
        // Check if there's an existing polygon to be stored
        if (currentPolygon.isEmpty() && isPolygonReady)
        {
            // Store the current complex polygon and clear its components
            polygonComplex.push_back(curentCPolygon);
            curentCPolygon.outer.clear();
            curentCPolygon.holes.clear();
            // Store and clear the current drawable polygon
            polygonsWH.push_back(curentPolygonWH);
            curentPolygonWH.clear();
        }

        // Clear any pending hole creation
        currentHole.clear();
        
        // Reset polygon status
        isPolygonReady = false;
        
        // Handle left button press event
        mousePressEventLeft(e);

    }
    if (e->button() == Qt::RightButton)
    {
        // Verify if a polygon is ready for hole creation
        if(!isPolygonReady || !currentPolygon.isEmpty())
        {
            QMessageBox::warning(this, "Information", "Please finish polygon before adding holes");
        }
        else
        {
            // Handle right button press for hole creation
            mousePressEventRight(e);
        }
    }

    // Update display
    repaint();
}
void Draw::mousePressEventLeft(QMouseEvent *e)
{
    // Handle double-click to finish polygon creation
    if (e->type() == QEvent::MouseButtonDblClick)
    {
        if (!currentPolygon.isEmpty())
        {
            // Store the completed polygon
            curentCPolygon.outer = currentPolygon;

            // Close the polygon by adding the first point at the end
            currentPolygon.push_back(currentPolygon.first());
            curentPolygonWH.addPolygon(currentPolygon);

            // Mark polygon as ready for hole creation
            isPolygonReady = true;
            currentPolygon.clear();
        }
        repaint();
        return;
    }

    // Get mouse coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    // Handle point addition based on mode
    if(add_point)
    {
        // Set reference point q
        q.setX(x);
        q.setY(y);
    }
    else
    {
        // Add new point to current polygon
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
            // Store the completed hole
            curentCPolygon.holes.push_back(currentHole);
            
            // Close the hole by adding the first point at the end
            currentHole.push_back(currentHole.first());
            curentPolygonWH.addPolygon(currentHole);
            
            // Clear the current hole
            currentHole.clear();
        }
        repaint();
        return;
    }

    // Add new point to current hole
    double x = e->pos().x();
    double y = e->pos().y();
    QPointF p(x, y);
    currentHole.push_back(p);
}



void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Draw all stored polygons
    for (const QPainterPath& polygon : polygonsWH)
    {
        painter.setPen(Qt::GlobalColor::red);
        painter.setBrush(Qt::GlobalColor::yellow);
        painter.drawPath(polygon);
    }

    // Draw selected polygons with swapped colors
    //qDebug() << "Drawing selected polygons. Count:" << selectedPolygonsWH.size();
    for (const QPainterPath& selectedPolygon : selectedPolygonsWH)
    {
        painter.setPen(Qt::GlobalColor::yellow);
        painter.setBrush(Qt::GlobalColor::red);
        painter.drawPath(selectedPolygon);
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
    // Store any existing polygon before switching input mode
    if (!currentPolygon.isEmpty())
    {
        // Store the polygon in complex polygon structure
        curentCPolygon.outer = currentPolygon;
        polygonComplex.push_back(curentCPolygon);

        // Close the polygon and add to drawable polygons
        currentPolygon.push_back(currentPolygon.first());
        curentPolygonWH.addPolygon(currentPolygon);
        polygonsWH.push_back(curentPolygonWH);

        // Clear all temporary structures
        curentPolygonWH.clear();
        curentCPolygon.outer.clear();
        curentCPolygon.holes.clear();
        currentPolygon.clear();
        repaint();
    }

    // Toggle between point input and polygon vertex input modes
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
    // Clear existing polygons before loading new ones
    clearPolygons();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (line.isEmpty())
        {
            // Process completed polygon when encountering empty line
            if (!currentPolygon.isEmpty())
            {
                // Store the polygon in complex polygon structure
                curentCPolygon.outer = currentPolygon;
                polygonComplex.push_back(curentCPolygon);

                // Close the polygon and add to drawable polygons
                currentPolygon.push_back(currentPolygon.first());
                curentPolygonWH.addPolygon(currentPolygon);
                polygonsWH.push_back(curentPolygonWH);
                
                // Clear all temporary structures
                curentPolygonWH.clear();
                curentCPolygon.outer.clear();
                curentCPolygon.holes.clear();
                currentPolygon.clear();
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
                    currentPolygon.append(QPointF(x, y));
                }
            }
        }
    }

    // Process the last polygon if not empty
    if (!currentPolygon.isEmpty())
    {
        // Store the polygon in complex polygon structure
        curentCPolygon.outer = currentPolygon;
        polygonComplex.push_back(curentCPolygon);

        // Close the polygon and add to drawable polygons
        currentPolygon.push_back(currentPolygon.first());
        curentPolygonWH.addPolygon(currentPolygon);
        polygonsWH.push_back(curentPolygonWH);
        
        // Clear all temporary structures
        curentPolygonWH.clear();
        curentCPolygon.outer.clear();
        curentCPolygon.holes.clear();
        currentPolygon.clear();
    }

    file.close();
    repaint();
}

void Draw::clearPolygons()
{
    // Clear all structures
    currentPolygon.clear();
    curentPolygonWH.clear();
    polygonsWH.clear();
    currentHole.clear();
    polygonComplex.clear();
    curentCPolygon.outer.clear();
    curentCPolygon.holes.clear();
    // Changes the status of polygon
    isPolygonReady = false;

    //Clear point
    q.setX(-5);
    q.setY(-5);

    repaint();
}

void Draw::addSelectedPolygon(const QPainterPath& selection)
{
    // Add selected polygon to vector
    selectedPolygonsWH.push_back(selection);
    repaint();
}

void Draw::clearSelectedPolygons()
{
    selectedPolygonsWH.clear();
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
    double offsetX = (widgetWidth  - (maxX - minX) * scale) / 2 - minX * scale;
    double offsetY = (widgetHeight - (maxY - minY) * scale) / 2 + maxY * scale; // Invert Y-axis

    for (int i = 0; i < nEntities; ++i)
    {
        SHPObject *psShape = SHPReadObject(hSHP, i);
        if (psShape == nullptr)
        {
            continue;
        }

        //currentPolygon.clear();

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
        curentCPolygon.outer = currentPolygon;
        polygonComplex.push_back(curentCPolygon);

        // Close the polygon and add to drawable polygons
        currentPolygon.push_back(currentPolygon.first());
        curentPolygonWH.addPolygon(currentPolygon);
        polygonsWH.push_back(curentPolygonWH);

        // Clear all temporary structures
        curentPolygonWH.clear();
        curentCPolygon.outer.clear();
        curentCPolygon.holes.clear();
        currentPolygon.clear();

        SHPDestroyObject(psShape);
    }

    SHPClose(hSHP);

    isShapefileLoaded = true;

    repaint();
}
