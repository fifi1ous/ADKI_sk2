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


    bool inside = false;
    bool edge = false;
    bool vertex = false;

    // Clear previous selections
    ui->Canvas->clearSelectedPolygons();

    for (size_t i = 0; i < polygons.size(); ++i)
    {
        if (Algorithms::minMaxBox(q, polygons[i]))
        {
            short res = Algorithms::analyzeRayCrossing(q, polygons[i]);

            // Cases of poin position
            switch (res)
            {
            case 1:
                inside = true;
                break;
            case 2:
                edge = true;
                break;
            case 3:
                vertex = true;
                break;
            case 0:
                break;
            default:
                setWindowTitle("Something went wrong");
                return;
            }

            if (res > 0)
            {
                ui->Canvas->addSelectedPolygon(polygons[i]);
            }
        }
    }

    // Print of results for all polygon
    if (inside)
        setWindowTitle("Inside");
    else if (edge)
        setWindowTitle("Point is on the edge");
    else if (vertex)
        setWindowTitle("Point is on the vertex");
    else
        setWindowTitle("Outside");
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

    bool inside = false;
    bool edge = false;
    bool vertex = false;

    // Clear previous selections
    ui->Canvas->clearSelectedPolygons();

    for (size_t i = 0; i < polygons.size(); ++i)
    {
        if (Algorithms::minMaxBox(q, polygons[i]))
        {
            short res = Algorithms::analyzeWindingNumber(q, polygons[i]);

            // Cases of poin position
            switch (res)
            {
            case 1:
                inside = true;
                break;
            case 2:
                edge = true;
                break;
            case 3:
                vertex = true;
                break;
            case 0:
                break;
            default:
                setWindowTitle("Something went wrong");
                return;
            }

            if (res > 0)
            {
                ui->Canvas->addSelectedPolygon(polygons[i]);
            }
        }
    }

    // Print of results for all polygon
    if (inside)
        setWindowTitle("Inside");
    else if (edge)
        setWindowTitle("Point is on the edge");
    else if (vertex)
        setWindowTitle("Point is on the vertex");
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

void MainForm::on_actionClear_data_triggered()
{
    setWindowTitle("Analyze point and polygon position");
    ui->Canvas->clearSelectedPolygons();
    ui->Canvas->repaint();
}

void MainForm::on_actionClear_all_triggered()
{
    setWindowTitle("Analyze point and polygon position");
    ui->Canvas->clearSelectedPolygons();
    ui->Canvas->clearPolygons();
    ui->Canvas->repaint();
}


void MainForm::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainForm::on_actionShapefile_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Otevřít Shapefile", "", "Shapefile (*.shp);;All Files (*)");
    if (!fileName.isEmpty())
    {
        ui->Canvas->loadPolygonFromShapefile(fileName);
    }
}
