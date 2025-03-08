#include "draw.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <ogrsf_frmts.h>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{
    q.setX(0);
    q.setY(0);
    add_point = false;
    isShapefileLoaded = false;
}


void Draw::mousePressEvent(QMouseEvent *e)
{
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
        //Create point
        QPointF p(x, y);

        //Add point to polygon
        pol.push_back(p);
    }

    //Repaint screen
    repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    // Draw
    QPainter painter(this);
    painter.begin(this);

    // Set graphic attributes for polygon
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);

    if (isShapefileLoaded)
    {
        // Get bounding box of polygon
        QRectF boundingRect = pol.boundingRect();

        // Scale calculation
        qreal scaleX = width() / boundingRect.width();
        qreal scaleY = height() / boundingRect.height();

        // Use the smaller scaling factor to preserve aspect ratio
        qreal scale = qMin(scaleX, scaleY);

        // Calculate offset to center the polygon
        qreal offsetX = (width() - boundingRect.width() * scale) / 2;
        qreal offsetY = (height() - boundingRect.height() * scale) / 2;

        // Apply transformations
        painter.translate(offsetX, offsetY);
        painter.scale(scale, scale);
        painter.translate(-boundingRect.topLeft());
    }

    // Begin drawing the polygon
    painter.drawPolygon(pol);

    // Set graphic attributes for point
    painter.setPen(Qt::GlobalColor::black);
    painter.setBrush(Qt::GlobalColor::blue);
    int r = 5; // size of point in pixels

    // Begin drawing the point
    painter.drawEllipse(q.x() - r, q.y() - r, 2 * r, 2 * r);

    // End drawing
    painter.end();
}


void Draw::switch_source()
{
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
    pol.clear(); // Clear the existing polygon

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList coordinates = line.split(",");
        if (coordinates.size() == 2)
        {
            bool ok1, ok2;
            double x = coordinates[0].toDouble(&ok1);
            double y = coordinates[1].toDouble(&ok2);
            if (ok1 && ok2)
            {
                pol.append(QPointF(x, y));
            }
        }
    }

    file.close();
    repaint(); // Repaint the widget to show the new polygon
}

void Draw::loadPolygonFromShapefile(const QString &fileName)
{
    // Initialize GDAL/OGR
    GDALAllRegister();

    // Open shapefile
    GDALDataset *poDS = (GDALDataset *)GDALOpenEx(fileName.toUtf8().constData(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
    if (poDS == nullptr)
    {
        QMessageBox::warning(this, "Error", "Cannot open shapefile.");
        return;
    }

    // Get the first layer – assuming the shapefile has only one layer
    OGRLayer *poLayer = poDS->GetLayer(0);
    if (poLayer == nullptr)
    {
        QMessageBox::warning(this, "Error", "Cannot load layer from shapefile.");
        GDALClose(poDS);
        return;
    }

    // Clear previous polygon
    pol.clear();

    // Loop through all features in the layer
    OGRFeature *poFeature = nullptr;
    poLayer->ResetReading();
    while ((poFeature = poLayer->GetNextFeature()) != nullptr)
    {
        OGRGeometry *poGeometry = poFeature->GetGeometryRef();
        if (poGeometry != nullptr && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon)
        {
            // Assuming we are working with a polygon
            OGRPolygon *poPolygon = dynamic_cast<OGRPolygon *>(poGeometry);
            if (poPolygon)
            {
                // Load the exterior ring (first linear ring)
                OGRLinearRing *poRing = poPolygon->getExteriorRing();
                if (poRing)
                {
                    int numPoints = poRing->getNumPoints();
                    for (int i = 0; i < numPoints; i++)
                    {
                        double x = poRing->getX(i);
                        double y = poRing->getY(i);
                        pol.append(QPointF(x, y));
                    }
                }
            }
        }
        OGRFeature::DestroyFeature(poFeature);
    }

    // Close dataset
    GDALClose(poDS);

    // Set flag to indicate shapefile was loaded
    isShapefileLoaded = true;

    // Repaint the widget to show the new polygon
    repaint();
}

