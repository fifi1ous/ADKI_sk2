#include "sortpointsbyx.h"

sortPointsByX::sortPointsByX() {}

bool sortPointsByX:: operator() (const QPointF &p1, const QPointF &p2)
{
    return p1.x() < p2.x();
}

