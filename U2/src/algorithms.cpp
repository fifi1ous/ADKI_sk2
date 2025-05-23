#include "algorithms.h"
#include <cmath>
#include "sortpointsbyx.h"
#include "sortpointsbyy.h"


double Algorithms::get2LinesAngle(const QPointF &p1, const QPointF &p2, const QPointF &p3, const QPointF &p4)
{
    // Compute angle between two lines
    double u_x = p2.x() - p1.x();
    double u_y = p2.y() - p1.y();

    double v_x = p4.x() - p3.x();
    double v_y = p4.y() - p3.y();

    // Dot product
    double dot = u_x*v_x + u_y*v_y;

    // Calculate norms
    double n_u = std::sqrt(u_x*u_x + u_y*u_y);
    double n_v = std::sqrt(v_x*v_x + v_y*v_y);

    return acos(dot/(n_u*n_v));
}

std::tuple<QPolygonF, double> Algorithms::minMaxBox(const QPolygonF &pol)
{
    // Create min max box
    double area;

    // Get extreme points
    QPointF qymin = *std::min_element(pol.begin(), pol.end(), sortPointsByY());
    QPointF qymax = *std::max_element(pol.begin(), pol.end(), sortPointsByY());

    QPointF qxmin = *std::min_element(pol.begin(), pol.end(), sortPointsByX());
    QPointF qxmax = *std::max_element(pol.begin(), pol.end(), sortPointsByX());

    // Get extreme coordinates
    double xmin = qxmin.x();
    double ymin = qymin.y();

    double xmax = qxmax.x();
    double ymax = qymax.y();

    // Create vetices of min max box
    QPointF v1(xmin,ymin);
    QPointF v2(xmax,ymin);
    QPointF v3(xmax,ymax);
    QPointF v4(xmin,ymax);

    // Create new polygon
    QPolygonF pol_new = {v1, v2, v3, v4};

    // Compute area
    area = (xmax - xmin) * (ymax - ymin);

    return {pol_new, area};
}


QPolygonF Algorithms::rotate(const QPolygonF &pol, double sigma)
{
    // Rotate polygon by angle sigma
    QPolygonF pol_rotated;

    // Proces polygon one by one
    for(QPointF point: pol)
    {
        // Get coordinates
        double xp = point.x();
        double yp = point.y();

        // Rotate point
        double xp_rot = xp*cos(sigma) - yp*sin(sigma);
        double yp_rot = xp*sin(sigma) + yp*cos(sigma);

        // Create new point
        QPointF p(xp_rot, yp_rot);

        // Add to the vecotor
        pol_rotated.push_back(p);

    }

    return pol_rotated;
}


double Algorithms::getArea(const QPolygonF &pol)
{
    // Compute area using LH formula

    int n = pol.size();
    double area = 0;

    // Proces points one by one
    for(int i = 0;i<n;i++)
    {
        area += pol[i].x() * (pol[(i+1)%n].y()-pol[(i-1+n)%n].y());
    }

    return fabs(area/2);
}


QPolygonF Algorithms::resize(const QPolygonF &pol, const QPolygonF &mmbox)
{
    // Resize minimum area enclosing rectangle in order to have the same area as a building
    double Ab = getArea(pol);
    double A = getArea(mmbox);

    // Compute kj
    double k = Ab/A;

    // Compute center of gravity
    double xt = (mmbox[0].x() + mmbox[1].x() + mmbox[2].x() + mmbox[3].x())/4;
    double yt = (mmbox[0].y() + mmbox[1].y() + mmbox[2].y() + mmbox[3].y())/4;

    // Create vectors u1-u4
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

    // Create new points
    QPointF p1(x1r,y1r);
    QPointF p2(x2r,y2r);
    QPointF p3(x3r,y3r);
    QPointF p4(x4r,y4r);

    // Create resized mmbox
    QPolygonF pol_res = {p1, p2, p3, p4};

    return pol_res;
}


