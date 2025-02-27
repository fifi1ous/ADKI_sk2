#include "mainform.h"
#include "./ui_mainform.h"
#include "algorithms.h"

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::on_actionPoint_Polygon_triggered()
{
    //Change input: point q or polygon P
    ui->Canvas->switch_source();
}


void MainForm::on_actionRay_Crossing_triggered()
{
    // Run raycrossing algorithm
    QPointF q = ui->Canvas->getQ();
    QPolygonF pol = ui->Canvas->getPol();

    short res = Algorithms::analyzePointAndPolygonPosition(q, pol);

    //show results
    if(res == 1)
        setWindowTitle("Inside");
    else
        setWindowTitle("Outside");
}

