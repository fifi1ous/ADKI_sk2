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
    void on_actionCreate_DT_triggered();

    void on_actionCreate_Contour_lines_triggered();

private:
    Ui::MainForm *ui;
};
#endif // MAINFORM_H