QPolygonF Algorithms::createCHJS(const QPolygonF &pol)
{
    // Create convex hull using Jarvis scan
    QPolygonF ch;

    // If the polygon has only 3 vertexes, return polygon as CH
    if (pol.size() == 3)
        return pol;

    // Get pivot q
    QPointF q = *std::min_element(pol.begin(), pol.end(), sortPointsByY());

    // Get point
    QPointF r = *std::min_element(pol.begin(), pol.end(), sortPointsByX());

    // Initliaze pj, pj1
    QPointF pj = q;
    QPointF pj1(r.x() - 1, q.y());

    // Add first point to ch
    ch.push_back(pj);

    // Find all points of ch
    do
    {
        // Maximum and index
        double omega_max = 0;
        int i_max = -1;

        // Find point generating max angle
        for(int i=0; i<pol.size(); i++)
        {
            // Find point generating maximum angle
            double omega = get2LinesAngle(pj, pj1, pj, pol[i]);

            // Update maximum
            if(omega>omega_max)
            {
                omega_max = omega;
                i_max = i;
            }
        }

        // Add point to ch
        ch.push_back(pol[i_max]);

        // Update vertices
        pj1 = pj;
        pj = pol[i_max];

    }while( pj !=q );


    return ch;
}

double Algorithms::getDistance(const QPointF &p1, const QPointF &p2)
{
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();

    return sqrt(dx*dx+dy*dy);
}


QPointF Algorithms::findPivotGS(const QPolygonF &pol)
{
    // Find the pivot point for the Graham scan algorithm

    // Initialize the pivot as the first point in the polygon
    QPointF q = pol[0];
    double q_y = q.y();

    // Iterate through all points to find the one with the lowest Y-coordinate
    for(int i = 1; i < pol.size(); i++)
    {
        double c_y = pol[i].y(); // Y-coordinate of the current candidate point

        // If the candidate has a lower or equal Y-coordinate than the current pivot
        if(c_y <= q_y)
        {
            // If Y-coordinates are equal, compare X-coordinates
            if(c_y == q_y)
            {
                double c_x = pol[i].x();
                double q_x = q.x();

                // Choose the point with the smaller X-coordinate (more to the left)
                if(c_x < q_x)
                {
                    q = pol[i];
                    q_y = q.y();
                }
            }
            else
            {
                // Candidate has a lower Y-coordinate, update the pivot
                q = pol[i];
                q_y = q.y();
            }
        }
    }

    return q;
}

std::vector<double> Algorithms::anglesWithPoints(const QPolygonF &pol, const QPointF &q)
{
    // Vector of angles
    std::vector<double> angles;

    // Generate x line
    QPointF x1 = q;
    QPointF x2(x1.x()+1,x1.y());

    // Calculate angles
    for(QPointF point: pol)
    {
        // Calculate angles
        double angle = get2LinesAngle(x1,x2,q,point);
        angles.push_back(angle);
    }
    return angles;
}

void Algorithms::sortAnglesPoints(const QPointF &q, std::vector<double> &angles, QPolygonF &pol_)
{
    // Sort the angles and corresponding points
    std::vector<size_t> indices(angles.size());
    std::iota(indices.begin(), indices.end(), 0);

    // Sort indices based on angles
    // This part of code was done by chatGPT
    std::sort(indices.begin(), indices.end(), [&angles](size_t i1, size_t i2) { return angles[i1] < angles[i2]; });
    // Here ends the part which was done by chatGPT

    // Create sorted and filtered vectors
    std::vector<double> sorted_angles;
    QPolygonF sorted_pol;

    if(!indices.empty())
    {
        // Add first point
        sorted_angles.push_back(angles[indices[0]]);
        sorted_pol.push_back(pol_[indices[0]]);

        // Process remaining points
        for(size_t i = 1; i < indices.size(); i++)
        {
            size_t current_idx = indices[i];
            size_t prev_idx = indices[i-1];

            // If angle is different from previous, add the point
            if(angles[current_idx] != angles[prev_idx]) {
                sorted_angles.push_back(angles[current_idx]);
                sorted_pol.push_back(pol_[current_idx]);
            }
            // If angle is same, keep the one with greater distance from pivot
            else
            {
                double current_dist = getDistance(pol_[current_idx], q);
                double prev_dist = getDistance(sorted_pol.back(), q);
                
                if(current_dist > prev_dist)
                {
                    sorted_pol.back() = pol_[current_idx];
                }
            }
        }
    }

    // Update original vectors
    angles = sorted_angles;
    pol_ = sorted_pol;
}


