#include "mainform.h"
#include "ui_mainform.h"
#include "qpoint3df.h"
#include "algorithms.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QDesktopServices>
#include <QUrl>
#include <set>

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);

    ui->Canvas->setSettings(&settings);

    // inicialize variables
    view_points = true;
    view_dt = true;
    view_contour_lines = true;
    view_aspect = true;
    view_slope = true;

    //check/uncheck menu items
    ui -> actionPoints -> setChecked(view_points);
    ui -> actionDT -> setChecked(view_dt);
    ui -> actionContour_Lines -> setChecked(view_contour_lines);
    ui -> actionExposition -> setChecked(view_aspect);
    ui -> actionSlope -> setChecked(view_slope);
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

    // Check if there are data to process
    if(!points.empty())
    {
        //Perform DT
        Algorithms a;
        std::vector<Edge> dt = a.DT(points);

        //Set results
        ui -> Canvas -> setDT(dt);

        //Repaint
        repaint();

        // change clicked
        ui ->Canvas -> setClicked(false);
    }
}


void MainForm::on_actionCreate_Contour_lines_triggered()
{


    //Generate contour lines

    //Get data
    std::vector<QPoint3DF> points = ui -> Canvas -> getPoints();

    // Check if there are data to process
    if(!points.empty())
    {
        // Get DT
        std::vector<Edge> dt = ui -> Canvas -> getDT();

        //Perform DT
        Algorithms a;

        bool click = ui -> Canvas ->getClicked();
        if (dt.empty() || click)
        {
            //Get data
            std::vector<QPoint3DF> points = ui -> Canvas -> getPoints();

            //Perform DT
            dt = a.DT(points);

            //Set results
            ui -> Canvas -> setDT(dt);
        }

        // Get parameters of contoru lines
        double z_min = settings.getZMin();
        double z_max = settings.getZMax();
        double dz = settings.getDz();

        //Generate contour lines
        std::vector<Edge> cl = a.createContourLines(dt,z_min, z_max, dz);

        //Set results
        ui -> Canvas -> setCL(cl);

        // Change clicked
        ui ->Canvas -> setClicked(false);

        //Repaint
        repaint();
    }
}


void MainForm::on_actionParameters_triggered()
{
    // Show window
    settings.exec();
}


void MainForm::on_actionAnalyze_slope_triggered()
{
    // calculate slope

    //Get data
    std::vector<QPoint3DF> points = ui -> Canvas -> getPoints();

    // Check if there are data to process
    if(!points.empty())
    {
        // Get Delaunay triangulation
        std::vector<Edge> dt = ui -> Canvas -> getDT();

        //Perform DT
        Algorithms a;

        bool click = ui -> Canvas ->getClicked();
        if (dt.empty() || click)
        {

            //Perform DT
            dt = a.DT(points);

            //Set results
            ui -> Canvas -> setDT(dt);
        }

        a.analyzeSlope(dt,tr1,click);

        //Set results
        ui -> Canvas -> setTR(tr1);

        //Check visibility of slope
        view_slope = ui -> actionSlope -> isChecked();
        if(!view_slope)
        {
            // If not visible set as visible
            ui -> Canvas ->setViewSlope(!view_slope);
            ui -> actionSlope -> setChecked(!view_slope);
        }

        // Set slope as invisible
        ui -> actionExposition -> setChecked(false);
        ui -> Canvas ->setViewAspect(false);

        //Repaint
        repaint();

        // Change clicked
        ui ->Canvas -> setClicked(false);
    }
}


void MainForm::on_actionClear_Results_triggered()
{
    ui -> Canvas ->clearResults();

    repaint();
}


void MainForm::on_actionPoints_changed()
{
    view_points = ui -> actionPoints -> isChecked();

    ui -> Canvas ->setViewPoints(view_points);

    repaint();
}


void MainForm::on_actionDT_changed()
{
    view_dt = ui -> actionDT -> isChecked();

    ui -> Canvas ->setViewDT(view_dt);

    repaint();
}


void MainForm::on_actionContour_Lines_changed()
{
    view_contour_lines = ui -> actionContour_Lines -> isChecked();

    ui -> Canvas ->setViewContourLines(view_contour_lines);

    repaint();
}


void MainForm::on_actionSlope_changed()
{
    view_slope = ui -> actionSlope -> isChecked();

    ui -> Canvas ->setViewSlope(view_slope);

    repaint();
}


void MainForm::on_actionExposition_changed()
{
    view_aspect = ui -> actionExposition -> isChecked();

    ui -> Canvas ->setViewAspect(view_aspect);

    repaint();
}


void MainForm::on_actionClear_All_triggered()
{
    ui -> Canvas ->clearAll();
    tr1.clear();

    repaint();
}


void MainForm::on_actionAnalyze_exposition_triggered()
{
    // calculate slope

    // Get data
    std::vector<QPoint3DF> points = ui -> Canvas -> getPoints();

    // Check if there are data to process
    if(!points.empty())
    {
        // Get Delaunay triangulation
        std::vector<Edge> dt = ui -> Canvas -> getDT();

        //Perform DT
        Algorithms a;

        bool click = ui -> Canvas ->getClicked();
        if (dt.empty() || click)
        {
            //Perform DT
            dt = a.DT(points);

            //Set results
            ui -> Canvas -> setDT(dt);
        }

        a.analyzeAspect(dt,tr1,click);

        //Set results
        ui -> Canvas -> setTR(tr1);

        //Check visibility of aspect
        view_aspect = ui -> actionExposition -> isChecked();
        if(!view_aspect)
        {
            // If not visible set as visible
            ui -> Canvas ->setViewAspect(!view_aspect);
            ui -> actionExposition -> setChecked(!view_aspect);
        }

        // Set slope as invisible
        ui -> actionSlope -> setChecked(false);
        ui -> Canvas ->setViewSlope(false);

        //Repaint
        repaint();

        // Change clicked
        ui ->Canvas -> setClicked(false);
    }
}

