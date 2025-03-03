#include "draw.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

Draw::Draw(QWidget *parent)
    : QWidget{parent}
{
    q.setX(0);
    q.setY(0);
    add_point = false;

}


void Draw::mousePressEvent(QMouseEvent *e)
{
    //Add point to polygon

    //Get x, y coordinates
    double x = e->pos().x();
    double y = e->pos().y();

    //set X y to q
    if(add_point)
    {
        q.setX(x);
        q.setY(y);
    }


    //add point to polygon
    else
    {
        //Create point
        QPointF p(x, y);

        //Add point to polygon
        pol.push_back(p);
    }

    //Repaint screen
    repaint();
}


void Draw::paintEvent(QPaintEvent *event)
{
    //Draw
    QPainter painter(this);

    //Create object for drawing
    painter.begin(this);

    //Set graphic attributes, polygon
    painter.setPen(Qt::GlobalColor::red);
    painter.setBrush(Qt::GlobalColor::yellow);

    //Begin draw
    painter.drawPolygon(pol);

    //Set graphic attributes, point
    painter.setPen(Qt::GlobalColor::black);
    painter.setBrush(Qt::GlobalColor::blue);
    int r = 5; // size of point in pixels

    //Begin draw
    painter.drawEllipse(q.x()-r, q.y()-r, 2*r, 2*r);

    //End draw
    painter.end();
}

void Draw::switch_source()
{
    //input q or polygon vertex
    add_point = !add_point;
}

void Draw::loadPolygonFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Cannot open file for reading");
        return;
    }

    QTextStream in(&file);
    pol.clear(); // Clear the existing polygon

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList coordinates = line.split(",");
        if (coordinates.size() == 2)
        {
            bool ok1, ok2;
            double x = coordinates[0].toDouble(&ok1);
            double y = coordinates[1].toDouble(&ok2);
            if (ok1 && ok2)
            {
                pol.append(QPointF(x, y));
            }
        }
    }

    file.close();
    repaint(); // Repaint the widget to show the new polygon
}