QPolygonF Algorithms::createCHGS(const QPolygonF &pol)
{
    // Create convex hull using Graham scan
    QPolygonF ch;


    // If the polygon has only 3 vertexes, return polygon as CH
    if (pol.size() == 3)
        return pol;

    // Get pivot q
    QPointF q = findPivotGS(pol);

    // Create a polygon without the pivot
    QPolygonF pol_;
    for(QPointF point: pol)
    {
        // Check if there is point with the same coordinates as pivot - leave him
        if((point != q))
        {
            pol_.push_back(point);
        }
    }

    // Vector of angles
    std::vector<double> angles  = anglesWithPoints(pol_,q);

    // Sort and filter angles and points in pol_
    sortAnglesPoints(q,angles,pol_);

    // Input pivot and and 2 last point from the angles to the convexhull
    ch.push_back(q);
    ch.push_back(pol_[0]);
    ch.push_back(pol_[1]);

    for (int i = 2; i < pol_.size(); ++i) {
        QPointF candidate = pol_[i];

        while (ch.size() >= 2 && findSide(ch[ch.size() - 2], ch[ch.size() - 1], candidate)==-1) {
            ch.pop_back(); // remove last point from convex hull
        }

        ch.push_back(candidate);
    }

    return ch;
}


QPolygonF Algorithms::createMAER(const QPolygonF &pol)
{
    // Create minimun area enclosing rectangle over the building
    double sigma_min = 2* M_PI;

    // Construct min-max box
    auto [mmbox_min, area_min] = minMaxBox(pol);

    // Create CH
    QPolygonF ch = createCHJS(pol);

    // Process all segments of CG
    int n = ch.size();
    for(int i = 0; i < n; i++)
    {
        // Get ch segment and its coordinate differencies
        double dx = ch[(i+1)%n].x()-ch[i].x();
        double dy = ch[(i+1)%n].y()-ch[i].y();

        // Get angle of rotation
        double sigma = atan2(dy, dx);

        // Rotation by -sigma
        QPolygonF ch_rot = rotate(ch, -sigma);

        // Compute min-max box
        auto [mmbox, area] = minMaxBox(ch_rot);

        // Update minimum
        if(area < area_min)
        {
            // Update minimum
            area_min = area;
            sigma_min = sigma;
            mmbox_min = mmbox;
        }

    }
    // Resize mmbox
    QPolygonF mmbox_min_res = resize(pol, mmbox_min);

    // Rotate min-max box with mimimum area
    return rotate(mmbox_min_res, sigma_min);

}


QPolygonF Algorithms::createERPCA(const QPolygonF &pol)
{
    // Create area enclosing rectangle over the building using PCA

    int n = pol.size();

    // Create matrix A
    Eigen::MatrixXd A(n, 2);

    // Add points to the matrix
    for(int i = 0; i < n; i++)
    {
        A(i,0) = pol[i].x();
        A(i,1) = pol[i].y();
    }

    // Compute means of coordinates over columns
    Eigen::RowVector2d M = A.colwise().mean();

    // Subtract mean: B = A - M
    Eigen::MatrixXd B = A.rowwise() - M;

    // Covariance matrix: C = B' * B / (m - 1)
    Eigen::MatrixXd C = (B.adjoint() * B) / double(A.rows() - 1);

    // Compute SVD, full version: [U, S, V] = svd(C)
    Eigen::JacobiSVD<Eigen::MatrixXd> svd(C, Eigen::ComputeFullV | Eigen::ComputeFullU);

    // Get matrices
    Eigen::MatrixXd U = svd.matrixU();
    Eigen::MatrixXd S = svd.singularValues();
    Eigen::MatrixXd V = svd.matrixV();

    // Compute sigma
    double sigma = atan2(V(1,0), V(0,0));

    // Rotation by -sigma
    QPolygonF pol_rot = rotate(pol, -sigma);

    // Compute min-max box
    auto [mmbox, area] = minMaxBox(pol_rot);

    // Resize mmbox
    QPolygonF mmbox_min_res = resize(pol, mmbox);

    // Rotate min-max box with mimimum area
    return rotate(mmbox_min_res, sigma);

}


