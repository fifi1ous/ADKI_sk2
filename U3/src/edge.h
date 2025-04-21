#ifndef EDGE_H
#define EDGE_H
#include "qpoint3df.h"

class Edge
{
private:

    QPoint3DF start;
    QPoint3DF end;


public:

    Edge(const QPoint3DF &start_, const QPoint3DF &end_)
    {
        start = start_;
        end = end_;
    }

    QPoint3DF getStart()const{return start;}
    QPoint3DF getEnd()const{return end;}
    Edge changeOrientation() const {return Edge(end, start);}

    bool operator == (const Edge &e)
    {
        //Compare two edges
        return (start == e.start) && (end == e.end);
    }

};

#endif // EDGE_H
