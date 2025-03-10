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
    const std::vector<QPolygonF>& polygons = ui->Canvas->getPolygons();

    if (polygons.empty())
    {
        setWindowTitle("No polygons to analyze");
        return;
    }

    QPolygonF pol = polygons.back();

    short res = Algorithms::analyzeRayCrossing(q, pol);

    if (res == 0)
        setWindowTitle("Outside");
    else if (res == 1)
        setWindowTitle("Inside");
    else if (res == 2)
        setWindowTitle("Point is on the edge");
    else if (res == 3)
        setWindowTitle("Point is on the vertex");
    else
        setWindowTitle("Something went wrong");
}

void MainForm::on_actionWinding_Number_triggered()
{
    // Run winding number algorithm
    QPointF q = ui->Canvas->getQ();
    const std::vector<QPolygonF>& polygons = ui->Canvas->getPolygons();

    if (polygons.empty())
    {
        setWindowTitle("No polygons to analyze");
        return;
    }

    QPolygonF pol = polygons.back();

    short res = Algorithms::analyzeWindingNumber(q, pol);

    if (res == 0)
        setWindowTitle("Outside");
    else if (res == 1)
        setWindowTitle("Inside");
    else if (res == 2)
        setWindowTitle("Point is on the edge");
    else if (res == 3)
        setWindowTitle("Point is on the vertex");
    else
        setWindowTitle("Something went wrong");
}

void MainForm::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Polygon File", "", "Text Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty())
    {
        ui->Canvas->loadPolygonFromFile(fileName);
    }
}

void MainForm::on_actionClear_data_triggered()
{
    ui->Canvas->clearPolygons();
}