QPolygonF Algorithms::createERLE(const QPolygonF &pol)
{
    // Create area enclosing rectangle over the building using longest edge

    double maxLength = 0;
    double dx = 0, dy = 0;

    int n = pol.size();

    // Find the longest edge
    for (int i = 0; i < n; ++i)
    {
        double dx_i = pol[(i+1) % n].x() - pol[i].x();
        double dy_i = pol[(i+1) % n].y() - pol[i].y();
        double length = sqrt(dx_i * dx_i + dy_i * dy_i);

        // Find the maximum length and its direction
        if (length > maxLength)
        {
            maxLength = length;
            dx = dx_i;
            dy = dy_i;
        }
    }

    // Compute the angle (sigma) of the longest edge
    double sigma = atan2(dy, dx);

    // Rotation by -sigma
    QPolygonF pol_rot = rotate(pol, -sigma);

    // Compute min-max box
    auto [mmbox, area] = minMaxBox(pol_rot);

    // Resize mmbox
    QPolygonF mmbox_min_res = resize(pol, mmbox);

    // Rotate min-max box with mimimum area
    return rotate(mmbox_min_res, sigma);
}


QPolygonF Algorithms::createERWA(const QPolygonF &pol)
{
    // Create area enclosing rectangle over the building using wall average

    int n = pol.size();
    double sigma = 0;
    double Si_sum = 0;

    // Compute initial direction sigma_12
    double dx = pol[1].x() - pol[0].x();
    double dy = pol[1].y() - pol[0].y();
    double sigma_12 = atan2(dy, dx);

    // Iterate over each vertex
    for (int i = 0; i < n; i++)
    {
        // Compute directions for segments
        double dx1 = pol[(i - 1 + n) % n].x() - pol[i].x();
        double dy1 = pol[(i - 1 + n) % n].y() - pol[i].y();
        double sigma_i = atan2(dy1, dx1);

        double dx2 = pol[(i + 1) % n].x() - pol[i].x();
        double dy2 = pol[(i + 1) % n].y() - pol[i].y();
        double sigma_i1 = atan2(dy2, dx2);
        double Si = sqrt(dx2 * dx2 + dy2 * dy2);

        // Compute internal angle omega_i
        double omega_i = std::abs(sigma_i - sigma_i1);

        // Compute multiple of π/2
        double k_i = (2 * omega_i) / M_PI;

        // Compute oriented remainder
        double ri = (k_i - floor(k_i)) * (M_PI / 2);
        if (fmod(omega_i, M_PI / 2) > (M_PI / 4))
        {
            ri = (M_PI / 2) - ri;
        }

        // Weighted average sums
        sigma += ri * Si;
        Si_sum += Si;
    }

    // Weighted average
    sigma = sigma_12 + sigma / Si_sum;

    // Rotation by -sigma
    QPolygonF pol_rot = rotate(pol, -sigma);

    // Compute min-max box
    auto [mmbox, area] = minMaxBox(pol_rot);

    // Resize mmbox
    QPolygonF mmbox_min_res = resize(pol, mmbox);

    // Rotate min-max box with mimimum area
    return rotate(mmbox_min_res, sigma);
}


QPolygonF Algorithms::createERWB(const QPolygonF &pol)
{
    // Create area enclosing rectangle over the building using weighted bisector

    double max_diag_1 = 0, max_diag_2 = 0;
    double dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
    int n = pol.size();

    // Find the two longest diagonals
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j) {
            // Calculate differences in coordinates between vertices
            double dxi = pol[(i + j + 2) % n].x() - pol[i % n].x();
            double dyi = pol[(i + j + 2) % n].y() - pol[i % n].y();

            // Calculate the length of the diagonal using the Pythagorean theorem
            double len_i = sqrt(dxi * dxi + dyi * dyi);

            // If the length is greater than the current longest diagonal
            if (len_i > max_diag_1)
            {   // Update the second longest diagonal
                max_diag_2 = max_diag_1;
                // Update the longest diagonal
                max_diag_1 = len_i;
                // Update the directions of the diagonals
                dx2 = dx1;
                dy2 = dy1;
                dx1 = dxi;
                dy1 = dyi;
            // If the length is greater than the second longest diagonal
            } else if (len_i > max_diag_2 && len_i < max_diag_1)
            {
                max_diag_2 = len_i;
                dx2 = dxi;
                dy2 = dyi;
            }
        }
    }

    // Direction of the longest diagonals
    double sigma1 = atan2(dy1, dx1);
    double sigma2 = atan2(dy2, dx2);

    // Weighted average direction of diagonals
    double sigma = (sigma1 * max_diag_1 + sigma2 * max_diag_2) / (max_diag_1 + max_diag_2);

    // Rotate polygon according to the computed direction
    QPolygonF pol_rot = rotate(pol, -sigma);

    // Create minimum bounding box
    auto [mmbox, area] = minMaxBox(pol_rot);

    // Resize the bounding box
    QPolygonF mmbox_min_res = resize(pol, mmbox);

    // Rotate the minimum bounding box back to the original direction
    return rotate(mmbox_min_res, sigma);
}

