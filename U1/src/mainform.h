#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>

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

private slots:
    void on_actionPoint_Polygon_triggered();

    void on_actionRay_Crossing_triggered();

    void on_actionWinding_Number_triggered();

    void on_actionOpen_triggered();

    void on_actionClear_data_triggered();

    void on_actionClear_all_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainForm *ui;
};
#endif // MAINFORM_H
