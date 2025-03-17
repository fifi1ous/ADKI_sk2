#include "algorithms.h"
#include "sortpointsbyx.h"
#include "sortpointsbyy.h"
#include <cmath>

Algorithms::Algorithms() {}

double Algorithms::get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    //Compute angle between two lines
    double u_x = p2.x() - p1.x();
    double u_y = p2.y() - p1.y();

    double v_x = p4.x() - p3.x();
    double v_y = p4.y() - p3.y();

    //Dot product
    double dot = u_x*v_x + u_y*v_y;

    //Calculate norms
    double n_u = std::sqrt(u_x*u_x + u_y*u_y);
    double n_v = std::sqrt(v_x*v_x + v_y*v_y);

    return acos(dot/(n_u*n_v));
}


QPolygonF Algorithms::createCH(const QPolygonF &pol)
{
    //Create convex hull using Jarvis scan
    QPolygonF ch;

    //Get pivot q
    QPointF q = *std::min_element(pol.begin(), pol.end(), sortPointsByY());

    //Get point
    QPointF r = *std::min_element(pol.begin(), pol.end(), sortPointsByX());

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

    }while( pj !=q );

    return ch;
}


std::tuple<QPolygonF, double> Algorithms::minMaxBox(const QPolygonF &pol)
{
    //Create min max box
    double area;

    //Get extreme points
    QPointF qymin = *std::min_element(pol.begin(), pol.end(), sortPointsByY());
    QPointF qymax = *std::max_element(pol.begin(), pol.end(), sortPointsByY());

    QPointF qxmin = *std::min_element(pol.begin(), pol.end(), sortPointsByX());
    QPointF qxmax = *std::max_element(pol.begin(), pol.end(), sortPointsByX());

    //Get extreme coordinates
    double xmin = qxmin.x();
    double ymin = qymin.y();

    double xmax = qxmax.x();
    double ymax = qymax.y();

    //Create vetices of min max box
    QPointF v1(xmin,ymin);
    QPointF v2(xmax,ymin);
    QPointF v3(xmax,ymax);
    QPointF v4(xmin,ymax);

    //Create new polygon
    QPolygonF pol_new = {v1, v1, v3, v4};

    //Compute area
    area = (xmax - xmin) * (ymax - ymin);

    return {pol_new, area};
}


QPolygonF Algorithms::rotate(const QPolygonF &pol, double sigma)
{
    // rotate polygon by angle sigma
    QPolygonF pol_rotated;

    // Proces polygon one by one
    for(QPointF point: pol)
    {
        //Get coordinates
        double xp = point.x();
        double yp = point.y();

        //Rotate point
        double xp_rot = xp*cos(sigma) - yp*sin(sigma);
        double yp_rot = xp*sin(sigma) + yp*cos(sigma);

        //Create new point
        QPointF p(xp_rot, yp_rot);

        //Add to the vecotor
        pol_rotated.push_back(p);

    }

    return pol_rotated;
}


static double getArea(const QPolygonF &pol)
{
    //Compute area using LH formula

    int n = pol.size();
    double area = 0;

    // Proces point one by one
    for(int i = 0;i<n;i++)
    {
        area += pol[i].x() * (pol[(i+1)%n].y()-pol[(i-1+n)%n].y());
    }

    return fabs(area/2);
}


static QPolygonF resize(const QPolygonF &pol, const QPolygonF &mmbox)
{
    //Resize minimum area enclosing rectangle in order to have the same area as a building
    double Ab = getArea(pol);
    double A = getArea(mmbox);

    //Compute kj
    double k = Ab/A;

    //Compute center of gravity
    double xt = (mmbox[0].x() + mmbox[1].x() + mmbox[2].x() + mmbox[3].x())/4;
    double yt = (mmbox[0].y() + mmbox[1].y() + mmbox[2].y() + mmbox[3].y())/4;

    //Create vectors u1-u4
    double u1x = mmbox[0].x() - xt;
    double u1y = mmbox[0].y() - yt;
    double u2x = mmbox[1].x() - xt;
    double u2y = mmbox[1].y() - yt;
    double u3x = mmbox[2].x() - xt;
    double u3y = mmbox[2].y() - yt;
    double u4x = mmbox[3].x() - xt;
    double u4y = mmbox[3].y() - yt;

    // Compute vertices of the resized rectangle
    double x1r = xt + sqrt(k)*u1x;
    double y1r = yt + sqrt(k)*u1y;

    double x2r = xt + sqrt(k)*u2x;
    double y2r = yt + sqrt(k)*u2y;

    double x3r = xt + sqrt(k)*u3x;
    double y3r = yt + sqrt(k)*u3y;

    double x4r = xt + sqrt(k)*u4x;
    double y4r = yt + sqrt(k)*u4y;

    //Create new points
    QPointF p1(x1r,y1r);
    QPointF p2(x2r,y2r);
    QPointF p3(x3r,y3r);
    QPointF p4(x4r,y4r);

    //Create resized mmbox
    QPolygonF pol_res = {p1, p2, p3, p4};

    return pol_res;
}


QPolygonF Algorithms::getMAER(const QPolygonF &pol)
{
    //Create minimun area enclosing rectangle over the building
    auto [maer, area_min] = minMaxBox(pol);

    //Create CH
    QPolygonF ch = createCH(pol);

    //Process all segments of CG
    int n = ch.size();
    for(int i = 0;i<n;i++)
    {
        //Get ch segment and its coordinate differencies
        double dx = ch[(i+1)%n].x()-ch[i+1].x();
        double dy = ch[(i+1)%n].y()-ch[i+1].y();

        //Get angle of rotation
        double sigma = atan2(dy, dx);

        //Rotation by -sigma
        QPolygonF ch_rot = rotate(ch, -sigma);

        //Compute min-max box
        auto [mmbox, area] = minMaxBox(ch_rot);

        //Update minimum
        if(area < area_min)
        {
            maer = mmbox;
        }

    }


}









