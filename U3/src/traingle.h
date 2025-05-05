#ifndef TRAINGLE_H
#define TRAINGLE_H

#include "qpoint3df.h"

class Traingle
{
private:
    QPoint3DF p1, p2, p3;
    double aspect, slope;
public:
    Traingle();

    Traingle(QPoint3DF p1_, QPoint3DF p2_, QPoint3DF p3_, double aspect_, double slope_)
    {
        p1 = p1_;
        p2 = p2_;
        p3 = p3_;
        slope = slope_;
        aspect = aspect_;
    }
};

#endif // TRAINGLE_H
