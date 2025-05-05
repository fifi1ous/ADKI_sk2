#include "settings2.h"
#include "ui_settings2.h"

Settings2::Settings2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Settings2)
{
    ui->setupUi(this);

    // Inicialize values
    z_min = 100;
    z_max =1500;
    dz = 10;

    // set values to spinboxes
    ui->doubleSpinBox->setValue(z_min);
    ui->doubleSpinBox_2->setValue(z_max);
    ui->doubleSpinBox_3->setValue(dz);

}

Settings2::~Settings2()
{
    delete ui;
}



void Settings2::on_doubleSpinBox_editingFinished()
{
    // Actualize zmin
    z_min = ui -> doubleSpinBox->value();
}


void Settings2::on_doubleSpinBox_2_editingFinished()
{
    // Actualize zmax
    z_max = ui -> doubleSpinBox_2->value();
}


void Settings2::on_doubleSpinBox_3_editingFinished()
{
    // Actualize zmax
    dz = ui -> doubleSpinBox_3->value();
}

