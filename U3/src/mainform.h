#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "settings2.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainForm;
}
QT_END_NAMESPACE

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();
    Settings2 settings;

private slots:
    void on_actionCreate_DT_triggered();

    void on_actionCreate_Contour_lines_triggered();

    void on_actionParameters_triggered();

private:
    Ui::MainForm *ui;
};
#endif // MAINFORM_H
