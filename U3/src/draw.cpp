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
    //painter.begin(this);

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

    // This part of the code was created with the help of ChatGPT
    // Draw contour lines with labels and halo effect
    if (view_contour_lines)
    {
        double dzValue = 10.0;
        if (settings != nullptr) {
            dzValue = settings->getDz();
        }

        int main_interval = static_cast<int>(dzValue * 5);
        const int minLabelLengthPx = 40;

        QFont font = painter.font();
        font.setPointSize(8);
        painter.setFont(font);

        std::map<int, std::vector<const Edge*>> contoursByZ;
        for (const Edge &e : contour_lines)
        {
            double z_avg = (e.getStart().getZ() + e.getEnd().getZ()) / 2.0;
            int z_level = static_cast<int>(std::round(z_avg));
            contoursByZ[z_level].push_back(&e);

            QPen pen;
            if (z_level % main_interval == 0) {
                pen = QPen(Qt::darkGray, 2);
            } else {
                pen = QPen(Qt::darkGray, 1);
            }

            painter.setPen(pen);
            painter.drawLine(e.getStart(), e.getEnd());
        }

        for (auto it = contoursByZ.begin(); it != contoursByZ.end(); ++it)
        {
            int z_level = it->first;
            const std::vector<const Edge*> &edges = it->second;

            if (z_level % main_interval != 0) continue;

            double totalLength = 0.0;
            std::vector<double> lengths;

            for (const Edge* e : edges)
            {
                double dx = e->getEnd().x() - e->getStart().x();
                double dy = e->getEnd().y() - e->getStart().y();
                double length = std::sqrt(dx * dx + dy * dy);
                lengths.push_back(length);
                totalLength += length;
            }

            double halfLength = totalLength / 2.0;
            double accumulated = 0.0;
            const Edge* bestEdge = nullptr;
            double bestDistance = std::numeric_limits<double>::max();

            for (size_t i = 0; i < edges.size(); ++i)
            {
                double midpoint = accumulated + lengths[i] / 2.0;
                double distanceFromCenter = std::abs(midpoint - halfLength);

                if (lengths[i] >= minLabelLengthPx && distanceFromCenter < bestDistance)
                {
                    bestDistance = distanceFromCenter;
                    bestEdge = edges[i];
                }

                accumulated += lengths[i];
            }

            if (bestEdge == nullptr && !edges.empty()) {
                bestEdge = edges[edges.size() / 2];
            }

            if (bestEdge == nullptr) {
                continue;
            }

            const QPoint3DF &p1 = bestEdge->getStart();
            const QPoint3DF &p2 = bestEdge->getEnd();
            QPointF labelPos((p1.x() + p2.x()) / 2.0, (p1.y() + p2.y()) / 2.0);
            double angle_rad = atan2(p2.y() - p1.y(), p2.x() - p1.x());
            double angle_deg = angle_rad * 180.0 / M_PI;

            // Compute perpendicular vector
            QPointF normal(-(p2.y() - p1.y()), p2.x() - p1.x());
            double normalLength = std::sqrt(normal.x() * normal.x() + normal.y() * normal.y());
            if (normalLength != 0) {
                normal.setX(normal.x() / normalLength);
                normal.setY(normal.y() / normalLength);
            }

            // Sample points on both sides of the contour segment
            QPointF sampleUp = labelPos + normal * 20.0;
            QPointF sampleDown = labelPos - normal * 20.0;

            double zUp = 0.0;
            double zDown = 0.0;
            int upCount = 0;
            int downCount = 0;

            for (auto contourPair : contoursByZ)
            {
                int level = contourPair.first;
                for (const Edge* e : contourPair.second)
                {
                    QPointF a(e->getStart().x(), e->getStart().y());
                    QPointF b(e->getEnd().x(), e->getEnd().y());

                    // Distance from sampleUp to segment ab
                    double dx = b.x() - a.x();
                    double dy = b.y() - a.y();
                    double t = 0.0;
                    if (dx != 0 || dy != 0) {
                        t = ((sampleUp.x() - a.x()) * dx + (sampleUp.y() - a.y()) * dy) / (dx * dx + dy * dy);
                        t = std::max(0.0, std::min(1.0, t));
                    }
                    double projX = a.x() + t * dx;
                    double projY = a.y() + t * dy;
                    double distUp = std::sqrt(std::pow(sampleUp.x() - projX, 2) + std::pow(sampleUp.y() - projY, 2));
                    if (distUp < 10.0) {
                        zUp += level;
                        upCount++;
                    }

                    // Distance from sampleDown to segment ab
                    t = 0.0;
                    if (dx != 0 || dy != 0) {
                        t = ((sampleDown.x() - a.x()) * dx + (sampleDown.y() - a.y()) * dy) / (dx * dx + dy * dy);
                        t = std::max(0.0, std::min(1.0, t));
                    }
                    projX = a.x() + t * dx;
                    projY = a.y() + t * dy;
                    double distDown = std::sqrt(std::pow(sampleDown.x() - projX, 2) + std::pow(sampleDown.y() - projY, 2));
                    if (distDown < 10.0) {
                        zDown += level;
                        downCount++;
                    }
                }
            }

            double avgZUp = z_level;
            double avgZDown = z_level;
            if (upCount > 0) {
                avgZUp = zUp / upCount;
            }
            if (downCount > 0) {
                avgZDown = zDown / downCount;
            }

            // Flip text to point uphill
            if (avgZUp > avgZDown) {
                angle_deg += 180.0;
            }

            // Draw label
            QString z_text = QString::number(z_level);
            painter.save();
            painter.translate(labelPos);
            painter.rotate(angle_deg);

            QFontMetrics fm(painter.font());
            int w = fm.horizontalAdvance(z_text);
            int h = fm.height();
            QPointF textCenter(-w / 2.0, h / 4.0);

            painter.setPen(Qt::white);
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx != 0 || dy != 0) {
                        painter.drawText(textCenter + QPointF(dx, dy), z_text);
                    }
                }
            }

            painter.setPen(Qt::black);
            painter.drawText(textCenter, z_text);
            painter.restore();
        }
    }
    // This part of the code was created with the help of ChatGPT
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
