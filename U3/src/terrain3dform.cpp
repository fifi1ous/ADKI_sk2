#include "terrain3dform.h"
#include "ui_terrain3dform.h"
#include "terrain3dcanvas.h"
#include <QVBoxLayout>

// Constructor – create 3D dialog window
Terrain3DForm::Terrain3DForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Terrain3DForm)
{
    ui->setupUi(this);

    // Create 3D canvas and add it to layout
    canvas3D = new Terrain3DCanvas(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(canvas3D);
    setLayout(layout);
}

// Destructor – clean up
Terrain3DForm::~Terrain3DForm()
{
    delete ui;
}

// Send points to the 3D canvas for display
void Terrain3DForm::setPoints(const std::vector<QPoint3DF> &points)
{
    if (canvas3D) {
        canvas3D->setPoints(points);
    }
}
