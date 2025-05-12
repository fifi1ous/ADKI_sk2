#include "terrain3dform.h"
#include "ui_terrain3dform.h"
#include "terrain3dcanvas.h"
#include <QVBoxLayout>

Terrain3DForm::Terrain3DForm(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Terrain3DForm)
{
    ui->setupUi(this);

    canvas3D = new Terrain3DCanvas(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(canvas3D);
    setLayout(layout);
}

Terrain3DForm::~Terrain3DForm()
{
    delete ui;
}

void Terrain3DForm::setPoints(const std::vector<QPoint3DF> &points)
{
    if (canvas3D) {
        canvas3D->setPoints(points);
    }
}
