#ifndef EDGE_H
#define EDGE_H
#include "qpoint3df.h"

class Edge
{
private:
    QPoint3DF start;
    QPoint3DF end;
public:
    Edge();
    Edge(QPoint3DF &start, QPoint3DF &end){this->start = start; this->end = end;}

    inline QPoint3DF getStart(){return start;}
    inline QPoint3DF getEnd(){return end;}
    inline Edge changeOrientation(){Edge(end, start);}
};

#endif // EDGE_H
