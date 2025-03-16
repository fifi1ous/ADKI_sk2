#include "mainform.h"
#include "./ui_mainform.h"
#include "algorithms.h"

#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

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
    // Get the point
    QPointF q = ui->Canvas->getQ();
    // Get the polygons
    const std::vector<Polygon_>& polygons = ui->Canvas->getPolygons();
    const std::vector<QPainterPath>& PolygonsP = ui->Canvas->getPolygonsWH();

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
        // Extract the holes and polygons into separete variables
        QPolygonF polygon = polygons[i].outer;
        std::vector<QPolygonF> holes = polygons[i].holes;
        // Set helping variables
        short resH = 0;
        short res = 0;

        if (Algorithms::minMaxBox(q, polygon))
        {
            // Check if the polygon has holes
            if (!holes.empty() )
            {
                // Check if the point is in the hole
                for (const auto& hole : holes)
                {
                    resH = Algorithms::analyzeRayCrossing(q, hole);
                }
            }

            // If not check the polygon
            if (resH != 1)
            {
                res = Algorithms::analyzeRayCrossing(q, polygon);

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
            }
            // Checks if the point is inside
            if (res > 0)
            {
                // returns seleceted polygon
                ui->Canvas->addSelectedPolygon(PolygonsP[i]);
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
    // Get the point
    QPointF q = ui->Canvas->getQ();
    // Get the polygons
    const std::vector<Polygon_>& polygons = ui->Canvas->getPolygons();
    const std::vector<QPainterPath>& PolygonsP = ui->Canvas->getPolygonsWH();

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
        // Extract the holes and polygons into separete variables
        QPolygonF polygon = polygons[i].outer;
        std::vector<QPolygonF> holes = polygons[i].holes;
        // Set helping variables
        short resH = 0;
        short res = 0;

        if (Algorithms::minMaxBox(q, polygon))
        {
            // Check if the polygon has holes
            if (!holes.empty() )
            {
                // Check if the point is in the hole
                for (const auto& hole : holes)
                {
                    resH = Algorithms::analyzeWindingNumber(q, hole);
                }
            }

            // If not check the polygon
            if (resH != 1)
            {
                res = Algorithms::analyzeWindingNumber(q, polygon);

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
            }

            // Checks if the point is inside
            if (res > 0)
            {
                // returns seleceted polygon
                ui->Canvas->addSelectedPolygon(PolygonsP[i]);
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
    // Open a file dialog to select a polygon file
    QString fileName = QFileDialog::getOpenFileName(this, "Open Polygon File", "", "Text Files (*.txt);;Shapefile (*.shp);;All Files (*)");
    if (!fileName.isEmpty())
    {
        if (fileName.endsWith(".txt"))
        {
            // Load polygon from a text file
            ui->Canvas->loadPolygonFromFile(fileName);
        }
        else if (fileName.endsWith(".shp"))
        {
            // Load polygon from a shapefile
            ui->Canvas->loadPolygonFromShapefile(fileName);
        }
        else
        {
            // Show an error message for unsupported file formats
            QMessageBox::warning(this, "Error", "Unsupported file format.");
        }
    }
}

void MainForm::on_actionClear_data_triggered()
{
    // Reset the window title
    setWindowTitle("Analyze point and polygon position");
    // Clear selected polygons from the canvas
    ui->Canvas->clearSelectedPolygons();
    // Repaint the canvas
    ui->Canvas->repaint();
}

void MainForm::on_actionClear_all_triggered()
{
    // Reset the window title
    setWindowTitle("Analyze point and polygon position");
    // Clear selected polygons from the canvas
    ui->Canvas->clearSelectedPolygons();
    // Clear all polygons from the canvas
    ui->Canvas->clearPolygons();
    // Repaint the canvas
    ui->Canvas->repaint();
}

void MainForm::on_actionExit_triggered()
{
    // Quit the application
    QApplication::quit();
}

