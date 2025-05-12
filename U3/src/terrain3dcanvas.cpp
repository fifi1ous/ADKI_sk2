#include "terrain3dcanvas.h"
#include <QtMath>

Terrain3DCanvas::Terrain3DCanvas(QWidget *parent)
    : QWidget(parent), angleX(30), angleY(30), scale(1.0)
{
    setMinimumSize(400, 400);
}

void Terrain3DCanvas::setPoints(const std::vector<QPoint3DF> &pts)
{
    points = pts;
    update();
}

QPoint Terrain3DCanvas::projectPoint(const QPoint3DF &p) const
{
    double radX = qDegreesToRadians(angleX);
    double radY = qDegreesToRadians(angleY);

    double x = p.x();
    double y = p.y() * qCos(radX) - p.getZ() * qSin(radX);
    double z = p.y() * qSin(radX) + p.getZ() * qCos(radX);

    double xFinal = x * qCos(radY) + z * qSin(radY);
    double yFinal = y;

    int centerX = width() / 2;
    int centerY = height() / 2;

    return QPoint(static_cast<int>(centerX + xFinal * scale),
                  static_cast<int>(centerY - yFinal * scale));
}

void Terrain3DCanvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::black);

    for (const QPoint3DF &p : points)
    {
        QPoint proj = projectPoint(p);
        painter.drawEllipse(proj, 2, 2);
    }
}

void Terrain3DCanvas::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
}

void Terrain3DCanvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - lastMousePos;
    angleY += delta.x();
    angleX += delta.y();
    lastMousePos = event->pos();
    update();
}

void Terrain3DCanvas::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        scale *= 1.1;
    else
        scale *= 0.9;

    update();
}
