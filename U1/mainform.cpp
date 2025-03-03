#include "mainform.h"
#include "./ui_mainform.h"
#include "algorithms.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

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

    short res = Algorithms::analyzeRayCrossing(q, pol);

    //show results
    if(res == 1)
        setWindowTitle("Inside");
    else
        setWindowTitle("Outside");
}


void MainForm::on_actionWinding_Number_triggered()
{
    // Run winding number algorithm
    QPointF q = ui->Canvas->getQ();
    QPolygonF pol = ui->Canvas->getPol();

    short res = Algorithms::analyzeWindingNumber(q, pol);

    //show results
    if(res == 1)
        setWindowTitle("Inside");
    else
        setWindowTitle("Outside");
}


void MainForm::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Polygon File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty())
    {
        ui->Canvas->loadPolygonFromFile(fileName);
    }
}

