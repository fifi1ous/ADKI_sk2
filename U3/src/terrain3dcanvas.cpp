#include "terrain3dcanvas.h"
#include <QtMath>

// Constructor – set default rotation angles and zoom scale
Terrain3DCanvas::Terrain3DCanvas(QWidget *parent)
    : QWidget(parent), angleX(30), angleY(30), scale(1.0)
{
    setMinimumSize(400, 400); // Set minimum window size
}

// Set input 3D points and update the view
void Terrain3DCanvas::setPoints(const std::vector<QPoint3DF> &pts)
{
    points = pts;
    update();
}

// Project 3D point to 2D screen using simple rotation and scale
QPoint Terrain3DCanvas::projectPoint(const QPoint3DF &p) const
{
    double radX = qDegreesToRadians(angleX); // Convert rotation X to radians
    double radY = qDegreesToRadians(angleY); // Convert rotation Y to radians

    // Rotate in X axis
    double x = p.x();
    double y = p.y() * qCos(radX) - p.getZ() * qSin(radX);
    double z = p.y() * qSin(radX) + p.getZ() * qCos(radX);

    // Rotate in Y axis
    double xFinal = x * qCos(radY) + z * qSin(radY);
    double yFinal = y;

    // Center the point on screen
    int centerX = width() / 2;
    int centerY = height() / 2;

    return QPoint(static_cast<int>(centerX + xFinal * scale),
                  static_cast<int>(centerY - yFinal * scale));
}

// Draw the 3D scene
void Terrain3DCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white); // White background
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black); // Draw in black

    // Draw all points as small circles
    for (const QPoint3DF &p : points)
    {
        QPoint proj = projectPoint(p);
        painter.drawEllipse(proj, 2, 2);
    }
}

// Save position when mouse is clicked
void Terrain3DCanvas::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
}

// Rotate scene when mouse is moved
void Terrain3DCanvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - lastMousePos;
    angleY += delta.x(); // Rotate around Y
    angleX += delta.y(); // Rotate around X
    lastMousePos = event->pos();
    update();
}

// Zoom in or out using mouse wheel
void Terrain3DCanvas::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        scale *= 1.1; // Zoom in
    else
        scale *= 0.9; // Zoom out

    update();
}