void Algorithms::exportFile(const std::vector<QPolygonF> &results,const QString &fileName)
{
    // Export results into txt file

    // Create file
    QFile file(fileName);

    // Opens file
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);

    // Go through all results
    for(QPolygonF polygon: results)
    {
        // Go thtough all points
        for(QPointF point: polygon)
        {
            // For each point print x and y coordinate
            out << point.x()<<", "<<point.y() << "\n";
        }
        // After polygon print 2 empty line
        out << "\n \n";
    }
    // Close file
    file.close();
}

double Algorithms::pointLineDistance(const QPointF& start, const QPointF& end, const QPointF& p)
{
    // Get distance point from line
    return abs((end.x() - start.x()) * (start.y() - p.y()) - (start.x() - p.x()) * (end.y() - start.y()));

}

short Algorithms::findSide(const QPointF& a, const QPointF& b, const QPointF& p) {
    // Define on which side of the line is point
    double val = (b.x() - a.x()) * (p.y() - a.y()) - (b.y() - a.y()) * (p.x() - a.x());
    // left    =  1
    // right   = -1
    // on line =  0
    return (val > 0) ? 1 : (val < 0) ? -1 : 0;
}

double Algorithms::getMainDirection(const QPolygonF &rect)
{
    // Finds the main direction of a rectangle
    double longestLength = 0; // store the longest edge length
    double direction = 0;     // store the angle of the longest edge

    for (int i = 0; i < rect.size(); ++i) {
        // Get two points of one edge
        QPointF a = rect[i];
        QPointF b = rect[(i + 1) % rect.size()];

        // Compute difference in x and y
        double dx = b.x() - a.x();
        double dy = b.y() - a.y();

        // Compute length of the edge
        double length = std::sqrt(dx * dx + dy * dy);

        // If this edge is the longest, save its length and angle
        if (length > longestLength) {
            longestLength = length;
            direction = std::atan2(dy, dx);// angle of the edge
        }
    }
    return direction;
}

std::vector<double> Algorithms::segmentAngleDeviations(const QPolygonF &polygon, double main_dir)
{
    // Calculates deviations of each polygon edge
    std::vector<double> deviations;

    for (int i = 0; i < polygon.size() - 1; ++i) {
        // Get two points of the edge
        QPointF p1 = polygon[i];
        QPointF p2 = polygon[i + 1];

        // Calculate direction of this edge
        double dx = p2.x() - p1.x();
        double dy = p2.y() - p1.y();
        double sigma_i = std::atan2(dy, dx);

        double k_i = (2.0 * sigma_i) / M_PI;
        double r_i = (k_i - std::floor(k_i)) * (M_PI / 2.0);

        double k = (2.0 * main_dir) / M_PI;
        double r = (k - std::floor(k)) * (M_PI / 2.0);

        // Calculate absolute difference
        double diff = std::abs(r_i - r);

        // Apply correction: map to [0, π/4] range
        if (diff > M_PI / 4.0) {
            diff = M_PI / 2.0 - diff;
        }

        // Save the difference
        deviations.push_back(diff);
    }
    return deviations;
}

void Algorithms::evaluateAccuracy(const QPolygonF &original, const QPolygonF &generalized, double &delta_sigma_1, double &delta_sigma_2)
{
    // Get the main direction (angle) of the generalized polygon
    double sigma = getMainDirection(generalized);

    // Get angular deviations of the original polygon from this direction
    std::vector<double> deviations = segmentAngleDeviations(original, sigma);

    // If the polygon is empty, return 0 values
    if (deviations.empty()) {
        delta_sigma_1 = 0.0;
        delta_sigma_2 = 0.0;
        return;
    }

    double sum_abs = 0.0;
    double sum_sq = 0.0;

    // Sum all absolute and squared deviations
    for (double d : deviations) {
        sum_abs += std::abs(d);
        sum_sq += d * d;
    }

    // Compute mean angular deviation (Δσ1)
    delta_sigma_1 = (M_PI / (2.0 * deviations.size())) * sum_abs;

    // Compute root mean square angular deviation (Δσ2)
    delta_sigma_2 = (M_PI / (2.0 * deviations.size())) * std::sqrt(sum_sq);
}

