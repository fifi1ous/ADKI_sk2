#include "mainform.h"
#include "ui_mainform.h"
#include "qpoint3df.h"
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

void MainForm::on_actionCreate_DT_triggered()
{
    //Perform Delaunay triangulation

    //Get data
    std::vector<QPoint3DF> points = ui -> Canvas -> getPoints();

    //Perform DT
    Algorithms a;
    std::vector<Edge> dt = a.DT(points);

    //Set results
    ui -> Canvas -> setDT(dt);

    //Repaint
    repaint();
}


void MainForm::on_actionCreate_Contour_lines_triggered()
{
    //Generate contour lines
    std::vector<Edge> dt = ui -> Canvas -> getDT();

    // Get parameters of contoru lines
    double z_min = settings.getZMin();
    double z_max = settings.getZMax();
    double dz = settings.getDz();

    //Generate contour lines
    Algorithms a;
    std::vector<Edge> cl = a.createContourLines(dt,z_min, z_max, dz);

    //Set results
    ui -> Canvas -> setCL(cl);

    //Repaint
    repaint();
}


void MainForm::on_actionParameters_triggered()
{
    // Show window
    settings.exec();
}


void MainForm::on_actionAnalyze_slope_triggered()
{
    // calculate slope
    std::vector<Edge> dt = ui -> Canvas -> getDT();

    //Perform DT
    Algorithms a;

    if (dt.empty())
    {
        //Get data
        std::vector<QPoint3DF> points = ui -> Canvas -> getPoints();

        //Perform DT
        dt = a.DT(points);

        //Set results
        ui -> Canvas -> setDT(dt);

        //Repaint
        repaint();
    }


    std::vector<Traingle> tr = a.analyzeSlope(dt);

    //Set results
    ui -> Canvas -> setSL(tr);

    //Repaint
    repaint();
}


