#include "sortpointsbyy.h"

sortPointsByY::sortPointsByY() {}

bool sortPointsByY:: operator() (const QPointF &p1, const QPointF &p2)
{
    return p1.y() < p2.y();
}