void MainForm::on_actionOpen_triggered()
{
    // Otevření souborového dialogu pro výběr souboru
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Points File"), "", tr("Text Files (*.txt *.xyz)"));

    // Pokud je soubor vybrán
    if (!fileName.isEmpty())
    {
        // Zavoláme metodu pro načtení bodů
        ui->Canvas->loadPointsFromTextfile(fileName);
    }
}

void MainForm::on_actionExit_triggered()
{
    // Quit the application
    QApplication::quit();
}

void MainForm::on_actionHill_triggered()
{
    int canvasWidth = ui->Canvas->width();
    int canvasHeight = ui->Canvas->height();

    int centerX = QRandomGenerator::global()->bounded(100, canvasWidth - 100);
    int centerY = QRandomGenerator::global()->bounded(100, canvasHeight - 100);
    int radiusX = QRandomGenerator::global()->bounded(100, canvasWidth / 2);
    int radiusY = QRandomGenerator::global()->bounded(100, canvasHeight / 2);
    int peakZ = QRandomGenerator::global()->bounded(800, 1200);

    Algorithms a;
    std::vector<QPoint3DF> points = a.generateHill(300, canvasWidth, canvasHeight, centerX, centerY, radiusX, radiusY, peakZ);

    auto existingPoints = ui->Canvas->getPoints();
    existingPoints.insert(existingPoints.end(), points.begin(), points.end());
    ui->Canvas->setPoints(existingPoints);
    ui->Canvas->repaint();
}

void MainForm::on_actionValley_triggered()
{
    int canvasWidth = ui->Canvas->width();
    int canvasHeight = ui->Canvas->height();

    int centerX = QRandomGenerator::global()->bounded(100, canvasWidth - 100);
    int centerY = QRandomGenerator::global()->bounded(100, canvasHeight - 100);
    int radiusX = QRandomGenerator::global()->bounded(100, canvasWidth / 2);
    int radiusY = QRandomGenerator::global()->bounded(100, canvasHeight / 2);
    int depthZ = QRandomGenerator::global()->bounded(200, 600);

    Algorithms a;
    std::vector<QPoint3DF> points = a.generateValley(300, canvasWidth, canvasHeight, centerX, centerY, radiusX, radiusY, depthZ);

    auto existingPoints = ui->Canvas->getPoints();
    existingPoints.insert(existingPoints.end(), points.begin(), points.end());
    ui->Canvas->setPoints(existingPoints);
    ui->Canvas->repaint();
}

void MainForm::on_actionRidge_triggered()
{
    int canvasWidth = ui->Canvas->width();
    int canvasHeight = ui->Canvas->height();

    int startX = QRandomGenerator::global()->bounded(100, canvasWidth - 100);
    int startY = QRandomGenerator::global()->bounded(100, canvasHeight - 100);
    int endX = QRandomGenerator::global()->bounded(100, canvasWidth - 100);
    int endY = QRandomGenerator::global()->bounded(100, canvasHeight - 100);
    int ridgeZ = QRandomGenerator::global()->bounded(800, 1200);

    Algorithms a;
    std::vector<QPoint3DF> points = a.generateRidge(300, canvasWidth, canvasHeight, startX, startY, endX, endY, ridgeZ);

    auto existingPoints = ui->Canvas->getPoints();
    existingPoints.insert(existingPoints.end(), points.begin(), points.end());
    ui->Canvas->setPoints(existingPoints);
    ui->Canvas->repaint();
}

void MainForm::on_actionBench_triggered()
{
    int canvasWidth = ui->Canvas->width();
    int canvasHeight = ui->Canvas->height();

    int stepStartX = QRandomGenerator::global()->bounded(100, canvasWidth / 2);
    int stepEndX = stepStartX + QRandomGenerator::global()->bounded(50, canvasWidth / 3);
    int benchZ = QRandomGenerator::global()->bounded(200, 400);

    Algorithms a;
    std::vector<QPoint3DF> points = a.generateBench(300, canvasWidth, canvasHeight, stepStartX, stepEndX, benchZ);

    auto existingPoints = ui->Canvas->getPoints();
    existingPoints.insert(existingPoints.end(), points.begin(), points.end());
    ui->Canvas->setPoints(existingPoints);
    ui->Canvas->repaint();
}

void MainForm::on_actionSaddle_triggered()
{
    int canvasWidth = ui->Canvas->width();
    int canvasHeight = ui->Canvas->height();

    int centerX = QRandomGenerator::global()->bounded(100, canvasWidth - 100);
    int centerY = QRandomGenerator::global()->bounded(100, canvasHeight - 100);
    int scaleX = QRandomGenerator::global()->bounded(100, canvasWidth / 3);
    int scaleY = QRandomGenerator::global()->bounded(100, canvasHeight / 3);

    Algorithms a;
    std::vector<QPoint3DF> points = a.generateSaddle(300, canvasWidth, canvasHeight, centerX, centerY, scaleX, scaleY);

    auto existingPoints = ui->Canvas->getPoints();
    existingPoints.insert(existingPoints.end(), points.begin(), points.end());
    ui->Canvas->setPoints(existingPoints);
    ui->Canvas->repaint();
}



