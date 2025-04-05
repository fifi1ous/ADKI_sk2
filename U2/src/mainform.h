#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainForm; }
QT_END_NAMESPACE

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

private slots:
    void on_actionOpen_triggered();

    void on_actionMBR_triggered();

    void on_actionPCA_triggered();

    void on_actionClear_All_triggered();

    void on_actionExit_triggered();

    void on_actionClear_results_triggered();

    void on_actionLongest_edge_triggered();

    void on_actionWall_average_triggered();

    void on_actionWeighted_bisector_triggered();

    void on_actionCovvex_Hull_ON_OFF_triggered();

    void on_actionAbout_triggered();

    void on_actionGraham_Scan_triggered();

    void on_actionJarvis_Scan_triggered();

private:
    Ui::MainForm *ui;
    bool checkValidation(const QPolygonF &building);
};

#endif // MAINFORM_H
