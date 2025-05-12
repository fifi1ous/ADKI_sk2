#ifndef SETTINGS2_H
#define SETTINGS2_H

#include <QDialog>

namespace Ui {
class Settings2;
}

class Settings2 : public QDialog
{
    Q_OBJECT

private:
    double z_min;
    double z_max;
    double dz;


public:
    explicit Settings2(QWidget *parent = nullptr);
    ~Settings2();
    double getZMin(){return z_min;}
    double getZMax(){return z_max;}
    double getDz(){return dz;}

private slots:

    void on_doubleSpinBox_3_editingFinished();

    void on_doubleSpinBox_2_editingFinished();

    void on_doubleSpinBox_editingFinished();

private:
    Ui::Settings2 *ui;
};

#endif // SETTINGS2_H
