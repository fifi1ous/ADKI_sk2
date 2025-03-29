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
    //Get all polygons
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    // Check if the polygons are empty
    if (polygons.empty()) {
        QMessageBox::warning(this, "Error", "No polygons to process.");
        return;
    }

    //Use std::vector to store the results
    std::vector<QPolygonF> results;

    // For each polygon in the list of polygons, create the MBR
    for (const auto& building : polygons) {
        // If the polygon has at least 3 points
        if (building.size() < 3) {
            continue;  // Skip invalid polygons
        }

        //Run the algorithm to create the MBR
        QPolygonF maer = Algorithms::createMAER(building);

        //Store the result
        results.push_back(maer);
    }

    //Set the results on the Canvas
    ui->Canvas->setResults(results);

    //Repaint the canvas to display the updated results
    ui->Canvas->repaint();
}


void MainForm::on_actionPCA_triggered()
{
    //Create minimum bounding rectangle

    // Get all polygons
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    // Check if there are no polygons to process
    if (polygons.empty()) {
        QMessageBox::warning(this, "Error", "No polygons to process.");
        return;
    }

    // Use std::vector to store the PCA results
    std::vector<QPolygonF> results;

    // For each polygon in the list of polygons, create the PCA
    for (const auto& building : polygons) {
        // If the polygon has at least 3 points
        if (building.size() < 3) {
            continue;  // Skip invalid polygons
        }

        // Run the algorithm to create the PCA
        QPolygonF er = Algorithms::createERPCA(building);

        // Store the result in the results vector
        results.push_back(er);
    }

    // Set the PCA results on the Canvas
    ui->Canvas->setResults(results);

    // Repaint the canvas to display the updated results
    ui->Canvas->repaint();
}


void MainForm::on_actionClear_All_triggered()
{
    // Clear all results from the canvas
    ui->Canvas->clearResults();
    // Clear all CHs from the canvas
    ui->Canvas->clearCHs();
    // Clear all polygons from the canvas
    ui->Canvas->clearPolygons();
    // Repaint the canvas
    ui->Canvas->repaint();
}


void MainForm::on_actionClear_results_triggered()
{
    // Clear all results from the canvas
    ui->Canvas->clearResults();
    // Clear all CHs from the canvas
    ui->Canvas->clearCHs();
    // Repaint the canvas
    ui->Canvas->repaint();
}


void MainForm::on_actionExit_triggered()
{
    // Quit the application
    QApplication::quit();
}

void Draw::setResults(const std::vector<QPolygonF>& newResults)
{
    results.clear();  // Clear previous results
    for (const auto& result : newResults) {
        results.push_back(result);  // Add new results
    }
    repaint();  // Repaint to show results
}

void MainForm::on_actionLongest_edge_triggered()
{
    //Create minimum bounding rectangle

    // Get all polygons
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    // Check if there are no polygons to process
    if (polygons.empty()) {
        QMessageBox::warning(this, "Error", "No polygons to process.");
        return;
    }

    // Use std::vector to store the Longes edge results
    std::vector<QPolygonF> results;

    // For each polygon in the list of polygons, create the Longest edge
    for (const auto& building : polygons) {
        // If the polygon has at least 3 points
        if (building.size() < 3) {
            continue;  // Skip invalid polygons
        }

        // Run the algorithm to create the Longest edge
        QPolygonF er = Algorithms::createERLE(building);

        // Store the result in the results vector
        results.push_back(er);
    }

    // Set the Longest edge results on the Canvas
    ui->Canvas->setResults(results);

    // Repaint the canvas to display the updated results
    ui->Canvas->repaint();
}

void MainForm::on_actionWall_average_triggered()
{
    //Create minimum bounding rectangle

    // Get all polygons
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    // Check if there are no polygons to process
    if (polygons.empty()) {
        QMessageBox::warning(this, "Error", "No polygons to process.");
        return;
    }

    // Use std::vector to store the Longes edge results
    std::vector<QPolygonF> results;

    // For each polygon in the list of polygons, create the Longest edge
    for (const auto& building : polygons) {
        // If the polygon has at least 3 points
        if (building.size() < 3) {
            continue;  // Skip invalid polygons
        }

        // Run the algorithm to create the Longest edge
        QPolygonF er = Algorithms::createERWA(building);

        // Store the result in the results vector
        results.push_back(er);
    }

    // Set the Longest edge results on the Canvas
    ui->Canvas->setResults(results);

    // Repaint the canvas to display the updated results
    ui->Canvas->repaint();
}

void MainForm::on_actionWeighted_bisector_triggered()
{
    //Create minimum bounding rectangle

    // Get all polygons
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    // Check if there are no polygons to process
    if (polygons.empty()) {
        QMessageBox::warning(this, "Error", "No polygons to process.");
        return;
    }

    // Use std::vector to store the Longes edge results
    std::vector<QPolygonF> results;

    // For each polygon in the list of polygons, create the Longest edge
    for (const auto& building : polygons) {
        // If the polygon has at least 3 points
        if (building.size() < 3) {
            continue;  // Skip invalid polygons
        }

        // Run the algorithm to create the Longest edge
        QPolygonF er = Algorithms::createERWB(building);

        // Store the result in the results vector
        results.push_back(er);
    }

    // Set the Longest edge results on the Canvas
    ui->Canvas->setResults(results);

    // Repaint the canvas to display the updated results
    ui->Canvas->repaint();
}

void MainForm::on_actionCovvex_Hull_ON_OFF_triggered() {
    // Get all polygons
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();

    // Check if there are no polygons to process
    if (polygons.empty()) {
        QMessageBox::warning(this, "Error", "No polygons to process.");
        return;
    }

    // Use std::vector to store the convex hull results
    std::vector<QPolygonF> chs;

    // For each polygon in the list of polygons, create the convex hull
    for (const auto& building : polygons) {
        // If the polygon has at least 3 points
        if (building.size() < 3) {
            continue;  // Skip invalid polygons
        }

        // Run the algorithm to create the convex hull
        QPolygonF ch = Algorithms::createCHJS(building);

        // Store the ch in the chs vector
        chs.push_back(ch);
    }

    // Set the convex hull chs on the Canvas
    ui->Canvas->setConvexHulls(chs);

    // Repaint the canvas to display the updated chs
    ui->Canvas->repaint();
}

void MainForm::on_actionAbout_triggered()
{
    //
}
