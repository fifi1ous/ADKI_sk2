#include "algorithms.h"
#include "sortpointsbyx.h"
#include "sortpointsbyy.h"
#include <cmath>

Algorithms::Algorithms() {}

double Algorithms::get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    //Compute angle between two lines
    double u_x  = p2.x() - p1.x();
    double u_y  = p2.y() - p1.y();

    double v_x  = p4.x() - p3.x();
    double v_y  = p4.y() - p3.y();

    //Dot product
    double dot = u_x*v_x + u_y*v_y;

    // Calculate norms
    double n_u = std::sqrt(u_x*u_x + u_y*u_y);
    double n_v = std::sqrt(v_x*v_x + v_y*v_y);

    return acos(dot/(n_u*n_v));
}

QPolygonF Algorithms::createCH(const QPolygonF &pol)
{
    //Create convex hull using Jarvis scan
    QPolygonF ch;

    //Get pivot q
    auto q = *std::min_element(pol.begin(), pol.end(), sortPointsByY());

    //Get point
    auto r = *std::min_element(pol.begin(), pol.end(), sortPointsByX());

    //Initliaze pj, pj1
    QPointF pj = q;
    QPointF pj1(r.x(),q.y());

    //Add first point to ch
    ch.push_back(pj);

    //Find all points of ch
    do
    {
        // Maximum and index
        double omega_max = 0;
        int i_max = -1;

        //Find point generating max angle
        for(int i=0; i<pol.size(); i++)
        {
            //Find point generating maximum angle
            double omega = get2LinesAngle(pj, pj1, pj, pol[i]);

            //Update maximum
            if(omega>omega_max)
            {
                omega_max = omega;
                i_max = i;
            }
        }
        //Add point to ch
        ch.push_back(pol[i_max]);

        //Update vertices
        pj1 = pj;
        pj = pol[i_max];

    }while(pj !=q );

    return ch;
}
