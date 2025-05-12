#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpoint3df.h"
#include "edge.h"
#include "triangle.h"

class Draw : public QWidget
{
    Q_OBJECT

private:
    bool view_points, view_dt, view_contour_lines, view_aspect, view_slope, clicked;

    std::vector<QPoint3DF> points;
    std::vector<Edge> dt;
    std::vector<Edge> contour_lines;
    std::vector<Triangle> triangles;
    int selectColor(const double &aspect);


public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);

    inline std::vector<QPoint3DF> getPoints()const {return points;}
    inline std::vector<Edge> getDT()const {return dt;}
    inline void setDT(const std::vector<Edge> &dt_){dt = dt_;}
    inline void setCL(const std::vector<Edge> &contour_lines_){contour_lines = contour_lines_;}
    inline void setTR(const std::vector<Triangle> &triangles_){triangles = triangles_;}
    inline void setViewPoints (const bool &view_points_){view_points = view_points_;}
    inline void setViewDT (const bool &view_dt_){view_dt = view_dt_;}
    inline void setViewContourLines (const bool &view_contour_lines_){view_contour_lines = view_contour_lines_;}
    inline void setViewSlope(const bool &view_slope_){view_slope=view_slope_;}
    inline void setViewAspect(const bool &view_aspect_){view_aspect=view_aspect_;}
    inline void setClicked(const bool &clicked_){clicked=clicked_;}
    inline bool getClicked(){return clicked;}

    inline void clearResults(){dt.clear(); triangles.clear(),contour_lines.clear();}
    inline void clearAll(){points.clear(); dt.clear(); triangles.clear(),contour_lines.clear();}

    void loadPointsFromTextfile(const QString &fileName);


signals:
};

#endif // DRAW_H
