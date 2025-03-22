#include "mainform.h"
#include "ui_mainform.h"
#include "algorithms.h"

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


void MainForm::on_actionOpen_triggered()
{
    // Open a file dialog to select a polygon file
    QString fileName = QFileDialog::getOpenFileName(this, "Open Polygon File", "", "Text Files (*.txt);;Shapefile (*.shp);;All Files (*)");
    if (!fileName.isEmpty())
    {
        if (fileName.endsWith(".txt"))
        {
            // Load polygon from a text file
            ui->Canvas->loadPolygonFromTextfile(fileName);
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


void MainForm::on_actionMBR_triggered()
{
    //Create minimum bounding rectangle

    //Get data
    QPolygonF building = ui->Canvas->getBuilding();

    // Check if polygon has at least 3 points
    if (building.size() < 3) {
        QMessageBox::warning(this, "Error", "Polygon must have at least three vertices");
        return;
    }

    //Run function data
    QPolygonF maer = Algorithms::createMAER(building);

    //Set results
    ui->Canvas->setMAER(maer);

    //Repaint
    repaint();
}


void MainForm::on_actionPCA_triggered()
{
    //Create minimum bounding rectangle

    //Get data
    QPolygonF building = ui->Canvas->getBuilding();

    //Run function data
    QPolygonF er = Algorithms::createERPCA(building);

    //Set results
    ui->Canvas->setMAER(er);

    //Repaint
    repaint();
}


void MainForm::on_actionClear_All_triggered()
{
    // Clear all polygons from the canvas
    ui->Canvas->clearResults();
    // Clear all polygons from the canvas
    ui->Canvas->clearPolygons();
    // Repaint the canvas
    ui->Canvas->repaint();
}


void MainForm::on_actionClear_results_triggered()
{
    // Clear all polygons from the canvas
    ui->Canvas->clearResults();
    // Repaint the canvas
    ui->Canvas->repaint();
}


void MainForm::on_actionExit_triggered()
{
    // Quit the application
    QApplication::quit();
}
