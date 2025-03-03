#include "algorithms.h"

short Algorithms::analyzeRayCrossing(const QPointF &q, const QPolygonF &pol)
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

short Algorithms::analyzeWindingNumber(const QPointF &q, const QPolygonF &pol)
{

}

double Algorithms::calculateDistance(const QPointF &p1, const QPointF &p2)
{
    double dx = p1.x() - p2.x(); // Calculate difference of x coordinates
    double dy = p1.y() - p2.y(); // Calculate difference of y coordinates
    return std::sqrt(dx*dx + dy*dy); // Calculate and return Euclidean distance
}

double Algorithms::calculateCosineValue(double a, double b, double c)
{
    // Calculate and return cosine of the angle (gamma) between sides a and b using the cosine rule
    return (a*a + b*b - c*c) / (2 * a * b);
}

double Algorithms::calculateDeterminant(const QPointF &p1, const QPointF &p2, const QPointF &q)
{
    // Calculate differences in coordinates
    double dx1 = p2.x() - p1.x();
    double dy1 = p2.y() - p1.y();
    double dx2 = q.x() - p1.x();
    double dy2 = q.y() - p1.y();

    // Calculate and return the determinant
    return (dx1 * dy2) - (dy1 * dx2);
}
