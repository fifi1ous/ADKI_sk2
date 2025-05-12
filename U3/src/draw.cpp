#include "draw.h"
#include <QtGui>
#include <time.h>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
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

void Draw::loadPointsFromTextfile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open the file.");
        return;
    }

    QTextStream in(&file);
    // Clear existing points before loading new ones
    points.clear();

    // Variables to track min and max values
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double minZ = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();
    double maxZ = std::numeric_limits<double>::lowest();

    // Read points from the file and calculate min/max values
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
        {
            continue; // skip empty lines
        }

        // Split the line by commas and spaces
        QStringList coordinates = line.split(",");
        for (int i = 0; i < coordinates.size(); ++i)
        {
            coordinates[i] = coordinates[i].trimmed();
        }

        QStringList finalCoordinates;
        for (const QString &coord : coordinates)
        {
            if (!coord.isEmpty())
            {
                QStringList temp = coord.split(" ");
                for (const QString &subCoord : temp)
                {
                    if (!subCoord.isEmpty())
                    {
                        finalCoordinates.append(subCoord);
                    }
                }
            }
        }

        if (finalCoordinates.size() == 3)  // We expect 3 values per line (x, y, z)
        {
            bool ok1, ok2, ok3;
            double x = finalCoordinates[0].toDouble(&ok1);
            double y = finalCoordinates[1].toDouble(&ok2);
            double z = finalCoordinates[2].toDouble(&ok3);

            if (ok1 && ok2 && ok3) // Ensure all values are valid numbers
            {
                // Update min/max values for scaling
                minX = std::min(minX, x);
                minY = std::min(minY, y);
                minZ = std::min(minZ, z);
                maxX = std::max(maxX, x);
                maxY = std::max(maxY, y);
                maxZ = std::max(maxZ, z);

                QPoint3DF point(x, y, z); // Create QPoint3DF object with x, y, and z values
                points.push_back(point);   // Add the point to the points vector
            }
            else
            {
                QMessageBox::warning(this, "Error", "Invalid coordinates in file.");
                continue; // Skip this line if parsing failed
            }
        }
        else
        {
            QMessageBox::warning(this, "Error", "Each line must contain exactly 3 coordinates (x, y, z).");
            continue;
        }
    }

    // Calculate scaling and translation to fit the points in the window
    if (points.empty()) return;

    // Get the size of the window (or the visible area)
    int windowWidth = width();
    int windowHeight = height();

    // Calculate the scaling factors
    double scaleX = windowWidth / (maxX - minX);
    double scaleY = windowHeight / (maxY - minY);

    // Determine the translation to center the points on the screen
    double offsetX = -minX;
    double offsetY = -minY;

    // Apply transformation (scale and translate) to each point
    for (auto &point : points)
    {
        double newX = (point.x() + offsetX) * scaleX;
        double newY = (point.y() + offsetY) * scaleY;
        point.setX(newX);
        point.setY(newY);
    }

    file.close();  // Close the file after reading
    repaint();     // Repaint the widget to show loaded points
}