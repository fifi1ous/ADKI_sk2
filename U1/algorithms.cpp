#include "algorithms.h"
#include <cmath>

short Algorithms::analyzeRayCrossing(const QPointF &q, const QPolygonF &pol)
{
    int k = 0; // Number of intersections polygon and ray edges
    int n = pol.size(); // Number of vertices in the polygon

    for(int i = 0; i < n ; i++)
    {
        // Get the index of the next vertex
        int ii = (i + 1) % n;

        // Get coordinates relative to q
        double xir = pol[i].x() - q.x();
        double yir = pol[i].y() - q.y();
        double xiir = pol[ii].x() - q.x();
        double yiir = pol[ii].y() - q.y();

        // Check if the ray intersects the edge
        if(((yiir > 0) && (yir <= 0)) || ((yir > 0) && (yiir <= 0)))
        {
            double xm = (xiir*yir-xir*yiir)/(yiir-yir);
            if(xm>0)
                k++;
        }

        // Check for singularities
        short singularityCheck = checkSingularities(q, pol, i, ii);
        if(singularityCheck != -1)
        {
            return singularityCheck;
        }
    }
    return k % 2;
}

short Algorithms::analyzeWindingNumber(const QPointF &q, const QPolygonF &pol)
{
    double omega = 0.0; // Initialize winding number
    int n = pol.size(); // Number of vertices in the polygon

    for (int i = 0; i < n; i++)
    {
        // Get the index of the next vertex
        int ii = (i + 1) % n;

        // Calculate distances from point q to vertices pi and pii, and between vertices pi and pii
        double distance_q_pi = calculateDistance(pol[i], q);
        double distance_q_pii = calculateDistance(pol[ii], q);
        double distance_pi_pii = calculateDistance(pol[i], pol[ii]);

        // Calculate the cosine of the angle using the cosine rule
        double cosineValue = calculateCosineValue(distance_q_pi, distance_q_pii, distance_pi_pii);

        // Calculate the angle in radians
        double angle = std::acos(cosineValue);

        // Calculate the determinant to determine the orientation
        double determinant = calculateDeterminant(pol[i], pol[ii], q);

        // Update the winding number based on the orientation
        if (determinant > 0)
        {
            omega += angle; // Point is in the left half-plane
        }
        else if (determinant < 0)
        {
            omega -= angle; // Point is in the right half-plane
        }

        // Check for singularities
        short singularityCheck = checkSingularities(q, pol, i, ii);
        if(singularityCheck != -1)
        {
            return singularityCheck;
        }
    }

    if (std::fabs(std::fabs(omega) - 2 * M_PI) < epsilon) // fabs: absolute value
    {
        return 1; // Point is inside the polygon
    }

    return 0; // Point is outside the polygon
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

short Algorithms::checkSingularities(const QPointF &q, const QPolygonF &pol, int i, int ii)
{
    // Check if point q is exactly on the vertex
    double xir = pol[i].x() - q.x();
    double yir = pol[i].y() - q.y();
    double xiir = pol[ii].x() - q.x();
    double yiir = pol[ii].y() - q.y();

    if((std::fabs(xir) <= epsilon && std::fabs(yir) <= epsilon) || (std::fabs(xiir) <= epsilon && std::fabs(yiir) <= epsilon))
    {
        return 3; // Point is on the vertex
    }

    // Check if point q is exactly on the edge using distances
    double distance_i_ii = calculateDistance(pol[i], pol[ii]);
    double distance_i_q = calculateDistance(pol[i], q);
    double distance_ii_q = calculateDistance(pol[ii], q);

    if(std::fabs(distance_i_q + distance_ii_q - distance_i_ii) < epsilon)
    {
        return 2; // Point is on the edge
    }

    return -1; // No singularity
}
