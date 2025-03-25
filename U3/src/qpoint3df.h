#ifndef QPOINT3DF_H
#define QPOINT3DF_H

#include <QPointF>

class QPoint3DF : public QPointF
{
private:
    double z;
public:
    QPoint3DF() : QPointF(0, 0), z(0) {}
    QPoint3DF(double x, double y, double z)
    {
        QPointF(x, y);
        this->z = z;
    }
    // QPoint3DF(double x_, double y_, double z_): QPointF(x_, y_) z(z_) {}

    inline void setZ(double z){this->z = z;}
    inline double getZ(){return z;}
};

#endif // QPOINT3DF_H
