#include "algorithms.h"

Algorithms::Algorithms()
{}
    short Algorithms::analyzePointAndPolygonPosition(const QPointF &q, const QPolygonF &pol)
    {
        // Number of intersections polygon and ray edges
        int k = 0;
        int n = pol.size();

        for(int i = 0; i < n ; i++)
        {
            //get coordinates

            double xir = pol[i].x() - q.x();
            double yir = pol[i].y() - q.y();

            double xi1r = pol[(i+1)%n].x() - q.x();
            double yi1r = pol[(i+1)%n].y() - q.y();

            if((yi1r > 0) && (yir <= 0) || (yir > 0)&& (yi1r <= 0))
            {
                double xm = (xi1r*yir-xir*yi1r)/(yi1r-yir);
                if(xm>0)
                    k++;
            }
        }
    return k % 2;
    }
